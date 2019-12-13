#ifndef NETWORK_H
#define NETWORK_H
#define HAVE_REMOTE

#include <QMap>
#include <QStatusBar>
#include <QStringList>
#include <QDateTime>
#include <QTableWidget>
#include <winsock2.h>
#include <QObject>
#include <QThread>
#include "pcap.h"
#include "remote-ext.h "
#include "netadaption.h"

struct EtherHeader//以太网首部
{
    u_int8_t ether_dhost[6]; //目的Mac地址
    u_int8_t ether_shost[6]; //源Mac地址
    u_int16_t ether_type;    //协议类型
};

/* IP报文包数据结构 */
struct IPHeader{
#if defined(WORDS_BIENDIAN)   //考虑大端序与小端序
    u_int8_t   ip_version : 4,
    ip_header_length : 4;
#else
    u_int8_t   ip_header_length : 4,//首部长度
    ip_version : 4;//版本
#endif
    u_int8_t    ip_tos;//服务类型
    u_int16_t   ip_length;//总长度
    u_int16_t   ip_id;//标识
    u_int16_t   ip_off;//片偏移
    u_int8_t    ip_ttl;//生存时间
    u_int8_t    ip_protocol;//协议
    u_int16_t   ip_checksum;//首部检验和
    struct in_addr ip_souce_address;//源地址
    struct in_addr ip_destination_address;//目的地址
};

class Network : public QThread
{
    Q_OBJECT
public:
    explicit Network(QObject *parent = nullptr);
    Network(NetAdaption*);
    ~Network();

public slots: 
    void init_dev();
    void startCraw(QTableWidget*, QStatusBar*, QMap<QString, QString>, bool);

protected:
    void run();

private:
    NetAdaption *p_netadaption = nullptr;
    pcap_t *p_fp = nullptr;
    bool craw_flag = false;
    QTableWidget *p_table = nullptr;
    QStatusBar *p_statusbar = nullptr;
    QMap<QString, QString> p_params;

    QStringList handle_ip_protocol(u_char*);
    bool filtered_pkg(QStringList);

};

#endif // NETWORK_H
