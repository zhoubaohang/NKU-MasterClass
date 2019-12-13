# -*- coding: utf-8 -*-
"""
Created on Thu Oct 9 2019
@author: 周宝航
"""

import numpy as np
from collections import Counter
from utils import minkowski_distance

class KNN(object):

    def __init__(self, p=2, k=5):
        self.n_neighbors = k
        self.p = p
    
    def __calc_distance(self, a, b):
        return minkowski_distance(a, b, self.p)

    def __call__(self, x, y):
        self.train_x = x
        self.train_y = y
    
    def predict(self, data):
        x, y = data
        pred = []

        for i in range(len(x)):
            distances = self.__calc_distance(x[i], self.train_x)
            k_neighbors = np.argsort(distances)[:self.n_neighbors]
            counter = Counter([ele for ele in self.train_y[k_neighbors]])
            
            tag, cnt = -1, -1
            for k, v in counter.items():
                if v > cnt:
                    tag = k
                    cnt = v
            
            pred.append(tag)
        
        acc = np.mean(np.array(pred) == y)

        return (pred, acc)
