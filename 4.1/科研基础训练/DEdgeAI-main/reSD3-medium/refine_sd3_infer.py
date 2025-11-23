# NOTE: Must have folder `models` with the following files:
# - `clip_g.safetensors` (openclip bigG)
# - `clip_l.safetensors` (OpenAI CLIP-L)
# - `sd3_medium.safetensors` (Main MMDiT model file)

import torch, fire, math
from safetensors import safe_open
from refine_other_impls import SDClipModel, SDXLClipG, SD3Tokenizer
from sd3_impls import BaseModel, sample_euler, SDVAE, CFGDenoiser, SD3LatentFormat
from PIL import Image
import numpy as np
import time

#################################################################################################
### Wrappers for model parts
#################################################################################################

def load_into(f, model, prefix, device, dtype=None):
    """Just a debugging-friendly hack to apply the weights in a safetensors file to the pytorch module."""
    for key in f.keys():
        if key.startswith(prefix) and not key.startswith("loss."):
            path = key[len(prefix):].split(".")
            obj = model
            for p in path:
                if obj is list:
                    obj = obj[int(p)]
                else:
                    obj = getattr(obj, p, None)
                    if obj is None:
                        print(f"Skipping key '{key}' in safetensors file as '{p}' does not exist in python model")
                        break
            if obj is None:
                continue
            try:
                tensor = f.get_tensor(key).to(device=device)
                if dtype is not None:
                    tensor = tensor.to(dtype=dtype)
                obj.requires_grad_(False)
                obj.set_(tensor)
            except Exception as e:
                print(f"Failed to load key '{key}' in safetensors file: {e}")
                raise e


CLIPG_CONFIG = {
    "hidden_act": "gelu",
    "hidden_size": 1280,
    "intermediate_size": 5120,
    "num_attention_heads": 20,
    "num_hidden_layers": 32
}


class ClipG:
    def __init__(self):
        with safe_open("models/clip_g.safetensors", framework="pt", device="cpu") as f:
            self.model = SDXLClipG(CLIPG_CONFIG, device="cpu", dtype=torch.float32)  # float32
            load_into(f, self.model.transformer, "", "cpu", torch.float32)  # float32


CLIPL_CONFIG = {
    "hidden_act": "quick_gelu",
    "hidden_size": 768,
    "intermediate_size": 3072,
    "num_attention_heads": 12,
    "num_hidden_layers": 12
}


class ClipL:
    def __init__(self):
        with safe_open("models/clip_l.safetensors", framework="pt", device="cpu") as f:
            self.model = SDClipModel(layer="hidden", layer_idx=-2, device="cpu", dtype=torch.float32, layer_norm_hidden_state=False, return_projected_pooled=False, textmodel_json_config=CLIPL_CONFIG)  # float32
            load_into(f, self.model.transformer, "", "cpu", torch.float32)  # float32


T5_CONFIG = {
    "d_ff": 10240,
    "d_model": 4096,
    "num_heads": 64,
    "num_layers": 24,
    "vocab_size": 32128
}


class SD3:
    def __init__(self, model, shift):
        with safe_open(model, framework="pt", device="cpu") as f:
            self.model = BaseModel(shift=shift, file=f, prefix="model.diffusion_model.", device="cpu", dtype=torch.float16).eval()  # float16
            load_into(f, self.model, "model.", "cpu", torch.float16)  # float16


class VAE:
    def __init__(self, model):
        with safe_open(model, framework="pt", device="cpu") as f:
            self.model = SDVAE(device="cpu", dtype=torch.float16).eval().cpu()  # float16
            prefix = ""
            if any(k.startswith("first_stage_model.") for k in f.keys()):
                prefix = "first_stage_model."
            load_into(f, self.model, prefix, "cpu", torch.float16)  # float16


#################################################################################################
### Main inference logic
#################################################################################################

