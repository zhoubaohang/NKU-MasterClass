#ifndef UDPCLIENT_H
#define UDPCLIENT_H

#include <QDebug>
#include <QObject>
#include <QUdpSocket>
#include <QList>
#include <QStatusBar>
#include <QDateTime>
#include <QLineEdit>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QCheckBox>
#include <QPlainTextEdit>

class User
{
public:
    User(QString, QString, int);
    QString getName() const {return m_name;}
    QString getIP() const {return m_ip;}
    int getPort() const {return m_port;}
    bool getChecked() const {return m_checked;}
    void setChecked(bool flag) {m_checked=flag;}

    bool operator==(const User);
    QString toString() const {return QString("%1,%2,%3").arg(m_name).arg(m_ip).arg(m_port);}

private:
    QString m_ip = "";
    QString m_name = "";
    int m_port = 0;
    bool m_checked = false;
};

class UDPClient : public QObject
{
    Q_OBJECT
public:
    UDPClient(QLineEdit*, QPlainTextEdit*);
    ~UDPClient();
    void close();

signals:
    void updateUsers(QList<User>);


public slots:
    void on_login(QString, int, QString);
    void on_logout();
    void on_broadcast(QList<int>, QString);

private slots:
    void onRecvData();

private:
    QPlainTextEdit *te_content = nullptr;
    QUdpSocket *m_socket = nullptr;
    QList<User> *m_users = nullptr;

    QString m_server_ip = "";
    quint16 m_server_port = 0;
    QString m_name = "";
    bool m_login_flag = false;

    QString getHostIpAddress();
    QString getUserName(QString, int);
};

#endif // UDPCLIENT_H
