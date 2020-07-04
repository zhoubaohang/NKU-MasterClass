#include "dataloader.h"

DataLoader::DataLoader()
{

}

DataLoader::DataLoader(QString path)
{
    this->p_path = path;
    cout<<"Loading data...."<<endl;
//    this->train_data = load_data(path+"\\train.csv");
    this->valid_data = load_data(path+"\\valid.csv");
//    this->test_data = load_data(path+"\\test.csv");
}

vector<matrix> DataLoader::getBatch(int batch_size, QString dtype)
{
    vector<matrix> *tmp;
    if (dtype == "train")
        tmp = &(this->train_data);
    else if (dtype == "valid")
        tmp = &(this->valid_data);
    else if (dtype == "test")
        tmp = &(this->test_data);
    else return *tmp;

    if (batch_size == -1)
        return (*tmp);
    else {
        vector<int> index;
        for (int i = 0; i < (*tmp)[0].getRowSize(); i++)
            index.push_back(i);
        unsigned seed = std::chrono::system_clock::now ().time_since_epoch ().count();
        shuffle(index.begin(), index.end(), default_random_engine (seed));

        vector<int> sliced;
        for (int i = 0; i < batch_size; i++)
            sliced.push_back(index[i]);

        matrix x = (*tmp)[0].index(sliced);
        matrix y = (*tmp)[1].index(sliced);

        vector<matrix> result = {x, y};

        return result;
    }
}

vector<matrix> DataLoader::load_data(QString path)
{
    vector<matrix> data;
    QFile file(path);
    QStringList data_list;
    data_list.clear();
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        while(!stream.atEnd()) {
            data_list.push_back(stream.readLine());
        }
        file.close();
    }

    int n_feature = data_list[0].split(',').size() - 1;
    matrix x(data_list.size(), n_feature, 0);
    matrix y(data_list.size(), 10, 0);

    for (int i = 0; i < data_list.size(); i++) {
        QStringList tmp = data_list[i].split(',');
        for (int j = 0; j < tmp.size(); j++) {
            if (j == tmp.size() - 1) {
                y(i, tmp[j].toInt()) = 1;
            } else {
                x(i, j) = tmp[j].toDouble();
            }
        }
    }

    data.push_back(x);
    data.push_back(y);

    return data;
}
