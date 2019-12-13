#ifndef NETADAPTION_H
#define NETADAPTION_H
#define HAVE_REMOTE

#include <QMessageBox>
#include <QComboBox>
#include <QMap>
#include <QDebug>
#include <QString>
#include <QObject>
#include "pcap.h"
#include "remote-ext.h "
#include "time.h"

class NetAdaption: public QObject {
    Q_OBJECT

public:
    NetAdaption();
    ~NetAdaption();

    pcap_t* OpenDev();

public slots:
    void findAllDevs(QComboBox*);
    void selectDev(QString);

private:
    pcap_if_t *p_devs = nullptr;
    pcap_if_t *p_dev = nullptr;

};

#endif // NETADAPTION_H
