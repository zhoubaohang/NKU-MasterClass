import numpy as np
from tqdm import trange, tqdm
from scipy.misc import derivative

class MDN(object):

    def __init__(self, n_input=1, n_output=1,
                       n_hidden=24, n_components=3, alpha=1e-4):
        # 学习率
        self.alpha = alpha
        self.epsilon = 1e-9
        # 输入特征维数
        self.n_input = n_input
        # 输出目标维数
        self.n_output = n_output
        # 隐含层神经元数
        self.n_hidden = n_hidden
        # 输出组成成分数
        self.n_components = n_components
        # 权重
        self.w = [np.random.normal(scale=np.sqrt(0.01), size=(n_hidden, n_input)),
                  np.random.normal(scale=np.sqrt(0.01), size=((n_output+2)*n_components, n_hidden))]
        # 偏置
        self.b = [np.random.normal(scale=np.sqrt(0.01), size=(n_hidden, 1)),
                  np.random.normal(scale=np.sqrt(0.01), size=((n_output+2)*n_components, 1))]
    
    def __tanh(self, z, flag=False):
        '''
            隐含层激活函数tanh
        '''
        c = np.tanh(z)
        # 求导结果
        if flag:
            return 1 - np.power(c, 2)
        else:
            return c

    def __output_activation(self, z):
        '''
            输出层激活函数
        '''
        pi = z[:self.n_components]
        mu = z[self.n_components:2*self.n_components]
        sigma = z[2*self.n_components:]

        o_pi = np.exp(pi) / np.sum(np.exp(pi), axis=0)
        o_sigma = np.exp(sigma)

        return (o_pi, mu, o_sigma)
    
    def __normal_likelihood(self, t, mu, sigma):
        '''
            高斯似然函数
        '''
        result = - np.square(t - mu)\
                 / (2 * np.square(sigma))
        result = np.exp(result) / sigma / np.sqrt(2*np.pi)
        return result
    
    def __loss_function(self, pi, N):
        '''
            损失函数
        '''
        loss = np.mean(- np.log(np.sum(pi * N, axis=0)))
        return loss
    
    def __forward(self, x):
        '''
            前向传播算法
        '''
        hiddens = []
        activations = [x]

        h = self.w[0] @ activations[-1] + self.b[0]
        hiddens.append(h)
        a = self.__tanh(h)
        activations.append(a)

        h = self.w[1] @ activations[-1] + self.b[1]
        hiddens.append(h)
        a = self.__output_activation(h)
        activations.append(a)
        
        return (hiddens, activations)
    
    def __backward(self, cache, t):
        '''
            反向传播算法
        '''
        hiddens, activations = cache
        nable_w = [np.zeros(w.shape) for w in self.w]
        nable_b = [np.zeros(b.shape) for b in self.b]

        pi, mu, sigma = activations[-1]
        N = self.__normal_likelihood(t, mu, sigma)
        loss = self.__loss_function(pi, N)
        # 计算后验概率
        gamma = (pi * N) / np.sum(pi * N, axis=0)
        # 误差函数对分布参数的偏导数计算
        gd_pi = pi - gamma
        gd_mu = gamma * (mu - t) / np.square(sigma)
        gd_sigma = - gamma * (np.square(t - mu)
                             / np.power(sigma, 2) - 1)
        # 反向求解各层梯度
        delta = np.vstack((gd_pi, gd_mu, gd_sigma))
        nable_b[-1] = np.mean(delta, axis=-1, keepdims=True)
        nable_w[-1] = delta @ activations[-2].T

        delta = self.w[-1].T @ delta * self.__tanh(hiddens[-2], flag=True)
        nable_b[-2] = np.mean(delta, axis=-1, keepdims=True)
        nable_w[-2] = delta @ activations[-3].T

        # 利用梯度更新每层参数
        self.b = [b - self.alpha*delta_b for b, delta_b in zip(self.b, nable_b)]
        self.w = [w - self.alpha*delta_w for w, delta_w in zip(self.w, nable_w)]

        return loss
        
    def fit(self, x, t, epoch=10, verbose=False):
        '''
            训练函数 随机梯度下降法
        '''
        m = x.shape[1]
        for i in range(epoch):
            loss = 0
            for j in trange(m, ascii=True):
                # 选取一个样本训练
                d1 = x[:, j].reshape((-1, 1))
                d2 = t[:, j].reshape((-1, 1))
                tmp = self.__backward(self.__forward(d1), d2)
                loss += tmp
            if verbose:
                tqdm.write(f"{loss / m}")
    
    def predict(self, x):
        '''
            预测函数
        '''
        _, activations = self.__forward(x)
        pi, mu, _ = activations[-1]
        pred = []

        for i, idx in enumerate(np.argmax(pi, axis=0)):
            pred.append(mu[idx, i])

        return pi, mu, pred