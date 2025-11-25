import numpy as np
import re
import cv2
from torch.nn import functional as F
from einops import rearrange
import torch


def get_img_gradient(img, ksize):
    """
    计算图像的梯度（Sobel算子）
    
    Args:
        img: 输入图像
        ksize: Sobel核大小
        
    Returns:
        sobelx: x方向梯度
        sobely: y方向梯度
    """
    # 使用Sobel算子计算x方向梯度（水平方向）
    sobelx = cv2.Sobel(img, cv2.CV_64F, 1, 0, ksize=ksize) # dx
    # 使用Sobel算子计算y方向梯度（垂直方向）
    sobely = cv2.Sobel(img, cv2.CV_64F, 0, 1, ksize=ksize) # dy
    return sobelx, sobely


def extract_drcdt_local(dx_blk, dy_blk, proj_n):
    """
    提取局部方向Radon特征
    
    Args:
        dx_blk: x方向梯度块
        dy_blk: y方向梯度块
        proj_n: 投影方向数量
        
    Returns:
        drcdt_local: 局部方向Radon特征
    """
    THETA = proj_n
    # 根据投影方向数量设置不同的角度范围
    if THETA == 2:
        THETA_range = np.asarray([30, 120])  # 2个方向：30度和120度

    if THETA == 3:
        THETA_range = np.asarray([0, 45, 135])  # 3个方向：0度、45度、135度

    if THETA == 4:
        THETA_range = np.asarray([0, 45, 135, 179])  # 4个方向：0度、45度、135度、179度

    # 如果投影方向数不是2、3、4，则均匀分布在0-179度之间
    if THETA != 2 and THETA != 3 and THETA != 4:
        THETA_range = np.linspace(0, 179, THETA) #[0, 90, 135] #180  # [0, 45, 90, 135]

    # 将x和y方向梯度堆叠成一个数组，形状为2×块大小^2
    dxdy_blk_arry = np.vstack([dx_blk, dy_blk]) # shape: 2 * cellsize^2 (2*256)
    # 初始化局部Radon特征数组
    drcdt_local = np.zeros([dx_blk.shape[0],THETA])
    theta_cnt = 0
    # 遍历每个投影角度
    for theta_temp in THETA_range:       
        # 计算当前角度的投影向量 [cos(theta), sin(theta)]
        w_local = np.asarray([np.cos((theta_temp*np.pi)/180), np.sin((theta_temp*np.pi)/180)]) # cos(theta), sin(theta) = 0: [1, 0], 180: [-1, 0]
        # 计算梯度向量在投影向量上的投影
        w_local_dot_dxdy_blk_arry = np.dot(w_local.reshape([1,2]), dxdy_blk_arry)
        # 对投影值进行排序（升序）
        drcdt_local[:,theta_cnt] = np.sort(w_local_dot_dxdy_blk_arry) # sort ascending: x_0<x_1<...<x_n
        theta_cnt = theta_cnt +1 

    return drcdt_local


class Patchify(object):
    """
    图像分块处理类
    
    Args:
        patch_size: 块大小
        overlap_size: 重叠大小
    """
    # adopt from vision transformer
    # credit: https://github.com/PyTorchLightning/lightning-bolts/blob/master/pl_bolts/transforms/self_supervised/ssl_transforms.py
    def __init__(self, patch_size, overlap_size):
        # 设置块大小
        self.patch_size = patch_size
        # 计算重叠步长（块大小减去重叠大小）
        self.overlap_size = self.patch_size - overlap_size
        
    def __call__(self, x):
        """
        对输入张量进行分块处理
        
        Args:
            x: 输入张量
            
        Returns:
            分块后的张量
        """
        # 增加批次维度
        x = x.unsqueeze(0)
        # 获取张量的形状：批次大小、通道数、高度、宽度
        b, c, h, w = x.size()
        # patch up the images
        # 使用unfold操作将图像分块：(b, c, h, w) -> (b, c*patch_size, L)
        x = F.unfold(x, kernel_size=self.patch_size, stride=self.overlap_size)
        # 转置并重塑张量形状：(b, c*patch_size, L) -> (b, nb_patches, width, height)
        x = x.transpose(2, 1).contiguous().view(b, -1, self.patch_size, self.patch_size)
        # 重塑为(b×patches, c, h, w)的形状
        x = x.view(-1, c, self.patch_size, self.patch_size)
        # 移除批次维度
        x = x.squeeze(0)
        return x