# Note: Sigma shift value, publicly released models use 3.0
SHIFT = 3.0
# Naturally, adjust to the width/height of the model you have
WIDTH = 512
HEIGHT = 512
# Most models prefer the range of 4-5, but still work well around 7
CFG_SCALE = 5
# Random seed
SEED = 1
# Actual model file path
MODEL = "models/sd3_medium.safetensors"
# VAE model file path, or set None to use the same model file
VAEFile = None  #
# Optional input image file path
INIT_IMAGE = None
# If init_image is given, this is the percentage of denoising steps to run (1.0 = full denoise, 0.0 = no denoise at all)
DENOISE = 0.6

# Randomly selects the N prompts from the Flickr8k_text dataset.
TASK_NUM = 10  # The number (N) of tasks
file_txt_path = "dataset/Flickr8k_text.txt"
with open(file_txt_path) as file:
    text = file.read().splitlines()
text = np.array(text)
text_size = len(text)
tasks = np.random.randint(0, text_size, size=[TASK_NUM])
PROMPTS = text[tasks]

# Different models want different step counts but most will be good at 50, albeit that's slow to run
# sd3_medium is quite decent at 28 steps
STEPS = np.random.randint(10, 20, size=[TASK_NUM])  # Setting By default. The larger, the better quality, the higher delay.

