# 机器学习相关算法实现
---
### DBSCAN
- python3.5
- 基于密度的聚类方法
- 左图：300  数据点，eps=2， minPts=1，time=15s
- 右图：2000数据点，eps=10，minPts=3，time=1015s
- 时间效率略差

<img src="https://github.com/xiaonimo/MachineLearning/blob/master/DBSCAN/result.PNG" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/DBSCAN/result2.PNG" width=128 height=128/>

---
### Kmeans
- C++
- 基于距离的聚类算法
- 二分Kmeans算法

---
### KNN
- C++
- 基于距离的分类过程

---
### Logistic Regression
- Python
- 逻辑回归解决二分类问题

<img src="https://github.com/xiaonimo/MachineLearning/blob/master/LogisticRegression/result.PNG" width=128 height=128/>
---

### Puzzle8
- C++
- 八数码问题

---

### DecisionTree
- C++
- 基于gian（信息增益）的决策树
- 基于Gini（基尼指数）的决策树
- 前剪枝、后剪枝
- 交叉验证

---

### MNIST
- python3.5
- 基于scikit-learn库，PCA+SVM
- 20000个训练样本，120维
- 准确率>98%

---

### BMP
- C++
- 实现对BMP图像的读取，存储，以及一些处理
- 读取，保存，直方图均衡化，直方图规定化，中值滤波，均值滤波，色域转换，OSTU，halftone，噪声模型，DCT，IDCT等

<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena.BMP" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_he2.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_halftone.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_otsu.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_hm.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_laplacian.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/salt.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/salt_medianfilter.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_dct.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_idct.bmp" width=128 height=128/>
<img src="https://github.com/xiaonimo/MachineLearning/blob/master/BMP/pics/lena_gray_dft.bmp" width=128 height=128/>
