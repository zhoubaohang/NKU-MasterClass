#include "cftp.h"

CFtp::CFtp(QStatusBar *statusbar)
{
    this->statusbar = statusbar;

    this->ctl_ts = new QTcpSocket(this);
//    this->ctl_ts->bind(8000);
    this->dta_ts = new QTcpSocket(this);
//    this->dta_ts->bind(8001);

    this->ctl_ts->abort();
    this->dta_ts->abort();

    connect(this->ctl_ts, SIGNAL(readyRead()), this, SLOT(ctl_ts_readdata()));
    connect(this->ctl_ts, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(ctl_ts_error(QAbstractSocket::SocketError)));
    connect(this->dta_ts, SIGNAL(readyRead()), this, SLOT(dta_ts_readdata()));
    connect(this->dta_ts, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(dta_ts_error(QAbstractSocket::SocketError)));
}

CFtp::~CFtp()
{
    this->quit();
    if (this->ctl_ts != nullptr)
    {
        this->ctl_ts->disconnectFromHost();
        delete this->ctl_ts;
    }
}

void CFtp::quit()
{
    if (this->flag)
    {
        this->ctl_ts->write(QString("QUIT\r\n").toLatin1());
    }
}

void CFtp::init_connections(QString server, QString user, QString pwd)
{
    this->server = server;
    this->ctl_ts->connectToHost(server, 21);
    if (this->ctl_ts->waitForConnected(1000))
    {
        this->ctl_ts->write(QString("USER %1\r\n").arg(user).toLatin1());
        this->user = user;
        this->pwd = pwd;
    }
}

void CFtp::ctl_ts_readdata()
{
    QByteArray bytedata = this->ctl_ts->readAll();
    qDebug()<<bytedata;
    int code = QString(bytedata).split(" ")[0].toInt();

    switch (code) {
        case 221:
            this->flag = false;
            this->statusbar->showMessage("用户退出成功", 1000);
            break;
        case 227:{
            QString strdata = QString(bytedata);
            QStringList data = strdata.split(',');
            int port1 = data[4].toInt();
            int port2 = data[5].split(')')[0].toInt();
            this->remote_port = static_cast<quint16>(port1*256 + port2);
            this->dta_ts->connectToHost(this->server, remote_port);
            if (!this->flag)
            {
                this->flag = true;
                this->ctl_ts->write(QString("LIST\r\n").toLatin1());
                this->cmd = 0;
            }
            if (this->upload_data.size() != 0)
            {
                this->dta_ts->write(this->upload_data);
                this->dta_ts->close();
                this->upload_data.clear();
            }
            break;
        }
        case 230:
            this->statusbar->showMessage("用户登录成功", 1000);
            this->ctl_ts->write(QString("TYPE I\r\n").toLatin1());
            this->ctl_ts->write(QString("PASV\r\n").toLatin1());
            break;
        case 331:
            this->ctl_ts->write(QString("PASS %1\r\n").arg(this->pwd).toLatin1());
            break;
        case 250:
            this->statusbar->showMessage("删除成功", 1000);
            emit this->delete_treeview_item(*this->curr_index);
            break;
        case 0:
            QMessageBox::information(nullptr, "提示", "登录失败，请检查用户名或密码");
            break;
    }
}

void CFtp::ctl_ts_error(QAbstractSocket::SocketError)
{
    qDebug()<<this->ctl_ts->errorString();
}

