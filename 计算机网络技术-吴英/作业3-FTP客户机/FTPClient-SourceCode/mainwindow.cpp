#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->p_filetree = new TreeModel(ui->treeView);
    this->cftp = new CFtp(ui->statusbar);
    connect(this, SIGNAL(init_connections(QString, QString, QString)),
            this->cftp, SLOT(init_connections(QString, QString, QString)));
    connect(this->cftp, SIGNAL(list(QVector<QStringList>)),
            this->p_filetree, SLOT(on_list_checked(QVector<QStringList>)));
    connect(this->ui->treeView, SIGNAL(clicked(const QModelIndex&)),
            this->cftp, SLOT(on_treeview_clicked(const QModelIndex&)));
    connect(this->cftp, SIGNAL(list(const QModelIndex&, QVector<QStringList>)),
            this->p_filetree, SLOT(on_list_checked(const QModelIndex&, QVector<QStringList>)));
    connect(this->ui->treeView, SIGNAL(doubleClicked(const QModelIndex&)),
            this->cftp, SLOT(on_treeview_doubleclicked(const QModelIndex&)));
    connect(this->cftp, SIGNAL(savefile(QByteArray, QString)), this, SLOT(on_file_saved(QByteArray, QString)));
    connect(this, SIGNAL(delete_action(const QModelIndex&)),
            this->cftp, SLOT(on_delete_action(const QModelIndex&)));
    connect(this, SIGNAL(upload_action(const QModelIndex&, QByteArray, QString)),
            this->cftp, SLOT(on_upload_action(const QModelIndex&, QByteArray, QString)));
    connect(this->cftp, SIGNAL(delete_treeview_item(const QModelIndex&)),
            this->p_filetree, SLOT(on_delete_treeview_item(const QModelIndex&)));
}

MainWindow::~MainWindow()
{
    delete this->cftp;
    delete ui;
}

void MainWindow::on_pushButton_toggled(bool checked)
{
    this->ui->le_user->setEnabled(!checked);
    this->ui->le_server->setEnabled(!checked);
    this->ui->le_pwd->setEnabled(!checked);
    this->ui->pushButton->setText(checked?"退出":"登录");

    if (checked)
        emit this->init_connections(this->ui->le_server->text(),
                                    this->ui->le_user->text(),
                                    this->ui->le_pwd->text());
    else {
        this->cftp->quit();
        this->p_filetree->clear();
    }
}

void MainWindow::on_file_saved(QByteArray bytedata, QString name)
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save file"), name, QString("Files (*.%1)").arg(name.split(".")[1]));
    if (filename.isEmpty())
        return;

    QFile fp(filename);
    if(!fp.open(QIODevice::WriteOnly))
    {
       return;
    }

    fp.write(bytedata);
    fp.close();
    this->statusBar()->showMessage("保存成功", 1000);
}

void MainWindow::on_upload_action()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("请选择上传文件"), ".", "");
    if (filename.isEmpty())
        return;

    QFile fp(filename);
    if(!fp.open(QIODevice::ReadOnly))
    {
       return;
    }
    QByteArray byte_file = fp.readAll();
    fp.close();

    emit this->upload_action(this->curr_index, byte_file, filename);
}

void MainWindow::on_delete_action()
{
    emit this->delete_action(this->curr_index);
}

void MainWindow::on_treeView_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->treeView->indexAt(pos);
    this->curr_index = index;

    ui->treeView->setCurrentIndex(index);

    QMenu *menu = new QMenu(this);
    menu->addAction("上传", this, SLOT(on_upload_action()), 0);
    if (index.data().toString().contains("FILE"))
        menu->addAction("删除", this, SLOT(on_delete_action()), 0);
    menu->exec(ui->treeView->mapToGlobal(pos));
}
