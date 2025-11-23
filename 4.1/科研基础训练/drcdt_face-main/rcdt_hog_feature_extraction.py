import numpy as np
import re
import cv2
from torch.nn import functional as F
from einops import rearrange
import torch


def get_img_gradient(img, ksize):
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=ksize) # dx
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=ksize) # dy
    return sobelx, sobely


def extract_drcdt_local(dx_blk, dy_blk, proj_n):
    THETA = proj_n
    if THETA == 2:
        THETA_range = np.asarray([30, 120])

    if THETA == 3:
        THETA_range = np.asarray([0, 45, 135])

    if THETA == 4:
        THETA_range = np.asarray([0, 45, 135, 179])

    if THETA != 2 and THETA != 3 and THETA != 4:
        THETA_range = np.linspace(0, 179, THETA) #[0, 90, 135] #180  # [0, 45, 90, 135]

    dxdy_blk_arry = np.vstack([dx_blk, dy_blk]) # shape: 2 * cellsize^2 (2*256)
    drcdt_local = np.zeros([dx_blk.shape[0],THETA])
    theta_cnt = 0
    for theta_temp in THETA_range:       
        w_local = np.asarray([np.cos((theta_temp*np.pi)/180), np.sin((theta_temp*np.pi)/180)]) # cos(theta), sin(theta) = 0: [1, 0], 180: [-1, 0]
        w_local_dot_dxdy_blk_arry = np.dot(w_local.reshape([1,2]), dxdy_blk_arry)
        drcdt_local[:,theta_cnt] = np.sort(w_local_dot_dxdy_blk_arry) # sort ascending: x_0<x_1<...<x_n
        theta_cnt = theta_cnt +1 

    return drcdt_local


class Patchify(object):
    # adopt from vision transformer
    # credit: https://github.com/PyTorchLightning/lightning-bolts/blob/master/pl_bolts/transforms/self_supervised/ssl_transforms.py
    def __init__(self, patch_size, overlap_size):
        self.patch_size = patch_size
        self.overlap_size = self.patch_size - overlap_size
    def __call__(self, x):
        x = x.unsqueeze(0)
        b, c, h, w = x.size()
        # patch up the images
        # (b, c, h, w) -> (b, c*patch_size, L)
        x = F.unfold(x, kernel_size=self.patch_size, stride=self.overlap_size)
        # (b, c*patch_size, L) -> (b, nb_patches, width, height)
        x = x.transpose(2, 1).contiguous().view(b, -1, self.patch_size, self.patch_size)
        # reshape to have (b x patches, c, h, w)
        x = x.view(-1, c, self.patch_size, self.patch_size)
        x = x.squeeze(0)
        return x


def get_drcdt_feature_sets(img_path_nm, step_size, overlap_size, proj_n):

    my_img = img_path_nm.copy()
    #my_img_dy, my_img_dx = get_img_gauss_gradient(my_img, sigma=5, K=5)
    my_img_dx_np, my_img_dy_np = get_img_gradient(my_img, ksize=3) # image range [0, 255], gaussian kernel size is 5
    
    my_img_dx_tensor = torch.unsqueeze(torch.from_numpy(my_img_dx_np), 0)
    my_img_dy_tensor = torch.unsqueeze(torch.from_numpy(my_img_dy_np), 0)  

    getPatch = Patchify(patch_size = step_size, overlap_size = overlap_size)
    my_patch_dx = torch.squeeze(getPatch(my_img_dx_tensor))
    my_patch_dy = torch.squeeze(getPatch(my_img_dy_tensor))

    my_patch_dxx = my_patch_dx.numpy()
    my_patch_dyy = my_patch_dy.numpy()
    
    #pdb.set_trace()
    my_img_dx_blocks = rearrange(my_patch_dxx, '(b1 b2) h w -> b1 b2 h w', b1=int(np.sqrt(my_patch_dxx.shape[0])))
    my_img_dy_blocks = rearrange(my_patch_dyy, '(b1 b2) h w -> b1 b2 h w', b1=int(np.sqrt(my_patch_dyy.shape[0])))

    
    img_path0_myrcdt_local_img_test = extract_drcdt_local(my_img_dx_blocks[0,0,...].ravel(), my_img_dy_blocks[0,0,...].ravel(), proj_n)
    
    myrcdt_local_h = img_path0_myrcdt_local_img_test.shape[0]
    myrcdt_local_w = img_path0_myrcdt_local_img_test.shape[1]
    
    row_STEPS = my_img_dx_blocks.shape[0]
    col_STEPS = my_img_dx_blocks.shape[1]

    big_Radon_img = np.zeros([row_STEPS, col_STEPS, myrcdt_local_h, myrcdt_local_w])
    for n_row in range(row_STEPS):
        for n_col in range(col_STEPS):
            dx_temp = my_img_dx_blocks[n_row,n_col,...].ravel()/255. # scale the gradient value from [0 255] to [0, 1]
            dy_temp = my_img_dy_blocks[n_row,n_col,...].ravel()/255.
            dxdy_myrcdt_local = extract_drcdt_local(dx_temp, dy_temp, proj_n)
            big_Radon_img[n_row,n_col,:,:] = dxdy_myrcdt_local

    return my_img, my_img_dx_np, my_img_dy_np, big_Radon_img  