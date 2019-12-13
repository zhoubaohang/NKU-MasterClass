#include "udpclient.h"

User::User(QString name, QString ip, int port)
{
    this->m_name = name;
    this->m_ip = ip;
    this->m_port = port;
}

UDPClient::UDPClient(QLineEdit *le_ip, QPlainTextEdit *te_content)
{
    this->te_content = te_content;
    this->m_users = new QList<User>();
    this->m_socket = new QUdpSocket(this);
    connect(this->m_socket, SIGNAL(readyRead()), this, SLOT(onRecvData()));
    quint16 port = 10000;
    QString localip = this->getHostIpAddress();
    while (!this->m_socket->bind(QHostAddress(localip), port)) { port ++; }

    le_ip->setText(localip);
}

UDPClient::~UDPClient()
{
    this->close();
    delete this->m_socket;
    delete this->m_users;
}

void UDPClient::close()
{
    if (this->m_socket != nullptr)
    {
        this->on_logout();
        this->m_socket->close();
        this->m_users->clear();
    }
}

void UDPClient::on_login(QString ip, int port, QString name)
{
    this->m_server_ip = ip;
    this->m_server_port = static_cast<quint16>(port);
    this->m_name = name;

    this->m_socket->writeDatagram(QString("201\r\n%1").arg(name).toUtf8(), QHostAddress(ip), m_server_port);
}

void UDPClient::on_logout()
{
    if (this->m_login_flag)
    {
        this->m_socket->writeDatagram(QString("501\r\n%1").arg(this->m_name).toUtf8(), QHostAddress(this->m_server_ip), this->m_server_port);
        this->m_login_flag = !this->m_login_flag;
    }
}

QString UDPClient::getUserName(QString ip, int port)
{
    QString name = "";
    for (int i = 0; i < this->m_users->size(); i++)
    {
        User user = m_users->at(i);
        if (!ip.compare(user.getIP()) && port == user.getPort())
            name = user.getName();
    }
    return name;
}

void UDPClient::onRecvData()
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
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");

        switch (code) {
            case 200:
                this->m_socket->writeDatagram(QString("301\r\n").toUtf8(), QHostAddress(this->m_server_ip), this->m_server_port);
                this->m_login_flag = true;
                break;
            case 302: {
                this->m_users->clear();
                QStringList users = data.split(split_flag);
                for (int i = 1; i < users.size(); i++)
                {
                    if (users[i] != "")
                    {
                        QStringList props = users[i].split(",");
                        if (props[0].compare(this->m_name))
                        {
                            User user(props[0], props[1], props[2].toInt());
                            this->m_users->append(user);
                        }
                    }
                }
                emit this->updateUsers(*this->m_users);
                break;
            }
            case 401: {
                QString name = this->getUserName(ip.toString(), port);
                QString content = data.split(split_flag)[1];
                te_content->appendPlainText(QString("%1\n").arg(content));
                break;
            }
            case 510:
                QMessageBox::information(nullptr, "警告", "用户名已存在");
                break;
        }

    }
}

void UDPClient::on_broadcast(QList<int> index, QString content)
{
    if (this->m_login_flag)
    {
        // 获取当前时间
        QDateTime current_date_time = QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
        if (index.size())
            te_content->appendPlainText(QString("Youself (%1):\n%2\n").arg(current_date).arg(content));
        for (int i = 0; i < index.size(); i++)
        {
            User user = this->m_users->at(index[i]);
            this->m_socket->writeDatagram(QString("401\r\nUser:%1 (%2)\n%3").arg(this->m_name).arg(current_date).arg(content).toUtf8(),
                                          QHostAddress(user.getIP()), static_cast<quint16>(user.getPort()));
        }
    }
}

QString UDPClient::getHostIpAddress()
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
