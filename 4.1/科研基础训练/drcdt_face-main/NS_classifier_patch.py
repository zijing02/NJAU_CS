import numpy as np
import pdb



class NS:
    def __init__(self, num_classes, num_proj, n_var): #num_proj
        """
        Parameters
        ----------
        num_classes : integer, total number of classes
        thetas : array-like, angles in degrees for taking radon projections
            default = [0,180) with increment of 4 degrees.
        rm_edge : boolean flag; IF TRUE the first and last points of RCDTs will be removed
            default = False
        """
        self.num_classes = num_classes
        if num_proj == 2:
            self.thetas = np.asarray([30, 120])

        if num_proj == 3:
            self.thetas = np.asarray([0, 45, 135])

        if num_proj == 4:
            self.thetas = np.asarray([0, 45, 135, 179])

        if num_proj != 2 and num_proj != 3 and num_proj != 4:
            self.thetas = np.linspace(0, 179, num_proj) #180

        self.subspaces = []
        self.len_subspace = 0
        self.n_var = n_var

    def fit(self, Xtrain, Ytrain):
        """Fit linear model.
        Parameters
        ----------
        Xtrain : array-like, shape (n_samples, n_rows, n_columns)
            Image data for training.
        Ytrain : ndarray of shape (n_samples,)
            Labels of the training images.
        no_deform_model : boolean flag; IF TRUE, no deformation model will be added
            default = False.
        """
        min_samples = min([(Ytrain==i).sum() for i in range(self.num_classes)])
        for class_idx in range(self.num_classes):
            class_data = Xtrain[Ytrain == class_idx]
            np.random.seed(123)
            idx = np.random.permutation(class_data.shape[0])
            class_data = class_data[idx][:min_samples]
            class_data_trans = self.add_trans_samples(class_data)

            flat = class_data_trans.reshape(class_data_trans.shape[0], -1)

            u, s, vh = np.linalg.svd(flat,full_matrices=False)
            
            cum_s = np.cumsum(s)
            cum_s = cum_s/np.max(cum_s)

            max_basis = (np.where(cum_s>=self.n_var)[0])[0] + 1

            if max_basis > self.len_subspace:
                self.len_subspace = max_basis
            
            basis = vh[:flat.shape[0]]
            self.subspaces.append(basis)


    def predict(self, Xtest):
        """Predict using the linear model
        Parameters
        ----------
        Xtest : array-like, shape (n_samples, n_rows, n_columns)
            Image data for testing.
        use_gpu: boolean flag; IF TRUE, use gpu for calculations
            default = False.
            
        Returns
        -------
        ndarray of shape (n_samples,)
           Predicted target values per element in Xtest.
        """
    
        
        # vectorize RCDT matrix
        X = Xtest.reshape([Xtest.shape[0], -1])
        
        D = []
        for class_idx in range(self.num_classes):
            basis = self.subspaces[class_idx]
            basis = basis[:self.len_subspace,:]
            
            proj = X @ basis.T  # (n_samples, n_basis)
            projR = proj @ basis  # (n_samples, n_features)
            D.append(np.linalg.norm(projR - X, axis=1))

        D = np.stack(D, axis=0)
        preds = np.argmin(D, axis=0)
        return D, preds
    
    def score(self, X_test, y_test):
        _, preds = self.predict(X_test)
        return (preds == y_test).mean()


    def add_trans_samples(self, rcdt_features):
        # rcdt_features: (n_samples, proj_len * num_angles)
        # deformation vectors for  translation
        v1, v2 = np.cos(self.thetas*np.pi/180), np.sin(self.thetas*np.pi/180)
        proj_len = int(rcdt_features.shape[1]/len(self.thetas))
        v1 = np.repeat(v1[np.newaxis], proj_len, axis=0) # v1 shape is (proj_len, num_angles)
        v2 = np.repeat(v2[np.newaxis], proj_len, axis=0)
        return np.concatenate([rcdt_features, v1.ravel()[np.newaxis], v2.ravel()[np.newaxis]])