import numpy as np
import re
import os
from os import listdir
from os.path import isfile, join, isdir
import math
from skimage import io
from skimage.color import rgb2gray
from skimage.transform import resize

import pdb


def atoi(text):
    return int(text) if text.isdigit() else text


def natural_keys(text):
    return [atoi(c) for c in re.split('(\d+)', text)]


def read_img(read_img_path, read_img_w, read_img_h):
    face_gray = resize(rgb2gray(io.imread(read_img_path)), (read_img_w, read_img_h), anti_aliasing=True)
    #face_gray = np.log(face_gray)
    return face_gray

def get_folder_from_path(path_to_get_folder):
    full_dir_to_get_folder = [dir_nm_to_get_folder for dir_nm_to_get_folder in listdir(path_to_get_folder) if isdir(join(path_to_get_folder, dir_nm_to_get_folder))]
    full_dir_to_get_folder.sort(key=natural_keys)
    return full_dir_to_get_folder


def get_files_from_folder(path_to_folder, file_ext):
    files_in_current_directory = []
    for file_path_temp in listdir(path_to_folder):
        if file_path_temp.endswith(file_ext):
            files_in_current_directory.append(file_path_temp)

    files_in_current_directory.sort(key=natural_keys)

    if len(files_in_current_directory) == 0:
        print('No ' + file_ext + ' files is found!')
        raise

    return files_in_current_directory


def check_substring(substring, string_being_checked):
    # input: 
    #       type(substring): string
    #       type(string_being_checked): string
    # output: True or False
    ## modifed from https://github.com/610265158/Peppa_Pig_Face_Engine
    ## https://github.com/jrosebr1/imutils/blob/master/imutils/face_utils/helpers.py
    if substring in string_being_checked:
        return True
    else:
        return False

def get_substring_between_markers(string_bet_mrk, before_mrk, after_mrk):
    # input:
    #       string_bet_mrk: string being checked, type() is string
    #       before_mrk: string that begins with, type() is string
    #       after_mrk: string that end with, type() is string
    # return:
    #       substring_bet: substring in between, type() is string

    index_sta = string_bet_mrk.find(before_mrk)
    if index_sta < 0:
        print('cannot find the {} in {}'.format(before_mrk, string_bet_mrk))
        raise
    else:
        index_sta = index_sta + len(before_mrk)

    index_end = string_bet_mrk.find(after_mrk)
    
    if index_end < 0:
        print('cannot find the {} in {}'.format(index_end, string_bet_mrk))
        raise
        
    substring_bet = string_bet_mrk[index_sta:index_end]

    return substring_bet

def get_each_subject_yale_dataset(path_to_yale_subject, img_files_list_yale_subject, img_ext, LOG_IMG):
    # Input:
    # path_to_yale_subject: path to subject
    # path_to_yale_subject: img file lists
    # Return: 
    # X_yale_img_list: read imgs
    # Y_yale_img_list: read imgs label
    # log image
    # s = c*log(r+1), r \in [0, 255], c = 255/log(1+255): gain 

    X_yale_img_list = []
    Y_yale_img_list = []
    c_gain = 255/np.log(1+255)
    #https://pythontic.com/image-processing/pillow/logarithmic%20transformation
    #azi_list = []
    #ele_list = []
    #group_list_for_subset = []
    for each_file_nm in img_files_list_yale_subject:
        if check_substring('Ambient', each_file_nm): # check ambient image first
            #print(each_file_nm)
            pass
        else:
            # get azimuth and elevation in degree
            each_azi = get_substring_between_markers(each_file_nm, 'P00A', 'E')
            each_ele = get_substring_between_markers(each_file_nm, 'E', img_ext)

            if each_azi[0] == '+':
                cur_azi = int(each_azi[1:4])
            else:
                cur_azi = int(each_azi[1:4])*-1
            #azi_list.append(cur_azi)

            if each_ele[0] == '+':
                cur_ele = int(each_ele[1:4])
            else:
                cur_ele = int(each_ele[1:4])*-1
            #ele_list.append(cur_ele)

            if LOG_IMG:
                face_cur = c_gain * np.log(1+255*resize(rgb2gray(io.imread(path_to_yale_subject  + each_file_nm)), (64, 64), anti_aliasing=True))
                #https://pythontic.com/image-processing/pillow/logarithmic%20transformation
            else:
                face_cur = resize(rgb2gray(io.imread(path_to_yale_subject  + each_file_nm)), (64, 64), anti_aliasing=True)

            if np.abs(cur_azi) <=10 and np.abs(cur_ele) <= 20 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(0)

            if np.abs(cur_azi) >10 and np.abs(cur_azi)<=25 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(1)

            if np.abs(cur_azi) >25 and np.abs(cur_azi)<=60 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(2)

            if (np.abs(cur_azi)==0 and np.abs(cur_ele) == 35) or (np.abs(cur_azi)==0 and np.abs(cur_ele) == 45):
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(2)
            
            if np.abs(cur_azi) >60 and np.abs(cur_azi)<=95 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(3)
            
            if np.abs(cur_azi)>95 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(4)

            if np.abs(cur_ele) == 90  and np.abs(cur_azi)==0 :
                X_yale_img_list.append(face_cur.ravel())
                Y_yale_img_list.append(4)

    return np.asarray(X_yale_img_list), np.asarray(Y_yale_img_list)