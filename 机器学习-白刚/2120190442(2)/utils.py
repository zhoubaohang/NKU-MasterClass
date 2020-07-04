import numpy as np
import matplotlib.pyplot as plt

def plot_data(x, y, title='Data'):
    '''
        生成数据图
    '''
    plt.scatter(x, y)
    plt.xlabel('X')
    plt.ylabel('Y')
    plt.title(title)
    plt.show()

def split_data(data, test_split=0.2):
    '''
        划分数据集
    '''
    size = len(data)
    train_size = int(size * (1. - test_split))
    train = data[:train_size]
    test = data[train_size:]
    return (train, test)

def cross_validation(model, data, params=[]):
    '''
        余一交叉验证
    '''
    result = {}
    for param in params:
        model.sigma = param
        errors = []
        for i in range(len(data)):
            tmp = data.tolist()
            # 余一测试样本
            test_data = np.array(tmp[i]).reshape((1, -1))
            del tmp[i]
            # 训练样本
            train_data = np.array(tmp)
            # 模型训练
            model.fit(train_data, verbose=False)
            # 模型预测
            pred = model.predict(test_data)
            # 预测误差
            errors.append(np.mean(np.square(pred.flatten() - test_data[:, -1])))
        # 每个超参数平均误差
        result[param] = np.mean(errors)
    
    min_error = 100.
    best_param = 0.
    for k, v in result.items():
        print(f"参数:{k} 误差:{v}")
        if v < min_error:
            best_param = k
            min_error = v
    
    return best_param