class SD3Inferencer:
    def load(self, model=MODEL, vae=VAEFile, shift=SHIFT):
        print("Loading tokenizers...")
        # NOTE: if you need a reference impl for a high performance CLIP tokenizer instead of just using the HF transformers one,
        # check https://github.com/Stability-AI/StableSwarmUI/blob/master/src/Utils/CliplikeTokenizer.cs
        self.tokenizer = SD3Tokenizer()
        print("Loading OpenCLIP bigG...")
        self.clip_g = ClipG()
        print("Loading OpenAI CLIP L...")
        self.clip_l = ClipL()
        print("Loading SD3 model...")
        self.sd3 = SD3(model, shift)
        print("Loading VAE model...")
        self.vae = VAE(vae or model)
        # self.vae = VAE(model)
        print("Models loaded.")

    def get_empty_latent(self, width, height):
        print("Prep an empty latent...")
        return torch.ones(1, 16, height // 8, width // 8, device="cpu") * 0.0609

    def get_sigmas(self, sampling, steps):
        start = sampling.timestep(sampling.sigma_max)
        end = sampling.timestep(sampling.sigma_min)
        timesteps = torch.linspace(start, end, steps)
        sigs = []
        for x in range(len(timesteps)):
            ts = timesteps[x]
            sigs.append(sampling.sigma(ts))
        sigs += [0.0]
        return torch.FloatTensor(sigs)

    def get_noise(self, seed, latent):
        generator = torch.manual_seed(seed)
        print(f"dtype = {latent.dtype}, layout = {latent.layout}, device = {latent.device}")
        return torch.randn(latent.size(), dtype=torch.float32, layout=latent.layout, generator=generator, device="cpu").to(latent.dtype) # float32

    def get_cond(self, prompt):
        print("Encode prompt...")
        tokens = self.tokenizer.tokenize_with_weights(prompt)
        l_out, l_pooled = self.clip_l.model.encode_token_weights(tokens["l"])
        g_out, g_pooled = self.clip_g.model.encode_token_weights(tokens["g"])
        lg_out = torch.cat([l_out, g_out], dim=-1)
        lg_out = torch.nn.functional.pad(lg_out, (0, 4096 - lg_out.shape[-1]))
        return lg_out, torch.cat((l_pooled, g_pooled), dim=-1)
        
    def max_denoise(self, sigmas):
        max_sigma = float(self.sd3.model.model_sampling.sigma_max)
        sigma = float(sigmas[0])
        return math.isclose(max_sigma, sigma, rel_tol=1e-05) or sigma > max_sigma

    def fix_cond(self, cond):
        cond, pooled = (cond[0].half().cuda(), cond[1].half().cuda())
        return { "c_crossattn": cond, "y": pooled }

    def do_sampling(self, latent, seed, conditioning, neg_cond, steps, cfg_scale, denoise=1.0) -> torch.Tensor:
        print("Sampling...")
        latent = latent.half().cuda()
        self.sd3.model = self.sd3.model.cuda()
        noise = self.get_noise(seed, latent).cuda()
        sigmas = self.get_sigmas(self.sd3.model.model_sampling, steps).cuda()
        sigmas = sigmas[int(steps * (1 - denoise)):]
        conditioning = self.fix_cond(conditioning)
        neg_cond = self.fix_cond(neg_cond)
        extra_args = { "cond": conditioning, "uncond": neg_cond, "cond_scale": cfg_scale }
        noise_scaled = self.sd3.model.model_sampling.noise_scaling(sigmas[0], noise, latent, self.max_denoise(sigmas))
        latent = sample_euler(CFGDenoiser(self.sd3.model), noise_scaled, sigmas, extra_args=extra_args)
        latent = SD3LatentFormat().process_out(latent)
        self.sd3.model = self.sd3.model.cpu()
        print("Sampling done")
        return latent

    def vae_encode(self, image) -> torch.Tensor:
        print("Encoding image to latent...")
        image = image.convert("RGB")
        image_np = np.array(image).astype(np.float32) / 255.0  # float32
        image_np = np.moveaxis(image_np, 2, 0)
        batch_images = np.expand_dims(image_np, axis=0).repeat(1, axis=0)
        image_torch = torch.from_numpy(batch_images)
        image_torch = 2.0 * image_torch - 1.0
        image_torch = image_torch.cuda()
        self.vae.model = self.vae.model.cuda()
        latent = self.vae.model.encode(image_torch).cpu()
        self.vae.model = self.vae.model.cpu()
        print("Encoded")
        return latent

    def vae_decode(self, latent) -> Image.Image:
        print("Decoding latent to image...")
        latent = latent.cuda()
        self.vae.model = self.vae.model.cuda()
        image = self.vae.model.decode(latent)
        image = image.float()
        self.vae.model = self.vae.model.cpu()
        image = torch.clamp((image + 1.0) / 2.0, min=0.0, max=1.0)[0]
        decoded_np = 255.0 * np.moveaxis(image.cpu().numpy(), 0, 2)
        decoded_np = decoded_np.astype(np.uint8)
        out_image = Image.fromarray(decoded_np)
        print("Decoded")
        return out_image

    def gen_image(self, prompt, width=WIDTH, height=HEIGHT, steps=STEPS[0], cfg_scale=CFG_SCALE, seed=SEED, init_image=INIT_IMAGE, denoise=DENOISE):
        latent = self.get_empty_latent(width, height)
        if init_image:
            image_data = Image.open(init_image)
            image_data = image_data.resize((width, height), Image.LANCZOS)
            latent = self.vae_encode(image_data)
            latent = SD3LatentFormat().process_in(latent)
        conditioning = self.get_cond(prompt)
        neg_cond = self.get_cond("")
        sampled_latent = self.do_sampling(latent, seed, conditioning, neg_cond, steps, cfg_scale, denoise if init_image else 1.0)
        image = self.vae_decode(sampled_latent)  # Decode the latent to an image
        return image

@torch.no_grad()
def main(prompts=PROMPTS, width=WIDTH, height=HEIGHT, steps=STEPS, cfg_scale=CFG_SCALE, shift=SHIFT, model=MODEL, vae=VAEFile, seed=SEED, init_image=INIT_IMAGE, denoise=DENOISE):
    inferencer = SD3Inferencer()
    inferencer.load(model, vae, shift)
    start_time = time.time()
    for i in range(TASK_NUM):
        image = inferencer.gen_image(prompts[i], width, height, steps[i], cfg_scale, seed, init_image, denoise)
        image.save("../results/images/output" + str(i) + ".png")
    end_time = time.time()
    aver_time = (end_time-start_time) / TASK_NUM
    print("Done.")
    print(f"Average generation time: {aver_time} seconds.")

fire.Fire(main)
