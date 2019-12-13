#include "network.h"

Network::Network(QObject *parent) : QThread(parent)
{

}

Network::Network(NetAdaption *netadaption)
{
    this->p_netadaption = netadaption;
}

Network::~Network()
{
    this->craw_flag = false;
}

void Network::init_dev()
{
    pcap_t *fp = nullptr;

    if ((fp = this->p_netadaption->OpenDev()) == nullptr)
    {
        qDebug()<<"开启网卡失败";
    }
    else
    {
        this->p_fp = fp;
        qDebug()<<"开启网卡成功";
    }
}

bool Network::filtered_pkg(QStringList pkg)
{
    bool flag = true;

    if ((this->p_params["ptc"] != "*")
            && (this->p_params["ptc"] != pkg[1]))
        flag = false;

    if ((this->p_params["src"].trimmed() != "")
            && (this->p_params["src"].trimmed() != pkg[2]))
        flag = false;

    if ((this->p_params["tge"].trimmed() != "")
            && (this->p_params["tge"].trimmed() != pkg[3]))
        flag = false;

    return flag;
}

void Network::run()
{
    int res;
    int count = 0;
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    u_int ethernet_type;

    while (this->craw_flag && ((res = pcap_next_ex(this->p_fp, &header, &pkt_data)) >= 0))
    {
        if (res == 0)
            continue;

        struct EtherHeader *ethernet_protocol = reinterpret_cast<struct EtherHeader*>(const_cast<u_char*>(pkt_data));//获得数据包内容
        ethernet_type = ntohs(ethernet_protocol->ether_type);//获得以太网类型
        switch (ethernet_type){
            case 0x0800:
                // 添加一项记录
                QStringList result = this->handle_ip_protocol(const_cast<u_char*>(pkt_data));
                if (this->filtered_pkg(result))
                {
                    int rows = this->p_table->rowCount();
                    this->p_table->insertRow(rows);
                    for(int i = 0; i < result.size(); i++)
                    {
                        this->p_table->setItem(rows, i, new QTableWidgetItem(result[i]));
                    }
                    this->p_table->scrollToBottom();

                    // 更新状态栏
                    count ++;
                    this->p_statusbar->showMessage(QString("已抓取 %1 个包").arg(count));
                }
            break;
        }
    }
}

void Network::startCraw(QTableWidget *tableWidget, QStatusBar *statusBar, QMap<QString, QString> params, bool state)
{
    if (this->p_table == nullptr)
        this->p_table = tableWidget;
    if (this->p_statusbar == nullptr)
        this->p_statusbar = statusBar;
    this->craw_flag = state;
    this->p_params = params;

    if (state)
    {
        this->p_table->clearContents();
        this->p_table->setRowCount(0);
    }
    else
    {
        this->p_statusbar->showMessage("");
    }
    this->start();
}

QStringList Network::handle_ip_protocol(u_char *pkt_data)
{
    QString strBuffer;
    QDateTime time;
    time = QDateTime::currentDateTime();
    strBuffer = time.toString("yyyy-MM-dd hh:mm:ss");

    struct IPHeader *ip_protocol = nullptr;
    QString protocol;

    ip_protocol = reinterpret_cast<struct IPHeader *>(pkt_data + 14);//以太网首部14位

//    qDebug()<<"版本号:%d\n"<<ip_protocol->ip_version;
//    qDebug()<<"首部长度:%d\n"<<ip_protocol->ip_header_length;
//    qDebug()<<"服务质量:%d\n"<<tos;
//    qDebug()<<"总长度:%d\n"<<ntohs(ip_protocol->ip_length);
//    qDebug()<<"标识:%d\n"<<ntohs(ip_protocol->ip_id);
//    qDebug()<<"偏移:%d\n"<<(offset & 0x1fff) * 8;
//    qDebug()<<"生存时间:%d\n"<<ip_protocol->ip_ttl;
//    qDebug()<<"协议类型:%d"<<ip_protocol->ip_protocol;
    switch (ip_protocol->ip_protocol)
    {
        case 1: protocol = "ICMP"; break;
        case 2: protocol = "IGMP"; break;
        case 6: protocol = "TCP"; break;
        case 17: protocol = "UDP"; break;
        default:break;
    }

    return QStringList()<<QString::number(ip_protocol->ip_version)<<protocol
                        <<QString(inet_ntoa(ip_protocol->ip_souce_address))
                        <<QString(inet_ntoa(ip_protocol->ip_destination_address))
                        <<strBuffer;
}
