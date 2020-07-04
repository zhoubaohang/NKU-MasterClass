import numpy as np
from classification import (
    GenerativeModel,
    LeastSquareMethod,
    LinearDiscriminantAnalysis
)

# 加载数据集
data = np.loadtxt('Ionosphere+Dataset.csv', delimiter=',', dtype=str)
train_data = data[:200]
test_data = data[200:]
# 划分为训练集和测试集
train_x, train_y = train_data[:, :-1].astype(np.float64), train_data[:, -1]
test_x, test_y = test_data[:, :-1].astype(np.float64), test_data[:, -1]

# 最小二乘法
lsm = LeastSquareMethod()
lsm.fit(train_x, train_y)
pred = lsm.predict(test_x)
acc = np.mean(pred == test_y)
print(f"测试误差: {1-acc}")

# 线性判别分析
lda = LinearDiscriminantAnalysis()
lda.fit(train_x, train_y)
pred = lda.predict(test_x)
acc = np.mean(pred == test_y)
print(f"测试误差: {1-acc}")

# 生成式模型
gm = GenerativeModel()
gm.fit(train_x, train_y)
pred = gm.predict(test_x)
acc = np.mean(pred == test_y)
print(f"测试误差: {1-acc}")