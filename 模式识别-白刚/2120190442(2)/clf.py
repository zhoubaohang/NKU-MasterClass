import numpy as np
from utils import likelihood

class Classifier(object):

    def __init__(self, config, clf='map'):
        self.config = config
        self.clf = clf
    
    def __call__(self, data):
        clf = getattr(self, '_{}_test'.format(self.clf))
        return clf(data)
    
    def _likelihood_test(self, data):
        '''
            似然率测试规则
        '''
        mean = self.config['mean']
        cov = self.config['cov']
        prior = self.config['prior']
        labels = []
        predictions = []

        for k, v in data.items():
            for i in range(len(v)):
                labels.append(k)

                likelihoods = np.array([
                    likelihood(v[i], mean[j], cov[j])
                    for j in range(len(prior))
                ]).flatten()

                cache = np.zeros((len(prior), len(likelihoods)))

                for j in range(len(prior)):
                    for h in range(len(likelihoods)):
                        if (likelihoods[h] / likelihoods[j]) > (prior[j] / prior[h]):
                                cache[j, h] = 1.
                
                predictions.append(np.argmax(np.sum(cache, axis=0)))

        return np.mean(np.array(labels) == np.array(predictions))

    def _euc_test(self, data):
        '''
            最短欧式距离规则
        '''
        mean = self.config['mean']
        labels = []
        predictions = []

        for k, v in data.items():
            for i in range(len(v)):
                labels.append(k)

                distances = np.linalg.norm(v[i] - mean, axis=1)
                predictions.append(np.argmin(distances))
        
        return np.mean(np.array(labels) == np.array(predictions))

    def _map_test(self, data):
        '''
            最大后验概率规则
        '''
        mean = self.config['mean']
        cov = self.config['cov']
        prior = self.config['prior']
        labels = []
        predictions = []

        for k, v in data.items():
            for i in range(len(v)):
                labels.append(k)

                probs = np.array([
                    likelihood(v[i], mean[j], cov[j]) * prior[j]
                    for j in range(len(prior))
                ]).flatten()

                predictions.append(np.argmax(probs))

        return np.mean(np.array(labels) == np.array(predictions))
    
    def _byr_test(self, data):
        '''
            贝叶斯风险规则
        '''
        mean = self.config['mean']
        cov = self.config['cov']
        prior = self.config['prior']
        labels = []
        predictions = []
        C = np.array(
            [[0, 2, 3],
            [1, 0, 2.5],
            [1, 1, 0]]
        )

        for k, v in data.items():
            for i in range(len(v)):
                labels.append(k)

                probs = np.array([
                    likelihood(v[i], mean[j], cov[j]) * prior[j]
                    for j in range(len(prior))
                ]).flatten()

                risks = np.sum(probs*C, axis=1).tolist()
                predictions.append(np.argmin(risks))

        return np.mean(np.array(labels) == np.array(predictions))