#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->m_server = new UDPServer(this->ui->te_logger, this->ui->le_ip);

    connect(this, SIGNAL(control_connect(bool, QString, quint16)),
            this->m_server, SLOT(on_control_connect(bool, QString, quint16)));
    connect(this->m_server, SIGNAL(addUser(User)), this, SLOT(on_add_user(User)));
    connect(this->m_server, SIGNAL(deleteUser(int)), this, SLOT(on_delete_user(int)));
}

void MainWindow::on_add_user(User user)
{
    int rows = this->ui->tw_users->rowCount();
    this->ui->tw_users->insertRow(rows);
    this->ui->tw_users->setItem(rows, 0, new QTableWidgetItem(user.getName()));
    this->ui->tw_users->setItem(rows, 1, new QTableWidgetItem(user.getIP()));
    this->ui->tw_users->setItem(rows, 2, new QTableWidgetItem(QString("%1").arg(user.getPort())));
}

void MainWindow::on_delete_user(int index)
{
//    qDebug()<<"删除 "<<index;
    this->ui->tw_users->removeRow(index);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    QString ip = this->ui->le_ip->text();
    quint16 port = this->ui->le_port->text().toUShort();
    QString btn = checked?"关闭":"开启";
    this->ui->le_port->setEnabled(!checked);

    if (checked)
    {
        if (port)
        {
            emit this->control_connect(checked, ip, port);
        }
        else
        {
            QMessageBox::information(nullptr, "警告", "请输入端口号");
        }
    }
    else
    {
        emit this->control_connect(checked, ip, port);
        this->ui->tw_users->clearContents();
        this->ui->tw_users->setRowCount(0);
    }
    this->ui->pushButton->setText(btn);
}
