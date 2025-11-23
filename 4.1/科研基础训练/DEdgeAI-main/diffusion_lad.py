import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F

from helpers import (
    cosine_beta_schedule,
    linear_beta_schedule,
    vp_beta_schedule,
    extract_into_tensor,
    Losses
)


class Diffusion(nn.Module):
    def __init__(self, state_dim, action_dim, model, beta_schedule='linear', denoising_steps=5,
                 loss_type='l1', clip_denoised=False, predict_epsilon=True):
        super(Diffusion, self).__init__()

        self.state_dim = state_dim
        self.action_dim = action_dim
        self.model = model

        if beta_schedule == 'linear':
            betas = linear_beta_schedule(denoising_steps)
        elif beta_schedule == 'cosine':
            betas = cosine_beta_schedule(denoising_steps)
        elif beta_schedule == 'vp':
            betas = vp_beta_schedule(denoising_steps)

        alphas = 1. - betas
        alphas_cumprod = torch.cumprod(alphas, axis=0)
        alphas_cumprod_prev = torch.cat([torch.ones(1), alphas_cumprod[:-1]])

        self.n_timesteps = int(denoising_steps)
        self.clip_denoised = clip_denoised
        self.predict_epsilon = predict_epsilon

        self.register_buffer('betas', betas)
        self.register_buffer('alphas_cumprod', alphas_cumprod)
        self.register_buffer('alphas_cumprod_prev', alphas_cumprod_prev)

        # calculations for diffusion q(x_t | x_{t-1}) and others
        self.register_buffer('sqrt_alphas_cumprod', torch.sqrt(alphas_cumprod))
        self.register_buffer('sqrt_one_minus_alphas_cumprod', torch.sqrt(1. - alphas_cumprod))
        self.register_buffer('log_one_minus_alphas_cumprod', torch.log(1. - alphas_cumprod))
        self.register_buffer('sqrt_recip_alphas_cumprod', torch.sqrt(1. / alphas_cumprod))
        self.register_buffer('sqrt_recipm1_alphas_cumprod', torch.sqrt(1. / alphas_cumprod - 1))

        # calculations for posterior q(x_{t-1} | x_t, x_0)
        posterior_variance = betas * (1. - alphas_cumprod_prev) / (1. - alphas_cumprod)
        self.register_buffer('posterior_variance', posterior_variance)

        # log calculation clipped because the posterior variance
        # is 0 at the beginning of the diffusion chain
        self.register_buffer('posterior_log_variance_clipped',
                             torch.log(torch.clamp(posterior_variance, min=1e-20)))
        self.register_buffer('posterior_mean_coef1',
                             betas * np.sqrt(alphas_cumprod_prev) / (1. - alphas_cumprod))
        self.register_buffer('posterior_mean_coef2',
                             (1. - alphas_cumprod_prev) * np.sqrt(alphas) / (1. - alphas_cumprod))

        self.loss_fn = Losses[loss_type]()

    # def predict_start_from_noise(self, x_t, t, noise):
    #     """
    #         if self.predict_epsilon, model output is (scaled) noise;
    #         otherwise, model predicts x0 directly
    #     """
    #     if self.predict_epsilon:
    #         return (
    #                 extract_into_tensor(self.sqrt_recip_alphas_cumprod, t, x_t.shape) * x_t -
    #                 extract_into_tensor(self.sqrt_recipm1_alphas_cumprod, t, x_t.shape) * noise
    #         )
    #     else:
    #         return noise

    def predict_start_from_noise(self, x_t, t, noise):
        """
            if self.predict_epsilon, model output is (scaled) noise;
            otherwise, model predicts x0 directly
        """
        return (
                extract_into_tensor(self.sqrt_recip_alphas_cumprod, t, x_t.shape) * x_t -
                extract_into_tensor(self.sqrt_recipm1_alphas_cumprod, t, x_t.shape) * noise
        )

    def predict_start_from_t_v(self, x_t, t, noise):
        """
            if self.predict_epsilon, model output is (scaled) noise;
            otherwise, model predicts x0 directly
        """
        if self.predict_epsilon:
            return (
                    extract_into_tensor(self.sqrt_recip_alphas_cumprod, t, x_t.shape) * x_t -
                    extract_into_tensor(self.sqrt_recipm1_alphas_cumprod, t, x_t.shape) * noise
            )
        else:
            return noise


    def q_posterior(self, x_start, x_t, t):
        posterior_mean = (
                extract_into_tensor(self.posterior_mean_coef1, t, x_t.shape) * x_start +
                extract_into_tensor(self.posterior_mean_coef2, t, x_t.shape) * x_t
        )
        posterior_variance = extract_into_tensor(self.posterior_variance, t, x_t.shape)
        posterior_log_variance_clipped = extract_into_tensor(self.posterior_log_variance_clipped, t, x_t.shape)
        return posterior_mean, posterior_variance, posterior_log_variance_clipped

    def p_mean_variance(self, x, t, s):
        x_recon = self.predict_start_from_noise(x, t=t, noise=self.model(x, t, s))
        if self.clip_denoised:
            x_recon.clamp_(-1, 1)
        model_mean, posterior_variance, posterior_log_variance = self.q_posterior(x_start=x_recon, x_t=x, t=t)
        return model_mean, posterior_variance, posterior_log_variance

    def p_sample(self, x, t, s):
        b, *_, device = *x.shape, x.device
        model_mean, _, model_log_variance = self.p_mean_variance(x=x, t=t, s=s)
        probs = torch.randn_like(x)
        # no noise when t == 0
        nonzero_mask = (1 - (t == 0).float()).reshape(b, *((1,) * (len(x.shape) - 1)))
        return model_mean + nonzero_mask * (0.5 * model_log_variance).exp() * probs

    # def p_sample_loop(self, state, shape):
    #     device = self.betas.device
    #     batch_size = shape[0]
    #     x = torch.randn(shape, device=device)
    #     for i in reversed(range(0, self.n_timesteps)):
    #         timesteps = torch.full((batch_size,), i, device=device, dtype=torch.long)
    #         x = self.p_sample(x, timesteps, state)
    #     return x

    def p_sample_loop(self, state, latent_action_probs):
        device = self.betas.device
        batch_size = state.shape[0]
        x = latent_action_probs
        for i in reversed(range(0, self.n_timesteps)):
            timesteps = torch.full((batch_size,), i, device=device, dtype=torch.long)
            x = self.p_sample(x, timesteps, state)
        return x

    # def sample(self, state, *args, **kwargs):
    #     batch_size = state.shape[0]
    #     shape = (batch_size, self.action_dim)
    #     action = self.p_sample_loop(state, shape, *args, **kwargs)
    #     return F.softmax(action, dim=-1)

    def sample(self, state, latent_action_probs, *args, **kwargs):
        action = self.p_sample_loop(state, latent_action_probs, *args, **kwargs)
        return F.softmax(action, dim=-1)

    def q_sample(self, x_start, t, noise=None):
        if noise is None:
            noise = torch.randn_like(x_start)

        sample = (
                extract_into_tensor(self.sqrt_alphas_cumprod, t, x_start.shape) * x_start +
                extract_into_tensor(self.sqrt_one_minus_alphas_cumprod, t, x_start.shape) * noise
        )
        return sample

    def p_losses(self, x_start, state, t, weights=1.0):
        noise = torch.randn_like(x_start)
        x_noisy = self.q_sample(x_start=x_start, t=t, noise=noise)
        x_recon = self.model(x_noisy, t, state)
        assert noise.shape == x_recon.shape
        if self.predict_epsilon:
            return self.loss_fn(x_recon, noise, weights)
        else:
            return self.loss_fn(x_recon, x_start, weights)

    def loss(self, x, state, weights=1.0):
        batch_size = len(x)
        t = torch.randint(0, self.n_timesteps, (batch_size,), device=x.device).long()
        return self.p_losses(x, state, t, weights)

    def forward(self, state, latent_action_probs, *args, **kwargs):
        return self.sample(state, latent_action_probs, *args, **kwargs)
