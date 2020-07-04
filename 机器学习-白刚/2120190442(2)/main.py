import numpy as np
import matplotlib.pyplot as plt
from regression import LeastSquareMethod
from regression import RobustnessLinearRegression
from utils import split_data, plot_data, cross_validation
np.random.seed(0)

# 生成数据
data_size = 100
x = np.random.rand(data_size).reshape((-1, 1))
noise = np.random.normal(0, np.sqrt(5), size=data_size).reshape((-1, 1))
y = 1.4 * x + 0.9 + noise

# 观察数据
plot_data(x, y)

# 划分数据
data = np.hstack((x, y))
train_data, test_data = split_data(data)
print(f"训练集样本数:{len(train_data)}\n测试集样本数:{len(test_data)}")

# 最小二乘法求解回归问题
lsm = LeastSquareMethod()
lsm.fit(train_data)
pred_y = lsm.predict(test_data)

# 对预测结果进行评价
mse = np.mean(np.square(pred_y.flatten() - test_data[:, -1]))
print(f"最小二乘法测试均平方误差:{mse}")

# 鲁棒线性回归算法
rlr = RobustnessLinearRegression()

# 余一交叉验证
sigmas = [1.5,2.5,2.6,2.7,2.8,2.94]
# 获得最优超参数
sigma = cross_validation(rlr, train_data, sigmas)

# 使用全部训练集训练模型
rlr = RobustnessLinearRegression(sigma=sigma)
rlr.fit(train_data)
pred_y = rlr.predict(test_data)

# 对预测结果进行评价
mse = np.mean(np.square(pred_y.flatten() - test_data[:, -1]))
print(f"鲁棒线性回归算法测试均平方误差:{mse}")

plt.legend()
plot_data(x, y, title='Result')