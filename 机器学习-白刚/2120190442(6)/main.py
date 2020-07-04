import numpy as np
from mdn import MDN
import matplotlib.pyplot as plt
np.random.seed(0)

# 生成数据
data_size = 200
x = np.random.uniform(size=data_size)
epsilon = np.random.uniform(low=-0.1, high=0.1, size=data_size)
t = x + 0.3 * np.sin(2*np.pi*x) + epsilon
# 绘制数据散布图
plt.scatter(t, x)
plt.show()

# 训练模型
model = MDN()
model.fit(t.reshape((1, -1)), x.reshape((1, -1)), epoch=2500)

# 测试模型
x_test = np.arange(0, 1, 0.01)
pi, mu, pred = model.predict(x_test.reshape((1, -1)))
plt.scatter(t, x)
plt.scatter(x_test, pred, s=6, c='r')
plt.show()