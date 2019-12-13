# -*- coding: utf-8 -*-
"""
Created on Thu Oct 9 2019
@author: 周宝航
"""

import numpy as np
from numpy.linalg import norm
import matplotlib.pyplot as plt

# 闵可夫斯基距离
minkowski_distance = lambda a, b, p: pow(np.sum(pow(abs(a - b), p), axis=-1), 1./p)

# 分割数据集
def split_dataset(data, **kwargs):
    train = kwargs.get('train', 0.8)
    val = kwargs.get('val', 0.1)
    test = kwargs.get('test', 0.1)
    assert train+val+test == 1., '划分比例之和不为1'

    split = {
        'train': train,
        'val': val,
        'test': test
    }
    split_data = {
        'train': None,
        'val': None,
        'test': None
    }

    idx = [i for i in range(len(data))]
    np.random.shuffle(idx)

    start_idx = 0
    for k, v in split.items():
        end_idx = int(start_idx + len(data)*v)
        split_data[k] = data[idx[start_idx:end_idx]]
        start_idx = end_idx
    
    return split_data.values()

# 画出带有方差的折线图
def plot_errorbar(data, label='default'):
    if type(data) != np.array:
        data = np.array(data)
    
    mean = np.mean(data, axis=0)
    std = np.std(data, axis=0)

    idx = [i+1 for i in range(len(mean))]
    plt.errorbar(idx, mean, std, label=label, fmt='-o')
    
    return (mean, std)