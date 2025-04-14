/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   tcpserver
* @brief         mytcpserver.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-25
*******************************************************************/
#include "mytcpserver.h"
#include <QNetworkInterface>
myTcpserver::myTcpserver(QObject *parent) : QObject (parent)
{
    tcpServer = new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,888);
    tcpSocket = new QTcpSocket(this);

    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(slot_tcp_connect()));
}
void myTcpserver::slot_tcp_connect()
{
    tcpSocket = tcpServer->nextPendingConnection();
    QString ip = tcpSocket->peerAddress().toString();
    qint16 port = tcpSocket->peerPort();
    newConnetInfo = QString("%1:端口:%2成功连接").arg(ip).arg(port);
    newConnetInfo = "已经与小可爱IP:" + newConnetInfo.mid(7,-1);
    emit newConnetChanged();

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(slot_tcp_readData()));
}
void myTcpserver::slot_tcp_readData()
{
    message = tcpSocket->readAll();
    emit recMessageChanged();
}

void myTcpserver::setRecMessage(const QString str)
{
    message = str;
}

QString myTcpserver::recMessage()
{
    return message;
}

QString myTcpserver::newConnet()
{
    return newConnetInfo;
}
myTcpserver::~myTcpserver()
{

}

void myTcpserver::sendMessage(QString str)
{
    if(NULL == tcpSocket)
        return;

    if(tcpSocket->state() == tcpSocket->ConnectedState)
        tcpSocket->write( str.toUtf8().data());

}

void myTcpserver::disConnect()
{
    if(NULL == tcpSocket)
        return;

    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket == NULL;
}

QString myTcpserver::getLocalIpAddress()
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
                    //str+= "子网掩码:" +  entry.netmask().toString() + "\n";
                    //str+= "广播地址:" +  entry.broadcast().toString() + "\n\n";
                }
            }
        }
    }
    return str;
}