def get_drcdt_feature_sets(img_path_nm, step_size, overlap_size, proj_n):
    """
    获取图像的DRCDT特征集
    
    Args:
        img_path_nm: 输入图像
        step_size: 步长大小
        overlap_size: 重叠大小
        proj_n: 投影方向数量
        
    Returns:
        my_img: 原始图像
        my_img_dx_np: x方向梯度
        my_img_dy_np: y方向梯度
        big_Radon_img: Radon特征图像
    """
    # 复制输入图像
    my_img = img_path_nm.copy()
    #my_img_dy, my_img_dx = get_img_gauss_gradient(my_img, sigma=5, K=5)
    # 计算图像的x和y方向梯度，图像范围[0, 255]，Sobel核大小为3
    my_img_dx_np, my_img_dy_np = get_img_gradient(my_img, ksize=3) # image range [0, 255], gaussian kernel size is 5
    
    # 将numpy数组转换为PyTorch张量，并增加批次维度
    my_img_dx_tensor = torch.unsqueeze(torch.from_numpy(my_img_dx_np), 0)
    my_img_dy_tensor = torch.unsqueeze(torch.from_numpy(my_img_dy_np), 0)  

    # 创建分块处理器
    getPatch = Patchify(patch_size = step_size, overlap_size = overlap_size)
    # 对梯度图像进行分块处理
    my_patch_dx = torch.squeeze(getPatch(my_img_dx_tensor))
    my_patch_dy = torch.squeeze(getPatch(my_img_dy_tensor))

    # 将PyTorch张量转换回numpy数组
    my_patch_dxx = my_patch_dx.numpy()
    my_patch_dyy = my_patch_dy.numpy()
    
    #pdb.set_trace()
    # 使用einops重新排列数组维度，将一维块数组重塑为二维网格
    my_img_dx_blocks = rearrange(my_patch_dxx, '(b1 b2) h w -> b1 b2 h w', b1=int(np.sqrt(my_patch_dxx.shape[0])))
    my_img_dy_blocks = rearrange(my_patch_dyy, '(b1 b2) h w -> b1 b2 h w', b1=int(np.sqrt(my_patch_dyy.shape[0])))

    # 提取第一个块的Radon特征用于获取输出形状
    img_path0_myrcdt_local_img_test = extract_drcdt_local(my_img_dx_blocks[0,0,...].ravel(), my_img_dy_blocks[0,0,...].ravel(), proj_n)
    
    # 获取Radon特征的高度和宽度
    myrcdt_local_h = img_path0_myrcdt_local_img_test.shape[0]
    myrcdt_local_w = img_path0_myrcdt_local_img_test.shape[1]
    
    # 获取块网格的行数和列数
    row_STEPS = my_img_dx_blocks.shape[0]
    col_STEPS = my_img_dx_blocks.shape[1]

    # 初始化大的Radon特征图像
    big_Radon_img = np.zeros([row_STEPS, col_STEPS, myrcdt_local_h, myrcdt_local_w])
    # 遍历每个块
    for n_row in range(row_STEPS):
        for n_col in range(col_STEPS):
            # 获取当前块的x和y方向梯度，并将值从[0,255]缩放到[0,1]
            dx_temp = my_img_dx_blocks[n_row,n_col,...].ravel()/255. # scale the gradient value from [0 255] to [0, 1]
            dy_temp = my_img_dy_blocks[n_row,n_col,...].ravel()/255.
            # 提取当前块的Radon特征
            dxdy_myrcdt_local = extract_drcdt_local(dx_temp, dy_temp, proj_n)
            # 将特征存储到大的Radon图像中
            big_Radon_img[n_row,n_col,:,:] = dxdy_myrcdt_local

    return my_img, my_img_dx_np, my_img_dy_np, big_Radon_img