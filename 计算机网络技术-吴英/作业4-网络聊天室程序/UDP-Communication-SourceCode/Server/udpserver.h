#ifndef UDPSERVER_H
#define UDPSERVER_H

#include <QDebug>
#include <QUdpSocket>
#include <QList>
#include <QString>
#include <QNetworkInterface>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QDateTime>
#include <QMessageBox>

class User
{
public:
    User(QString, QString, int);
    QString getName() const {return m_name;}
    QString getIP() const {return m_ip;}
    int getPort() const {return m_port;}

    bool operator==(const User);
    QString toString() const {return QString("%1,%2,%3").arg(m_name).arg(m_ip).arg(m_port);}

private:
    QString m_ip = "";
    QString m_name = "";
    int m_port = 0;
};

class UDPServer: public QObject
{
    Q_OBJECT

public:
    UDPServer(QPlainTextEdit*, QLineEdit*);
    void close();

public slots:
    void on_control_connect(bool, QString, quint16);

private slots:
    void onRecvData();

signals:
    void addUser(User);
    void deleteUser(int);

private:
    QUdpSocket *m_socket = nullptr;
    QPlainTextEdit *m_logger = nullptr;
    QList<User> *m_users = nullptr;

    QString getHostIpAddress();
    QString getUsers();
    bool checkUserName(QString);
    void broadcastUserLists();

};

#endif // UDPSERVER_H
