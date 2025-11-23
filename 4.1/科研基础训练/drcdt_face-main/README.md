# Local Sliced-Wasserstein Feature Sets for Illumination-invariant Face Recognition

This repository contains the Python implementation of Dicrete RCDT transform and its application on illumination-invariant face recognition (paper link: https://arxiv.org/abs/2202.10642). It is also a part of PyTransKit (link: https://github.com/rohdelab/PyTransKit).


# DRCDT-NS based Illumination-invariant Face Recognition Demo

1. Download the The Extended Yale Face Database B from: http://vision.ucsd.edu/~leekc/ExtYaleDatabase/ExtYaleB.html (Please use the "Cropped Images")
2. Extract the folder to the local directory, the directory should have the structure as shown below:
```
Local directory
└───CroppedYale
|    └───yaleB01
│        │   yaleB02_P00A+000E+00.pgm
│        │   yaleB02_P00A+000E+20.pgm
│        │   ...
|    └───yaleB02
|    └───yaleB03
│    |...
│   NS_classifier_patch.py
│   rcdt_hog_feature_extraction.py
|   utility.py
|   drcdt_yale_notebook.ipynb
|   README.md
```
3. Run the Jupyter notebook: drcdt_yale_notebook.ipynb

# Dependencies 

Please see the "requirements.txt".

# Citation

Please cite the following publication when publishing findings that benefit from the codes provided here.

@article{zhuang2022local,  
  title={Local Sliced-Wasserstein Feature Sets for Illumination-invariant Face Recognition},  
  author={Zhuang, Yan and Li, Shiying and Yin, Xuwang and Rubaiyat, Abu Hasnat Mohammad and Rohde, Gustavo K and others},  
  journal={arXiv preprint arXiv:2202.10642},  
  year={2022}  
}
