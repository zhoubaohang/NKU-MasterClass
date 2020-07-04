#include "neuralnetwork.h"

NeuralNetwork::NeuralNetwork()
{

}

NeuralNetwork::~NeuralNetwork()
{
    vector<matrix>().swap(this->layers);
}

NeuralNetwork::NeuralNetwork(vector<int> hiddens, double lr, QString mode)
{
    this->p_lr = lr;
    this->p_mode = mode;
    this->n_layers = hiddens.size();

    for (int i = 0; i < n_layers-1; i++) {
        this->layers.push_back(this->initializer_of_layer(hiddens[i+1], hiddens[i]));
    }
}

void NeuralNetwork::train(matrix x, matrix y)
{
    vector<vector<matrix>> cache = this->forward(x);
    double loss = this->loss(y, cache[1][cache[1].size()-1].transpose());
    cout<<"Loss:"<<loss;
    this->backward(y.transpose(), cache);
}

double NeuralNetwork::loss(matrix y, matrix _y)
{
    matrix tmp = y * ele_log(_y) + (1 - y) * ele_log(1 - _y);
    double loss = 0 - sum(tmp);
    return loss / tmp.getRowSize();
}

matrix NeuralNetwork::predict(matrix x)
{
    vector<vector<matrix>> cache = this->forward(x);
    matrix activations = cache[1][n_layers-1].transpose();
    matrix output(activations.getRowSize(), 10, 0);

    for (int i = 0; i < output.getRowSize(); i++){
        int idx = 0;
        double val = 0;
        for (int j = 0; j < activations.getColSize(); j++) {
            if (activations(i, j) > val) {
                idx = j;
                val = activations(i,j);
            }
        }
        output(i, idx) = 1;
    }
    return output;
}

vector<vector<matrix>> NeuralNetwork::forward(matrix input)
{
    vector<matrix> zs;
    vector<matrix> activations = {input.transpose()};
    vector<vector<matrix>> cache;

    for (int i = 0; i < layers.size(); i++) {
        matrix tmp(1,1,0);
        // 循环每层权重，前向计算每层输出值
        if (this->p_mode == "normal")
            tmp = layers[i].mult(activations[activations.size() - 1]);
        else if (this->p_mode == "sse")
            tmp = layers[i].sse_mult(activations[activations.size() - 1]);
        zs.push_back(matrix(tmp));
        // 保存经过激活函数数值
        activations.push_back(sigmoid(tmp, false));
    }

    cache.push_back(zs);
    cache.push_back(activations);
    return cache;
}

void NeuralNetwork::backward(matrix output, vector<vector<matrix>> cache)
{
    int n_layers = this->n_layers;
    vector<matrix> zs = cache[0];
    vector<matrix> activations = cache[1];
    // 每层权重对应的梯度
    vector<matrix> nable_w;

    for (auto l : layers)
        nable_w.push_back(matrix(l.getRowSize(), l.getColSize(), 0));
    // 计算损失
    matrix cost = activations[n_layers - 1] - output;
    // 计算梯度
    matrix gradient = cost * sigmoid(zs[n_layers - 2], true);
    // 计算最后一层权重的梯度
    if (this->p_mode == "normal")
        nable_w[nable_w.size() - 1] = gradient.mult(activations[n_layers - 2].transpose());
    else if (this->p_mode == "sse")
        nable_w[nable_w.size() - 1] = gradient.sse_mult(activations[n_layers - 2].transpose());
    // 反向传播
    for (int i = 2; i <= layers.size(); i++) {
        // 反向传播梯度，并计算每层权重的梯度值
        if (this->p_mode == "normal") {
            gradient = layers[n_layers-i].transpose().mult(gradient)
                        * sigmoid(zs[n_layers-i-1], true);
            nable_w[n_layers-i-1] = gradient.mult(activations[n_layers-i-1].transpose());
        }
        else if (this->p_mode == "sse") {
            gradient = layers[n_layers-i].transpose().sse_mult(gradient)
                        * sigmoid(zs[n_layers-i-1], true);
            nable_w[n_layers-i-1] = gradient.sse_mult(activations[n_layers-i-1].transpose());
        }
    }
    // 利用梯度，更新每层权重
    for (int i = 0; i < layers.size(); i++) {
        layers[i] = layers[i] - nable_w[i] * p_lr;
    }
}

matrix NeuralNetwork::sigmoid(matrix z, bool flag)
{
    matrix tmp(z);
    for (int i = 0; i < z.getRowSize(); i++)
        for (int j = 0; j < z.getColSize(); j++)
            tmp(i,j) = double(1) / double(double(1) + exp(double(0) - tmp(i,j)));

    if (flag)
        tmp = tmp * (double(1) - tmp);

    return tmp;
}

matrix NeuralNetwork::initializer_of_layer(int m, int n)
{
    matrix tmp(m, n, 0);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            tmp(i,j) = double(rand()%10000+1) / double(1000000);
    return tmp;
}
