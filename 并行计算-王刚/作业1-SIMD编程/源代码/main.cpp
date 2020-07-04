#include <QCoreApplication>
#include <QDebug>
#include <cstdlib>
#include <iostream>
#include "matrix.h"
#include "dataloader.h"
#include "neuralnetwork.h"
#include "conjugategradient.h"

#define N_ITER 60

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    clock_t start = clock(); double dt, acc;
    // 加载数据集
    DataLoader dataloader("D:\\CplusCode\\mnist");
    // 定义网络结构
    vector<int> n_layers = {784, 900, 10};
//    vector<matrix> batch;

    NeuralNetwork nn(n_layers, 0.0001, "normal");

    start = clock();
    cout<<"[Normal Mode] Begin to train..."<<endl;
    for (int i = 0; i < N_ITER; i++) {
        vector<matrix> batch = dataloader.getBatch(64, "valid");
        nn.train(batch[0], batch[1]);
        if (i != N_ITER - 1)
            cout<<"\r";
        vector<matrix>().swap(batch);
    }
    dt = static_cast<double>(clock() - start)/CLOCKS_PER_SEC;
    qDebug()<<"\nTime: "<<dt<<"s"<<endl;

//    batch = dataloader.getBatch(128, "valid");
//    matrix pred = nn->predict(batch[0]);
//    acc = mean(batch[1].argmax(1) == pred.argmax(1));
//    qDebug()<<"Acc: "<<acc*100<<"%"<<endl;
//    vector<matrix>().swap(batch);

    NeuralNetwork nn2(n_layers, 0.0001, "sse");

    start = clock();
    cout<<"[SSE Mode] Begin to train..."<<endl;
    for (int i = 0; i < N_ITER; i++) {
        cout<<"";
        vector<matrix> batch = dataloader.getBatch(64, "valid");
        nn2.train(batch[0], batch[1]);
        if (i != N_ITER - 1)
            cout<<"\r";
        vector<matrix>().swap(batch);
    }
    dt = static_cast<double>(clock() - start)/CLOCKS_PER_SEC;
    qDebug()<<"\nTime: "<<dt<<"s"<<endl;

//    batch = dataloader.getBatch(-1, "valid");
//    matrix pred = nn->predict(batch[0]);
//    acc = mean(batch[1].argmax(1) == pred.argmax(1));
//    qDebug()<<"Acc: "<<acc*100<<"%"<<endl;
//    vector<matrix>().swap(batch);

    return a.exec();
}
