import numpy as np
import pdb



class NS:
    def __init__(self, num_classes, num_proj, n_var): #num_proj
        """
        邻域子空间分类器
        
        Parameters
        ----------
        num_classes : integer, 总的类别数量
        num_proj : integer, 投影方向数量
        n_var : float, 方差阈值用于选择基向量数量
        """
        # 设置类别数量
        self.num_classes = num_classes
        
        # 根据投影方向数量设置不同的角度范围
        if num_proj == 2:
            self.thetas = np.asarray([30, 120])  # 2个投影方向：30度和120度

        if num_proj == 3:
            self.thetas = np.asarray([0, 45, 135])  # 3个投影方向：0度、45度、135度

        if num_proj == 4:
            self.thetas = np.asarray([0, 45, 135, 179])  # 4个投影方向：0度、45度、135度、179度

        # 如果投影方向数不是2、3、4，则在0-179度之间均匀分布
        if num_proj != 2 and num_proj != 3 and num_proj != 4:
            self.thetas = np.linspace(0, 179, num_proj) #180

        # 初始化子空间列表
        self.subspaces = []
        # 初始化子空间长度
        self.len_subspace = 0
        # 设置方差阈值
        self.n_var = n_var

    def fit(self, Xtrain, Ytrain):
        """训练邻域子空间模型
        
        Parameters
        ----------
        Xtrain : array-like, shape (n_samples, n_rows, n_columns)
            训练图像数据
        Ytrain : ndarray of shape (n_samples,)
            训练图像的标签
        """
        # 计算每个类别的最小样本数（平衡采样）
        min_samples = min([(Ytrain==i).sum() for i in range(self.num_classes)])
        
        # 对每个类别进行处理
        for class_idx in range(self.num_classes):
            # 获取当前类别的数据
            class_data = Xtrain[Ytrain == class_idx]
            
            # 设置随机种子并打乱数据顺序
            np.random.seed(123)
            idx = np.random.permutation(class_data.shape[0])
            # 选择最小样本数量的数据（平衡采样）
            class_data = class_data[idx][:min_samples]
            
            # 添加平移样本（数据增强）
            class_data_trans = self.add_trans_samples(class_data)

            # 将数据展平为二维矩阵
            flat = class_data_trans.reshape(class_data_trans.shape[0], -1)

            # 对数据进行奇异值分解（SVD）
            u, s, vh = np.linalg.svd(flat,full_matrices=False)
            
            # 计算累积方差解释比例
            cum_s = np.cumsum(s)
            cum_s = cum_s/np.max(cum_s)

            # 根据方差阈值确定基向量数量
            max_basis = (np.where(cum_s>=self.n_var)[0])[0] + 1

            # 更新最大子空间长度
            if max_basis > self.len_subspace:
                self.len_subspace = max_basis
            
            # 获取基向量
            basis = vh[:flat.shape[0]]
            # 将当前类别的子空间基向量保存到列表中
            self.subspaces.append(basis)


    def predict(self, Xtest):
        """使用线性模型进行预测
        
        Parameters
        ----------
        Xtest : array-like, shape (n_samples, n_rows, n_columns)
            测试图像数据
            
        Returns
        -------
        D : ndarray of shape (num_classes, n_samples)
            每个样本到每个类别子空间的距离
        preds : ndarray of shape (n_samples,)
            每个样本的预测类别
        """
        # 将测试数据展平为二维矩阵
        X = Xtest.reshape([Xtest.shape[0], -1])
        
        # 初始化距离列表
        D = []
        
        # 对每个类别计算距离
        for class_idx in range(self.num_classes):
            # 获取当前类别的基向量
            basis = self.subspaces[class_idx]
            # 截取到最大子空间长度
            basis = basis[:self.len_subspace,:]
            
            # 计算测试数据在子空间上的投影
            proj = X @ basis.T  # (n_samples, n_basis)
            # 重构数据
            projR = proj @ basis  # (n_samples, n_features)
            # 计算重构误差（到子空间的距离）
            D.append(np.linalg.norm(projR - X, axis=1))

        # 将距离列表转换为数组
        D = np.stack(D, axis=0)
        # 找到最小距离对应的类别（预测结果）
        preds = np.argmin(D, axis=0)
        return D, preds
    
    def score(self, X_test, y_test):
        """计算模型准确率
        
        Parameters
        ----------
        X_test : array-like, 测试数据
        y_test : ndarray, 测试标签
            
        Returns
        -------
        float : 模型准确率
        """
        # 预测并计算准确率
        _, preds = self.predict(X_test)
        return (preds == y_test).mean()


    def add_trans_samples(self, rcdt_features):
        """添加平移样本（数据增强）
        
        Parameters
        ----------
        rcdt_features : array-like, shape (n_samples, proj_len * num_angles)
            RCDT特征矩阵
            
        Returns
        -------
        ndarray : 增强后的特征矩阵（包含原始特征和平移特征）
        """
        # 计算平移向量的x和y分量
        v1, v2 = np.cos(self.thetas*np.pi/180), np.sin(self.thetas*np.pi/180)
        
        # 计算每个投影方向的长度
        proj_len = int(rcdt_features.shape[1]/len(self.thetas))
        
        # 重复平移向量以匹配投影长度
        v1 = np.repeat(v1[np.newaxis], proj_len, axis=0) # v1 shape is (proj_len, num_angles)
        v2 = np.repeat(v2[np.newaxis], proj_len, axis=0)
        
        # 将原始特征与平移特征拼接
        return np.concatenate([rcdt_features, v1.ravel()[np.newaxis], v2.ravel()[np.newaxis]])