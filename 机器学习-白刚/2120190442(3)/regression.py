import numpy as np
import matplotlib.pyplot as plt
from utils import mean_squared_error

class PolynomialRegression(object):
    '''
        一元多项式回归算法
    '''
    def __init__(self, degree=2.):
        self.params = None
        self.degree = degree
    
    def __create_polynomial(self, x):
        polynomial = np.ones_like(x)

        for i in range(1, self.degree+1):
            polynomial = np.hstack((polynomial, np.power(x, i)))
        
        return polynomial
    
    def fit(self, data, verbose=False):
        # 样本维度
        x = data[:, :-1].reshape((-1, 1))
        # 预测值维度
        y = data[:, -1].reshape((-1, 1))
        # 加入常数项
        c_x = self.__create_polynomial(x)

        # 求解正规方程
        w = np.linalg.inv(c_x.T @ c_x) @ c_x.T @ y

        self.params = w

        if verbose:
            # 训练误差
            pred = c_x @ self.params
            mse = mean_squared_error(pred, y)
            print(f"{self.__class__.__name__}\t训练误差:{mse}")
            plt.plot(x, pred, '-', label='least squares')
    
    def predict(self, x):
        x = x.reshape((-1, 1))
        # 加入常数项
        c_x = self.__create_polynomial(x)
        # 预测结果
        pred = c_x @ self.params
        return pred

class LeastSquareMethod(object):
    '''
        最小二乘法求解回归问题
    '''
    def __init__(self):
        self.params = None

    def fit(self, data, verbose=False):
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
            # 训练误差
            pred = c_x @ self.params
            mse = mean_squared_error(pred, y)
            print(f"{self.__class__.__name__}\t训练误差:{mse}")
            plt.plot(x, pred, '-', label='least squares')
    
    def predict(self, x):
        x = x.reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))
        # 预测结果
        pred = c_x @ self.params
        return pred

class RidgeRegression(object):
    '''
        岭回归算法
    '''
    def __init__(self, lamb=1.):
        self.lamb = lamb
        self.params = None
    
    def fit(self, data, verbose=False):
        # 样本维度
        x = data[:, :-1].reshape((-1, 1))
        # 预测值维度
        y = data[:, -1].reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))

        # 求解参数
        tmp = c_x.T @ c_x
        w = np.linalg.inv(self.lamb*np.eye(len(tmp)) + tmp) @ c_x.T @ y

        self.params = w


        if verbose:
            # 训练误差
            pred = c_x @ self.params
            mse = mean_squared_error(pred, y)
            print(f"{self.__class__.__name__}\t训练误差:{mse}")
            plt.plot(x, pred, '-', label='ridge regression')
    
    def predict(self, x):
        x = x.reshape((-1, 1))
        # 加入常数项
        c_x = np.hstack((np.ones_like(x), x))
        # 预测结果
        pred = c_x @ self.params
        return pred