void CFtp::dta_ts_readdata()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");

    switch (this->cmd) {
        case 0:{ // 解析根目录文件
            QByteArray bytedata = this->dta_ts->readAll();
            QStringList files = codec->toUnicode(bytedata).split("\r\n");
            QVector<QStringList> datas;
            for(QString file: files)
            {
                QStringList infos;
                for(QString f: file.split(" "))
                {
                    if (!f.isEmpty())
                        infos.append(f);
                }
                if (infos.size())
                    datas.append(infos);
            }
            emit this->list(datas);
            break;
        } // case 0
        case 1:{ // 解析指定目录文件
            QByteArray bytedata = this->dta_ts->readAll();
            QStringList files = codec->toUnicode(bytedata).split("\r\n");
            QVector<QStringList> datas;
            for(QString file: files)
            {
                QStringList infos;
                for(QString f: file.split(" "))
                {
                    if (!f.isEmpty())
                        infos.append(f);
                }
                if (infos.size())
                    datas.append(infos);
            }
            emit this->list(*this->curr_index, datas);
            break;
        } // case 1
    case 2:{
            QByteArray data = this->dta_ts->readAll();

            while(this->dta_ts->waitForReadyRead())
            {
                data.append(this->dta_ts->readAll());
            }

            emit this->savefile(data, this->curr_filename);
            break;
        } // case 2
    }
}

void CFtp::dta_ts_error(QAbstractSocket::SocketError)
{
    qDebug()<<this->dta_ts->errorString();
}

QString CFtp::getFullPath(const QModelIndex index)
{
    QString path = index.data().toString().split("\t")[0];
    QModelIndex parent = index.parent();
    while (parent.data().toString() != "")
    {
        path = QString("%1\\%2").arg(parent.data().toString().split("\t")[0]).arg(path);
        parent = parent.parent();
    }
    return path;
}

void CFtp::on_treeview_clicked(const QModelIndex &index)
{
    if (index.data().toString().contains("DIR"))
    {
        this->ctl_ts->write(QString("PASV\r\n").toLatin1());
        this->ctl_ts->write(QString("LIST %1\r\n").arg(this->getFullPath(index)).toLatin1());
        this->cmd = 1;
        this->curr_index = &index;
    }
}

void CFtp::on_treeview_doubleclicked(const QModelIndex &index)
{
    if (index.data().toString().contains("FILE"))
    {
        QString path = this->getFullPath(index);
        this->ctl_ts->write(QString("PASV\r\n").toLatin1());
        QString cmd = QString("RETR %1\r\n").arg(path);
        this->ctl_ts->write(cmd.toLatin1());
        this->cmd = 2;
        this->curr_filename = index.data().toString().split("\t")[0];
    }
}

void CFtp::on_delete_action(const QModelIndex &index)
{
    QString path = this->getFullPath(index);
    QTextCodec *codec = QTextCodec::codecForName("GBK");

    if (index.data().toString().contains("FILE"))
    {
        QString cmd = QString("DELE %1\r\n").arg(path);

        this->ctl_ts->write(codec->fromUnicode(cmd));
        this->curr_index = &index;
    }
}

void CFtp::on_upload_action(const QModelIndex &index, QByteArray file, QString filename)
{
    QString cmd;
    QString path;
    QModelIndex parent;

    if (index.data().toString().contains("FILE"))
    {
        path = this->getFullPath(index.parent());
        parent = index.parent();
    }
    else
    {
        path = this->getFullPath(index);
        parent = index;
    }

    QString fp = filename.split("/").last();
    QTextCodec *codec = QTextCodec::codecForName("GBK");

    if (path == "")
    {
        cmd = QString("STOR %1\r\n").arg(fp);
    }
    else
    {
        cmd = QString("STOR %1\\%2\r\n").arg(path).arg(fp);
    }
    this->ctl_ts->write(QString("PASV\r\n").toLatin1());
    this->ctl_ts->write(codec->fromUnicode(cmd));
    this->upload_data = file;


    if (path == "")
    {
        this->ctl_ts->write(QString("PASV\r\n").toLatin1());
        this->ctl_ts->write(QString("LIST\r\n").toLatin1());
        this->cmd = 0;
    }
    else
    {
        this->ctl_ts->write(QString("PASV\r\n").toLatin1());
        this->ctl_ts->write(QString("LIST %1\r\n").arg(path).toLatin1());
        this->cmd = 1;
        this->curr_index = &index;
    }
}
