import numpy as np
import matplotlib.pyplot as plt

class LeastSquareMethod(object):
    '''
        最小二乘法求解回归问题
    '''
    def __init__(self):
        self.params = None

    def fit(self, data, verbose=True):
        # 样本维度
        x = data[:, :-1].reshape((-1, 1))
        # 预测值维度
        y = data[:, -1].reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))

        # 求解正规方程
        w = np.linalg.inv(c_x.T @ c_x) @ c_x.T @ y

        self.params = w

        if verbose:
            x_min = np.min(x) - 1.
            pred_y = c_x @ self.params
            plt.plot(x, pred_y, '-', label='least squares')
    
    def predict(self, data):
        # 样本维度
        x = data[:, :-1].reshape((-1, 1))
        # 预测值维度
        y = data[:, -1].reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))
        # 预测结果
        pred = c_x @ self.params
        return pred

class RobustnessLinearRegression(object):
    '''
        鲁棒线性回归算法
        采用Huber损失函数进行训练
    '''
    def __init__(self, sigma=1., threshold=1e-3):
        # 方差参数
        self.sigma = sigma
        # 迭代误差阈值
        self.threshold = threshold
    
    def fit(self, data, verbose=True):
        # 样本维度
        x = data[:, :-1].reshape((-1, 1))
        # 预测值维度
        y = data[:, -1].reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))

        w = np.random.normal(size=(c_x.shape[1], 1))
        threshold = 100.

        while threshold >= self.threshold:
            # 求得损失
            r = np.abs(c_x @ w - y)
            W = np.ones((c_x.shape[0], 1))
            W[r > self.sigma] = self.sigma / r[r > self.sigma]
            # 转换为对角矩阵
            W = np.eye(W.shape[0]) * W
            # 求解参数
            new_w = np.linalg.inv(c_x.T @ W @ c_x) @ c_x.T @ W @ y
            # 参数是否收敛
            threshold = np.sqrt(np.sum(np.square(w - new_w)))
            w = new_w
    
        self.params = w

        if verbose:
            pred_y = c_x @ self.params
            plt.plot(x, pred_y, '-', label='Huber')
    
    def predict(self, data):
        # 样本维度
        x = data[:, :-1].reshape((-1, 1))
        # 预测值维度
        y = data[:, -1].reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))
        # 预测结果
        pred = c_x @ self.params
        return pred