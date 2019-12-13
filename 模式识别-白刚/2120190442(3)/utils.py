import numpy as np

# 似然函数
likelihood = lambda x, mean, cov: 1. / ((2*np.pi)**(len(cov)/2) * np.linalg.det(cov)**0.5)\
* np.exp(-0.5 * np.reshape(x - mean, (1, -1)) @ np.linalg.inv(cov) @ np.reshape(x - mean, (-1, 1)))

def manhattan_distance(x, mean, cov):
    a = np.reshape(x - mean, (1, -1))
    b = np.reshape(x - mean, (-1, 1))
    c = np.linalg.inv(cov)
    return np.sqrt(a @ c @ b)

# 贝叶斯分类器
class Classifier(object):

    def __init__(self, config, clf='map'):
        self.config = config
        self.clf = clf
    
    def __call__(self, data):
        clf = getattr(self, '_{}_test'.format(self.clf))
        return clf(data)
    
    def _map_test(self, data):
        '''
            最大后验概率规则
        '''
        mean = self.config['mean']
        cov = self.config['cov']
        prior = self.config['prior']
        predictions = []

        for i in range(len(data)):
            probs = np.array([
                likelihood(data[i], mean[j], cov[j]) * prior[j]
                for j in range(len(prior))
            ]).flatten()

            predictions.append(np.argmax(probs))

        return predictions