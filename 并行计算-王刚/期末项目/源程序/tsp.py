# -*- coding: utf-8 -*-
"""
Created on Fri Dec 21 21:10:34 2018

@author: 周宝航
"""
import numpy as np
import matplotlib.pyplot as plt

class TSP(object):
    
    def __init__(self, n_cities):
        self.n_cities = n_cities
        self.cities = np.random.rand(n_cities, 2)
    
    def plot_result(self, line_x, line_y):
        '''
            Plot the result
        '''
        plt.scatter(self.cities[:, 0], self.cities[:, 1])
        
        for i in range(self.n_cities-1):
            plt.plot(line_x[i:i+2], line_y[i:i+2], c='r')