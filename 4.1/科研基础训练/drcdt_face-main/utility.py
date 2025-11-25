import numpy as np
import re
import os
from os import listdir
from os.path import isfile, join, isdir
import math
from skimage import io
from skimage.color import rgb2gray
from skimage.transform import resize

import pdb  # Python调试器


def atoi(text):
    """
    将字符串转换为整数，如果可能的话
    
    Args:
        text: 输入字符串
        
    Returns:
        如果字符串是数字则返回整数，否则返回原字符串
    """
    return int(text) if text.isdigit() else text


def natural_keys(text):
    """
    为自然排序生成键值
    
    Args:
        text: 输入字符串
        
    Returns:
        分割后的字符串和数字列表，用于自然排序
    """
    return [atoi(c) for c in re.split('(\d+)', text)]


def read_img(read_img_path, read_img_w, read_img_h):
    """
    读取并预处理图像
    
    Args:
        read_img_path: 图像文件路径
        read_img_w: 目标宽度
        read_img_h: 目标高度
        
    Returns:
        处理后的灰度图像
    """
    # 读取图像，转换为灰度，调整大小，并应用抗锯齿
    face_gray = resize(rgb2gray(io.imread(read_img_path)), (read_img_w, read_img_h), anti_aliasing=True)
    #face_gray = np.log(face_gray)  # 注释掉的对数变换
    return face_gray


def get_folder_from_path(path_to_get_folder):
    """
    获取路径下的所有文件夹
    
    Args:
        path_to_get_folder: 目标路径
        
    Returns:
        排序后的文件夹名称列表
    """
    # 获取路径下所有目录，排除文件
    full_dir_to_get_folder = [dir_nm_to_get_folder for dir_nm_to_get_folder in listdir(path_to_get_folder) 
                             if isdir(join(path_to_get_folder, dir_nm_to_get_folder))]
    # 使用自然排序对文件夹名称排序
    full_dir_to_get_folder.sort(key=natural_keys)
    return full_dir_to_get_folder


def get_files_from_folder(path_to_folder, file_ext):
    """
    获取文件夹中指定扩展名的所有文件
    
    Args:
        path_to_folder: 文件夹路径
        file_ext: 文件扩展名
        
    Returns:
        排序后的文件名称列表
        
    Raises:
        Exception: 如果没有找到指定扩展名的文件
    """
    files_in_current_directory = []
    # 遍历文件夹中的所有文件
    for file_path_temp in listdir(path_to_folder):
        if file_path_temp.endswith(file_ext):
            files_in_current_directory.append(file_path_temp)

    # 使用自然排序对文件名称排序
    files_in_current_directory.sort(key=natural_keys)

    # 检查是否找到文件
    if len(files_in_current_directory) == 0:
        print('No ' + file_ext + ' files is found!')
        raise

    return files_in_current_directory


def check_substring(substring, string_being_checked):
    """
    检查字符串是否包含子字符串
    
    Args:
        substring: 要查找的子字符串
        string_being_checked: 被检查的字符串
        
    Returns:
        True如果包含子字符串，否则False
    """
    ## 修改自 https://github.com/610265158/Peppa_Pig_Face_Engine
    ## https://github.com/jrosebr1/imutils/blob/master/imutils/face_utils/helpers.py
    if substring in string_being_checked:
        return True
    else:
        return False


def get_substring_between_markers(string_bet_mrk, before_mrk, after_mrk):
    """
    获取两个标记之间的子字符串
    
    Args:
        string_bet_mrk: 被检查的字符串
        before_mrk: 起始标记
        after_mrk: 结束标记
        
    Returns:
        两个标记之间的子字符串
        
    Raises:
        Exception: 如果找不到标记
    """
    # 查找起始标记位置
    index_sta = string_bet_mrk.find(before_mrk)
    if index_sta < 0:
        print('cannot find the {} in {}'.format(before_mrk, string_bet_mrk))
        raise
    else:
        index_sta = index_sta + len(before_mrk)  # 移动到起始标记之后

    # 查找结束标记位置
    index_end = string_bet_mrk.find(after_mrk)
    
    if index_end < 0:
        print('cannot find the {} in {}'.format(index_end, string_bet_mrk))
        raise
        
    # 提取两个标记之间的子字符串
    substring_bet = string_bet_mrk[index_sta:index_end]

    return substring_bet


def get_each_subject_yale_dataset(path_to_yale_subject, img_files_list_yale_subject, img_ext, LOG_IMG):
    """
    处理耶鲁数据集中的每个受试者图像
    
    Args:
        path_to_yale_subject: 受试者图像路径
        img_files_list_yale_subject: 图像文件列表
        img_ext: 图像扩展名
        LOG_IMG: 是否应用对数变换
        
    Returns:
        X_yale_img_list: 图像数据数组
        Y_yale_img_list: 图像标签数组
    """
    # 初始化返回列表
    X_yale_img_list = []
    Y_yale_img_list = []
    
    # 对数变换的增益系数
    c_gain = 255/np.log(1+255)
    #https://pythontic.com/image-processing/pillow/logarithmic%20transformation
    
    # 遍历每个图像文件
    for each_file_nm in img_files_list_yale_subject:
        # 跳过环境光照图像
        if check_substring('Ambient', each_file_nm):
            #print(each_file_nm)
            pass
        else:
            # 从文件名中提取方位角和仰角信息
            each_azi = get_substring_between_markers(each_file_nm, 'P00A', 'E')
            each_ele = get_substring_between_markers(each_file_nm, 'E', img_ext)

            # 解析方位角值（处理正负号）
            if each_azi[0] == '+':
                cur_azi = int(each_azi[1:4])
            else:
                cur_azi = int(each_azi[1:4])*-1

            # 解析仰角值（处理正负号）
            if each_ele[0] == '+':
                cur_ele = int(each_ele[1:4])
            else:
                cur_ele = int(each_ele[1:4])*-1

            # 读取并预处理图像
            if LOG_IMG:
                # 应用对数变换
                face_cur = c_gain * np.log(1+255*resize(rgb2gray(io.imread(path_to_yale_subject  + each_file_nm)), (64, 64), anti_aliasing=True))
                #https://pythontic.com/image-processing/pillow/logarithmic%20transformation
            else:
                # 普通灰度变换
                face_cur = resize(rgb2gray(io.imread(path_to_yale_subject  + each_file_nm)), (64, 64), anti_aliasing=True)

            # 根据方位角和仰角分配标签
            # 标签0：小角度（正面）
            if np.abs(cur_azi) <=10 and np.abs(cur_ele) <= 20 :
                X_yale_img_list.append(face_cur.ravel())  # 将图像展平为一维数组
                Y_yale_img_list.append(0)

            # 标签1：中等角度
            if np.abs(cur_azi) >10 and np.abs(cur_azi)<=25 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(1)

            # 标签2：较大角度
            if np.abs(cur_azi) >25 and np.abs(cur_azi)<=60 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(2)

            # 特殊情况：特定仰角组合
            if (np.abs(cur_azi)==0 and np.abs(cur_ele) == 35) or (np.abs(cur_azi)==0 and np.abs(cur_ele) == 45):
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(2)
            
            # 标签3：大角度
            if np.abs(cur_azi) >60 and np.abs(cur_azi)<=95 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(3)
            
            # 标签4：极大角度或顶部视角
            if np.abs(cur_azi)>95 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(4)

            # 顶部视角特殊情况
            if np.abs(cur_ele) == 90  and np.abs(cur_azi)==0 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(4)

    # 返回numpy数组格式的结果
    return np.asarray(X_yale_img_list), np.asarray(Y_yale_img_list)