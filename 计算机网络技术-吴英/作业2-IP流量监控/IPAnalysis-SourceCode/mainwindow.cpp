#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // set Table adapted to contents
    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    this->ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);

    this->p_netadaption = new NetAdaption();
    this->p_network = new Network(this->p_netadaption);

    connect(this, SIGNAL(comboBox_loaded(QComboBox*)), this->p_netadaption, SLOT(findAllDevs(QComboBox*)));
    emit this->comboBox_loaded(this->ui->cb_netadaptor);

    connect(this->ui->cb_netadaptor, SIGNAL(currentIndexChanged(QString)), this->p_netadaption, SLOT(selectDev(QString)));
    connect(this->ui->pushButton, SIGNAL(pressed()), this->p_network, SLOT(init_dev()));
    connect(this, SIGNAL(craw_package(QTableWidget*, QStatusBar*, QMap<QString, QString>, bool)),
            this->p_network, SLOT(startCraw(QTableWidget*, QStatusBar*, QMap<QString, QString>, bool)));

    this->p_netadaption->selectDev(this->ui->cb_netadaptor->currentText());
}

MainWindow::~MainWindow()
{
    delete this->p_network;
    delete this->p_netadaption;
    delete ui;
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(nullptr, "软件信息", "作者：周宝航 学号：2120190442 专业：计算机科学与技术");
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    this->ui->pushButton->setText(checked?"停止抓取":"开始抓取");

    QMap<QString, QString> params;
    if (checked)
    {
        params.insert("src", this->ui->le_sourceIP->text());
        params.insert("tge", this->ui->le_targetIP->text());
        params.insert("ptc", this->ui->cb_protocal->currentText());
        this->ui->tableWidget->setSortingEnabled(false);
    }
    else
    {
        this->ui->tableWidget->setSortingEnabled(true);
    }

    emit this->craw_package(this->ui->tableWidget, this->ui->statusBar, params, checked);
}

void MainWindow::on_pushButton_2_clicked()
{
    if (this->ui->tableWidget->rowCount()==0)
    {
        QMessageBox::information(nullptr, nullptr, "没有抓取的数据包！");
    }
    else
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Excel file"), qApp->applicationDirPath (),tr("Files (*.csv)"));
        if (filename.isEmpty())
            return;

        QFile fp(filename);
        if(!fp.open(QIODevice::WriteOnly | QIODevice::Text))
        {
           return;
        }

        QTextStream outstream(&fp);

        for (int i = 0; i < this->ui->tableWidget->horizontalHeader()->count(); i++)
        {
            outstream<<this->ui->tableWidget->horizontalHeaderItem(i)->text()<<",";
        }
        outstream<<"\n";

        for (int i = 0; i < this->ui->tableWidget->rowCount(); i++)
        {
            for (int j = 0; j < this->ui->tableWidget->columnCount(); j++)
            {
                outstream<<this->ui->tableWidget->item(i, j)->text()<<",";
            }
            outstream<<"\n";
        }

        fp.close();

        QMessageBox::information(nullptr, nullptr, "导出成功");
    }
}
