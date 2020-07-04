import numpy as np
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis

# 加载数据集
data = np.loadtxt('Ionosphere+Dataset.csv', delimiter=',', dtype=str)
train_data = data[:200]
test_data = data[200:]
# 划分为训练集和测试集
train_x, train_y = train_data[:, :-1].astype(np.float64), train_data[:, -1]
test_x, test_y = test_data[:, :-1].astype(np.float64), test_data[:, -1]

lda = LinearDiscriminantAnalysis('eigen')
lda.fit(train_x, train_y)
pred = lda.predict(train_x)
acc = np.mean(pred == train_y)
print(f"train {1-acc}")

pred = lda.predict(test_x)
acc = np.mean(pred == test_y)
print(f"test {1-acc}")