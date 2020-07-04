#ifndef DATALOADER_H
#define DATALOADER_H

#include <QFile>
#include <QDebug>
#include <QString>
#include <chrono>
#include <random>
#include <iostream>
#include "matrix.h"

using namespace std;

class DataLoader
{
public:
    DataLoader();
    DataLoader(QString path);
    vector<matrix> getBatch(int batch_size, QString dtype);

private:
    QString p_path;
    vector<matrix> train_data;
    vector<matrix> valid_data;
    vector<matrix> test_data;

    vector<matrix> load_data(QString path);
};

#endif // DATALOADER_H
