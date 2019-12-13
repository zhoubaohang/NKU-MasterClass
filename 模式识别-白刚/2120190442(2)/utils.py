import numpy as np
import matplotlib.pyplot as plt

# 生成符合多元正态分布的数据
generate_data = lambda mean, cov, size: np.random.multivariate_normal(mean, cov, size)
# 似然函数
likelihood = lambda x, mean, cov: 1. / ((2*np.pi)**(len(cov)/2) * np.linalg.det(cov)**0.5)\
* np.exp(-0.5 * np.reshape(x - mean, (1, -1)) @ np.linalg.inv(cov) @ np.reshape(x - mean, (-1, 1)))

# 根据条件生成数据
def generate_dataset(config):
    size = config['size']
    mean = config['mean']
    cov = config['cov']
    prior = config['prior']

    data = {}

    for i, p in enumerate(prior):
        s = int(size*p)
        d = generate_data(mean[i], cov[i], s)
        data[i] = d
    
    return data

# 画出数据的散点图
def plot_data(data):

    for k, v in data.items():
        label = 'Class %d' % k
        plt.scatter(v[:, 0], v[:, 1], label=label, s=5)
    
    plt.legend(loc='best')
    plt.show()