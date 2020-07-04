#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <vector>
#include <QString>
#include <iostream>
#include "matrix.h"

using namespace std;

class NeuralNetwork
{
public:
    NeuralNetwork();
    ~NeuralNetwork();
    NeuralNetwork(vector<int> hiddens, double lr, QString mode);
    // 训练方法
    void train(matrix x, matrix y);
    // 预测方法
    matrix predict(matrix x);
    // 损失函数
    double loss(matrix y, matrix _y);

private:
    // 学习率
    double p_lr;
    // 网络层数
    int n_layers;
    // 网络层参数
    vector<matrix> layers;
    // 运算加速
    QString p_mode;

    // 前向传播算法
    vector<vector<matrix>> forward(matrix input);
    // 反向传播算法
    void backward(matrix output, vector<vector<matrix>> cache);
    // 初始化网络层参数
    matrix initializer_of_layer(int m, int n);
    // 激活函数
    matrix sigmoid(matrix z, bool flag);
};

#endif // NEURALNETWORK_H
