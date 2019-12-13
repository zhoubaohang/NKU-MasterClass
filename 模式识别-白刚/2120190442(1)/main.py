# -*- coding: utf-8 -*-
"""
Created on Thu Oct 9 2019
@author: 周宝航
"""

import numpy as np
from knn import KNN
from utils import split_dataset, plot_errorbar
import matplotlib.pyplot as plt
from tqdm import trange
import argparse

# 脚本输入参数
parser = argparse.ArgumentParser()
parser.add_argument('-p', default=2, type=int, help="闵可夫斯基距离p参数")
parser.add_argument('-s', default=100, type=int, help="模型训练次数")
args = parser.parse_args()

data = np.loadtxt('iris.txt', delimiter=',')

K = [i for i in range(1, 21)]
val_accs = []
test_accs = []

print("开始训练")
for i in trange(args.s, ascii=True):
    np.random.seed(i)
    train_data, val_data, _ = split_dataset(data)
    val_tmp = []

    for k in K:
        clf = KNN(p=args.p, k=k)

        clf(train_data[:, :-1], train_data[:, -1])

        _, val_acc = clf.predict((val_data[:, :-1], val_data[:, -1]))

        val_tmp.append(val_acc)
    
    val_accs.append(val_tmp)

plt.axis([0, max(K), 0.75, 1.1])
plt.xlabel('k')
plt.ylabel('accuracy')
val_mean, val_std = plot_errorbar(val_accs, label='validation')
plt.legend(loc='best')

idx = np.argmax(val_mean)
result = map(lambda a: '%.4f'%a, [val_mean[idx], val_std[idx]])
print("Best K={}: [validation] {}({})".format(K[idx], *result))
plt.scatter(K[idx], val_mean[idx], s=70, c='r', marker='x')

print("开始测试")
test_tmp = []
for i in trange(args.s, ascii=True):
    np.random.seed(i)
    train_data, val_data, test_data = split_dataset(data)
    train_data = np.vstack([train_data, val_data])
    
    clf = KNN(p=args.p, k=K[idx])
    clf(train_data[:, :-1], train_data[:, -1])
    _, test_acc = clf.predict((test_data[:, :-1], test_data[:, -1]))
    test_tmp.append(test_acc)

result = map(lambda a: '%.4f'%a, [np.mean(test_tmp), np.std(test_tmp)])
print("Test Result: {}({})".format(*result))

plt.show()