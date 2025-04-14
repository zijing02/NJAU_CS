/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   udpchat
* @brief         udpsend.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-20
*******************************************************************/
#include "udpsender.h"
#include <QNetworkInterface>
UdpSender::UdpSender()
{
    m_Socket = new QUdpSocket();
}
UdpSender::~UdpSender()
{
    if (m_Socket != NULL){
        delete m_Socket;
        m_Socket = NULL;
    }
}

void UdpSender::send(QByteArray msg)
{
    m_Socket->writeDatagram(msg, QHostAddress(m_address), m_port);
}

void UdpSender::recMsg(QByteArray msgInfo)
{
    //send(msgInfo);
}

QString UdpSender::getLocalIpAddress()
{
    QString str;
    QList<QNetworkInterface>list = QNetworkInterface::allInterfaces();
    foreach(QNetworkInterface interface,list){
        if (interface.hardwareAddress() =="" || interface.name().contains("tunnel_")){
            continue;
        }else{
            QList<QNetworkAddressEntry> entryList = interface.addressEntries();
            foreach(QNetworkAddressEntry entry,entryList){
                if (entry.broadcast().toString()=="" || entry.ip().toString().contains("127.0.")){
                    continue;
                } else {
                    str+= "网卡设备:" +  interface.name() + "\n";
                    str+= "IP 地址:" +  entry.ip().toString() + "\n\n";
                    // str+= "子网掩码:" +  entry.netmask().toString() + "\n";
                    // str+= "广播地址:" +  entry.broadcast().toString() + "\n\n";
                }

            }
        }
    }
    return str;
}

void UdpSender::setUdpReciverInfo(QString desHost, QString port)
{
    m_address = desHost;
    m_port = port.toInt();
}
