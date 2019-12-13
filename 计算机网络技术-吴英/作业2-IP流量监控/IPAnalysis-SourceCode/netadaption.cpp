#include "netadaption.h"

NetAdaption::NetAdaption()
{
}

NetAdaption::~NetAdaption()
{
    if (this->p_devs != nullptr)
        pcap_freealldevs(this->p_devs);
}

void NetAdaption::findAllDevs(QComboBox *combox)
{
    combox->clear();
    pcap_if_t *alldevs;
    pcap_if_t *d;
    int i = 0;

    char errbuf[PCAP_ERRBUF_SIZE];
    if(pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        qDebug() << errbuf;
    }
    for(d = alldevs; d; d = d->next)
    {
        i++;
        combox->addItem(d->name);
    }
    if(0 == i)
    {
        qDebug("No interfaces found! Make sure WinPcap is installed.");
    }

    this->p_devs = alldevs;
}

void NetAdaption::selectDev(QString devname)
{
    pcap_if_t *d;

    for (d = this->p_devs; d; d = d->next)
    {
        if (d->name == devname)
            this->p_dev = d;
    }
    qDebug()<<"选择设备成功";
}

pcap_t* NetAdaption::OpenDev()
{
    u_int netmask;
    struct bpf_program fcode;
    pcap_t *fp = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];

    if (this->p_dev != nullptr)
    {
        if ((fp = pcap_open(this->p_dev->name,
            100 /*snaplen*/,
            PCAP_OPENFLAG_PROMISCUOUS /*flags*/,
            20 /*read timeout*/,
            nullptr /* remote authentication */,
            errbuf)
            ) == nullptr)
        {
            QMessageBox::information(nullptr, "警告", "打开网卡失败，请检查Winpcap驱动.");
        } else {
            if (this->p_dev->addresses != nullptr)
                netmask = reinterpret_cast<struct sockaddr_in *>(this->p_dev->addresses->netmask)->sin_addr.S_un.S_addr;
            else
                netmask = 0xffffff;

            //编译过滤表达式
            if (pcap_compile(fp, &fcode, "ip or arp or icmp", 1, netmask) < 0)
            {
                qDebug()<<"编译过滤表达式失败";
            }

            //绑定过滤器
            if (pcap_setfilter(fp, &fcode)<0)
            {
                qDebug()<<"绑定过滤器失败";
            }
        }
    }
    else
    {
        QMessageBox::information(nullptr, "警告", "网卡设备无法获取，请检查Winpcap驱动.");
    }

    return fp;
}
