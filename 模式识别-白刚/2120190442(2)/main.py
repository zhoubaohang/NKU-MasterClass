import numpy as np
import matplotlib.pyplot as plt
from utils import generate_dataset, plot_data
from clf import Classifier
from tqdm import trange
import argparse

# 脚本输入参数
parser = argparse.ArgumentParser()
parser.add_argument('-t', default='map', type=str,
            choices=['map', 'byr', 'likelihood', 'euc'],
            help='''模型测试规则,
                    map: 最大后验概率规则
                    byr: 贝叶斯风险规则
                    likelihood: 似然率测试规则
                    euc: 最短欧式距离规则''')
parser.add_argument('-s', default=100, type=int, help="模型训练次数")
parser.add_argument('-v', default=False, type=bool, help="是否展示数据散布图")
args = parser.parse_args()

# 数据属性包括：均值、协方差、先验概率
data1_config = {
    'size': 1000,
    'mean': np.array([[1,1],[4,4],[8,1]]),
    'cov': np.array([np.eye(2)*2 for _ in range(3)]),
    'prior': [1/3, 1/3, 1/3]
}

data2_config = {
    'size': 1000,
    'mean': np.array([[1,1],[4,4],[8,1]]),
    'cov': np.array([np.eye(2)*2 for _ in range(3)]),
    'prior': [0.6, 0.3, 0.1]
}
# 多次实验的错误率集合
errors = []


for i in trange(args.s, ascii=True):
    np.random.seed(i)

    # 生成数据
    data1 = generate_dataset(data1_config)
    data2 = generate_dataset(data2_config)
    # 画出数据分布图
    if args.v and i == 0:
        plot_data(data1)
        plot_data(data2)

    cache = []
    clf = Classifier(data1_config, args.t)
    cache.append(1. - clf(data1))

    clf = Classifier(data2_config, args.t)
    cache.append(1. - clf(data2))

    errors.append(cache)

mean = np.mean(errors, axis=0)
std = np.std(errors, axis=0)
for i in range(len(mean)):
    print("数据集合{}：{}({})".format(i+1, mean[i], std[i]))
    