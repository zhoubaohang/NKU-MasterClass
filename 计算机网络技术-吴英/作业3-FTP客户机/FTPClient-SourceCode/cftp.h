#ifndef CFTP_H
#define CFTP_H

#include <cstdio>
#include <QDebug>
#include <QRegExp>
#include <QStatusBar>
#include <QTcpSocket>
#include <QString>
#include <QDataStream>
#include <QStandardItemModel>
#include <QVector>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>

class CFtp: public QObject
{
    Q_OBJECT

public:
    CFtp(QStatusBar*);
    ~CFtp();
    void quit();

signals:
    void list(QVector<QStringList>);
    void list(const QModelIndex&, QVector<QStringList>);
    void savefile(QByteArray, QString);
    void delete_treeview_item(const QModelIndex&);

public slots:
    void init_connections(QString, QString, QString);
    void on_treeview_clicked(const QModelIndex&);
    void on_treeview_doubleclicked(const QModelIndex&);
    void on_delete_action(const QModelIndex&);
    void on_upload_action(const QModelIndex&, QByteArray, QString);

private slots:
    void ctl_ts_readdata();
    void ctl_ts_error(QAbstractSocket::SocketError);
    void dta_ts_readdata();
    void dta_ts_error(QAbstractSocket::SocketError);

private:
    QString getFullPath(const QModelIndex);

    QTcpSocket *ctl_ts = nullptr;
    QTcpSocket *dta_ts = nullptr;
    QByteArray upload_data;

    QStatusBar *statusbar = nullptr;

    const QModelIndex *curr_index = nullptr;
    QString curr_filename = "";

    bool flag = false;

    QString server = "";
    QString user = "";
    QString pwd = "";
    int cmd = -1;
    quint16 remote_port = 0;

    QByteArray data_buffer;
};

#endif // CFTP_H
