#include "udpserver.h"

User::User(QString name, QString ip, int port)
{
    this->m_name = name;
    this->m_ip = ip;
    this->m_port = port;
}

bool User::operator==(const User a)
{
    bool flag = false;
    if (!(a.getIP().compare(this->m_ip)) &&
        !(a.getName().compare(this->m_name)) &&
          a.getPort() == this->m_port)
        flag = true;
    return flag;
}

UDPServer::UDPServer(QPlainTextEdit *logger, QLineEdit *le)
{
    le->setText(this->getHostIpAddress());
    this->m_logger = logger;
}

void UDPServer::close()
{
    if (this->m_socket != nullptr)
    {
        this->m_socket->close();
        delete this->m_socket;
        this->m_users->clear();
        delete this->m_users;
        this->m_logger->appendPlainText("关闭UDP监听成功！");
    }
}

void UDPServer::on_control_connect(bool flag, QString ip, quint16 port)
{
    if (flag)
    {
        this->m_socket = new QUdpSocket();
        connect(this->m_socket, SIGNAL(readyRead()), this, SLOT(onRecvData()));
        this->m_users = new QList<User>();

        this->m_socket->bind(QHostAddress(ip), port);
        this->m_logger->appendPlainText("开启UDP监听成功！");
    }
    else {
        this->close();
    }
}

QString UDPServer::getUsers()
{
    QString users = "";

    for(int i = 0; i < this->m_users->size(); i++)
    {
        users += QString("%1\r\n").arg(this->m_users->at(i).toString());
    }

    return users;
}

bool UDPServer::checkUserName(QString name)
{
    bool flag = true;

    for(int i = 0; i < this->m_users->size(); i++)
        if (!this->m_users->at(i).getName().compare(name))
            flag = false;
    return flag;
}

void UDPServer::broadcastUserLists()
{
    QString userlists = QString("302\r\n%1").arg(this->getUsers());
    for (int i = 0; i < this->m_users->size(); i++)
    {
        User user = this->m_users->at(i);
        this->m_socket->writeDatagram(userlists.toUtf8(), QHostAddress(user.getIP()),
                                      static_cast<quint16>(user.getPort()));
    }
}

void UDPServer::onRecvData()
{

    while(this->m_socket->hasPendingDatagrams())
    {
        QByteArray data_array;
        QHostAddress ip;
        quint16 port;
        data_array.resize(static_cast<int>(this->m_socket->pendingDatagramSize()));
        this->m_socket->readDatagram(data_array.data(), data_array.size(), &ip, &port);

        QString data = QString(data_array);
        qDebug()<<"Recv:"<<data;

        QString split_flag = "\r\n";

        int code = data.split(split_flag)[0].toInt();
        // 获取当前时间
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");

        switch (code) {
            case 201: {
                    QString name = data.split(split_flag)[1];
                    // 实例化User对象
                    User user(name, ip.toString(), port);
                    // 索引User
                    int idx = this->m_users->indexOf(user);
                    if (idx < 0) {
                        if (this->checkUserName(name))
                        {
                            // Log
                            this->m_logger->appendPlainText(QString("%1 (%2 %3) 于 %4 登录").arg(name)
                                    .arg(ip.toString()).arg(port).arg(current_date));
                            // 添加在线用户信息
                            this->m_users->append(user);
                            emit this->addUser(user);
                            // 返回确认指令
                            this->m_socket->writeDatagram(QString("200\r\n").toUtf8(), ip, port);
                            // 广播更新用户列表信息
                            this->broadcastUserLists();
                        } else {
                            // 返回重名警告
                            this->m_socket->writeDatagram(QString("510\r\n").toUtf8(), ip, port);
                        }
                    }
                    break;
                } // case 201
            case 301: {
                    QString data = QString("302\r\n%1").arg(this->getUsers());
                    this->m_socket->writeDatagram(data.toUtf8(), ip, port);
                    break;
                }
            case 501: {
                    QString name = data.split(split_flag)[1];
                    // 实例化User对象
                    User user(name, ip.toString(), port);
                    // 索引User
                    int idx = this->m_users->indexOf(user);
                    // 存在该用户
                    if (idx >= 0) {
                        // Log
                        this->m_logger->appendPlainText(QString("%1 (%2 %3) 于 %4 退出").arg(name)
                                .arg(ip.toString()).arg(port).arg(current_date));
                        this->m_users->removeAt(idx);
                        emit this->deleteUser(idx);
                        // 广播更新用户列表信息
                        this->broadcastUserLists();
                    }
                    break;
                } // case 501
        }
    }
}

QString UDPServer::getHostIpAddress()
{
    QString strIpAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // 获取第一个本主机的IPv4地址
    int nListSize = ipAddressesList.size();
    for (int i = 0; i < nListSize; ++i)
    {
           if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
               ipAddressesList.at(i).toIPv4Address()) {
               strIpAddress = ipAddressesList.at(i).toString();
               break;
           }
     }
     // 如果没有找到，则以本地IP地址为IP
     if (strIpAddress.isEmpty())
        strIpAddress = QHostAddress(QHostAddress::LocalHost).toString();
     return strIpAddress;
}
