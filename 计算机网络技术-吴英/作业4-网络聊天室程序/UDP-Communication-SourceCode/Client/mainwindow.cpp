#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tw_users->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    this->m_client = new UDPClient(ui->le_ip, ui->te_content);
    connect(this, SIGNAL(do_login(QString, int, QString)), this->m_client, SLOT(on_login(QString, int, QString)));
    connect(this, SIGNAL(do_logout()), this->m_client, SLOT(on_logout()));
    connect(this->m_client, SIGNAL(updateUsers(QList<User>)), this, SLOT(on_update_users(QList<User>)));
    connect(this, SIGNAL(do_broadcast(QList<int>, QString)), this->m_client, SLOT(on_broadcast(QList<int>, QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->m_client;
}

void MainWindow::on_update_users(QList<User> users)
{
    this->ui->tw_users->clearContents();

    this->ui->tw_users->setRowCount(0);

    for (int i = 0; i < users.size(); i++)
    {
        int rows = this->ui->tw_users->rowCount();
        this->ui->tw_users->insertRow(rows);
        this->ui->tw_users->setCellWidget(rows, 0, new QCheckBox());
        this->ui->tw_users->setItem(rows, 1, new QTableWidgetItem(users[i].getName()));
    }
}


void MainWindow::on_btn_login_toggled(bool checked)
{
    QString ip = this->ui->le_ip->text();
    quint16 port = this->ui->le_port->text().toUShort();
    QString btn = checked?"退出":"登录";
    this->ui->le_ip->setEnabled(!checked);
    this->ui->le_port->setEnabled(!checked);
    this->ui->le_name->setEnabled(!checked);

    if (checked)
    {
        if (port)
        {
            emit this->do_login(ip, port, this->ui->le_name->text());
        }
        else
        {
            QMessageBox::information(nullptr, "警告", "请输入端口号");
        }
    }
    else
    {
        emit this->do_logout();
        this->ui->tw_users->clearContents();
        this->ui->tw_users->setRowCount(0);
    }
    this->ui->btn_login->setText(btn);
}

void MainWindow::on_btn_push_clicked()
{
    QString content = this->ui->te_input->toPlainText().trimmed();

    if (content.compare(""))
    {
        QList<int> index;
        for (int i = 0; i < this->ui->tw_users->rowCount(); i++)
        {
            QCheckBox *check = static_cast<QCheckBox*>(this->ui->tw_users->cellWidget(i, 0));
            if (check->checkState())
                index.append(i);
        }
        emit this->do_broadcast(index, content);
    }
}
