import numpy as np
from utils import Classifier, manhattan_distance
import matplotlib.pyplot as plt

# 数据属性包括：均值、协方差、先验概率
data_config = {
    'size': 1000,
    'mean': np.array([[0.1,0.1],[2.1,1.9],[-1.5,2.0]]),
    'cov': np.array([[[1.2,0.4],[0.4,1.8]] for _ in range(3)]),
    'prior': [1/3, 1/3, 1/3]
}
# 测试样本
test_sample = np.array([[1.6,1.5]])
# 分类器
clf = Classifier(data_config)
# 打印预测结果
for i, pred in enumerate(clf(test_sample)):
    print(f"测试样本{i+1}\n所属类别信息:\nmean:{data_config['mean'][pred]}\
            \ncov:\n{data_config['cov'][pred]}")

#########################################################################
# 采样点数目
n_p = 800
# 模式特征矢量
center = np.array([2.1,1.9])
# 协方差矩阵
cov = np.array([[1.2,0.4],[0.4,1.8]])
# 维度x采样
x = np.linspace(0.9, 3.5, n_p)
# 维度y采样
y = np.linspace(0.4, 3.5, n_p)
# 组合得到样本点
x, y = np.meshgrid(x,y)
target = []
# 马氏距离要求
distance = 1

# 计算不同样本点到模式特征矢量的马氏距离
print('绘制中......')
fxy = np.zeros([n_p, n_p])
for i in range(n_p):
    for j in range(n_p):
        fxy[i][j] = manhattan_distance(np.array([x[i, j], y[i, j]]), center, cov)[0]

# 画出轨迹
plt.scatter(center[0], center[1])
contour = plt.contour(x, y, fxy, [distance])
plt.clabel(contour, inline=True)
plt.show()