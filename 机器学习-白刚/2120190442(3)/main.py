import numpy as np
import matplotlib.pyplot as plt
from utils import cross_validation, mean_squared_error
from regression import (
    PolynomialRegression,
    LeastSquareMethod,
    RidgeRegression
)
np.random.seed(1)

# 加载训练数据
train_data = np.loadtxt('Dataset(5).csv', delimiter=',', skiprows=1)[:, 1:]

# 最小二乘法拟合
lsm = LeastSquareMethod()
lsm.fit(train_data)

# 生成测试样本
n_test_data = 5
test_data = np.random.uniform(0, 2, size=n_test_data).reshape((-1, 1))
noise = np.random.normal(scale=np.sqrt(5), size=n_test_data).reshape((-1, 1))
pred = lsm.predict(test_data)
test_data = np.hstack((test_data, pred + noise))

# 绘制训练样本和测试样本
plt.scatter(train_data[:,0], train_data[:,1], label='train')
plt.scatter(test_data[:,0], test_data[:,1], label='test')
plt.title('Data')
plt.legend()
plt.show()

# 最小二乘法测试
lsm.fit(train_data, verbose=True)
pred = lsm.predict(test_data[:, :-1])
mse = mean_squared_error(test_data[:, -1], pred)
print(f"最小二乘法测试结果: {mse}")

# 岭回归算法求解
ridge = RidgeRegression()
# 超参数搜索
params = {
    'lamb': [np.exp(-i) for i in range(1, 10)]
}
lamb, errors = cross_validation(ridge, train_data, params)

# plt.plot(params['lamb'], errors, '-x')
# plt.title('Cross Validation')
# plt.show()

ridge.lamb = lamb
ridge.fit(train_data, verbose=True)

# 岭回归测试
pred = ridge.predict(test_data[:, :-1])
mse = mean_squared_error(test_data[:, -1], pred)
print(f"岭回归测试结果: {mse}")

# 绘制结果
plt.scatter(train_data[:,0], train_data[:,1], label='train')
plt.scatter(test_data[:,0], test_data[:,1], label='test')
plt.title('Result')
plt.legend()
plt.show()

# 多项式回归算法
polynomial = PolynomialRegression()
# 超参数搜索
params = {
    'degree': [i for i in range(1, 5)]
}
degree, errors = cross_validation(polynomial, train_data, params)

plt.plot(params['degree'], errors, '-x')
plt.title('Cross Validation')
plt.show()