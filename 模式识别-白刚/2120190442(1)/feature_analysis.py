import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('iris.txt', delimiter=',')

mean = np.mean(data, axis=0)
std = np.std(data, axis=0)

print(mean, std)