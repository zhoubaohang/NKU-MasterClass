import numpy as np

class LeastSquareMethod(object):
    '''
        最小二乘线性分类方法
    '''
    def __init__(self):
        self.params = None
    
    def fit(self, X, y):
        # 统计类别
        classes = np.unique(y).tolist()
        m, _ = X.shape
        # 目标值矩阵
        T = np.zeros((m, len(classes)))
        for i in range(m):
            T[i, classes.index(y[i])] = 1.
        
        c_X = np.hstack((np.ones((m, 1)), X))
        # 求解参数
        w = np.linalg.pinv(c_X.T @ c_X) @ c_X.T @ T

        self.params = w
        self.labels = classes
        # 预测训练集误差
        pred = self.predict(X)
        acc = np.mean(pred == y)
        print(f"\n{self.__class__.__name__}\n训练错误率: {1-acc}")

    def predict(self, X):
        m, _ = X.shape
        X = np.hstack((np.ones((m, 1)), X))
        y = [
            self.labels[i]
            for i in np.argmax(X @ self.params, axis=-1)
        ]
        return y


class LinearDiscriminantAnalysis(object):
    '''
        线性判别分析方法
    '''
    def __init__(self):
        self.params = None
    
    def fit(self, X, y):
        _, n = X.shape
        # 全局均值
        mu_t = np.mean(X, axis=0)
        # 统计类别
        classes = np.unique(y).tolist()
        # 类内散度矩阵
        c_mu = {}
        Sw = np.zeros((n, n))
        for c in classes:
            index = np.where(y == c)[0]
            subData = X[index, :]
            # 类别均值向量
            mu = np.mean(subData, axis=0, keepdims=True)
            c_mu[c] = mu
            # 类别协方差矩阵
            Sw += (subData - mu).T @ (subData - mu)
        # 求解参数
        w = np.linalg.pinv(Sw) @ (c_mu[classes[1]] - c_mu[classes[0]]).T

        self.mu = mu_t
        self.params = w
        self.c_mu = c_mu
        
        # 预测训练误差
        pred = self.predict(X)
        acc = np.mean(pred == y)
        print(f"\n{self.__class__.__name__}\n训练错误率: {1-acc}")

    def predict(self, X):
        # 判别函数
        distance = (X - self.mu) @ self.params
        # 预测类别
        y = [
            list(self.c_mu.keys())[int(distance[i, 0] > 0)]
            for i in range(len(distance))
        ]
        return y


class GenerativeModel(object):
    '''
        概率生成式模型
    '''
    def __init__(self):
        self.params = None
    
    def fit(self, X, y):
        _, n = X.shape
        cache = {}
        # 统计类别
        classes = np.unique(y).tolist()
        for c in classes:
            index = np.where(y == c)[0]
            subData = X[index, :]
            tmp = {}
            # 类别均值向量
            mu = np.mean(subData, axis=0, keepdims=True)
            tmp['mean'] = mu
            # 类别协方差矩阵
            tmp['var'] = (subData - mu).T @ (subData - mu) / len(subData)
            # 先验概率
            tmp['prior'] = len(subData) / len(X)
            cache[c] = tmp
        # 类别加权协方差矩阵
        sigma = np.zeros((n, n))
        for _, v in cache.items():
            sigma += v['prior'] * v['var']
        # 类别1均值向量
        mu1 = cache[classes[0]]['mean'].T
        # 类别2均值向量
        mu2 = cache[classes[1]]['mean'].T
        # 类别1先验概率
        p1 = cache[classes[0]]['prior']
        # 类别2先验概率
        p2 = cache[classes[1]]['prior']
        # 求解参数
        w = np.linalg.pinv(sigma) @ (mu1 - mu2)
        w0 = -0.5 * mu1.T @ np.linalg.pinv(sigma) @ mu1\
             +0.5 * mu2.T @ np.linalg.pinv(sigma) @ mu2 + np.log(p1 / p2)

        self.labels = classes
        self.params = {'w':w, 'w0':w0}
        # 训练集测试误差
        pred = self.predict(X)
        acc = np.mean(pred == y)
        print(f"\n{self.__class__.__name__}\n训练错误率: {1-acc}")
    
    def predict(self, X):
        w = self.params['w']
        w0 = self.params['w0']
        # 后验概率
        prob = 1 / (1 + np.exp(- (X @ w + w0)))
        # 预测类别
        y = [
            self.labels[int(p < 0.5)]
            for p in prob
        ]
        return y