/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   tcpclient
* @brief         mytcpclient.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-31
*******************************************************************/
#include "mytcpclient.h"
#include <QNetworkInterface>
myTcpclient::myTcpclient(QObject *parent) : QObject (parent)
{
    tcpSocket = NULL;
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket,SIGNAL(connected()),this,SLOT(slot_tcp_connect()));
}
void myTcpclient::slot_tcp_connect()
{
    emit newConnetChanged();
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(slot_tcp_readData()));
}
void myTcpclient::slot_tcp_readData()
{
    message = tcpSocket->readAll();
    emit recMessageChanged();
}

void myTcpclient::buttonConnectClicked(QString str1, QString str2)
{
    QString ip = str1;
    qint16 port = str2.toInt();
    newConnetInfo = "已经与小胖子IP:" + str1 + "端口:" + str2 + "成功连接";

    if (tcpSocket->state() != tcpSocket->ConnectedState)
        tcpSocket->connectToHost(QHostAddress(ip),port);
}

void myTcpclient::setRecMessage(const QString str)
{
    message = str;
}

QString myTcpclient::recMessage()
{
    return message;
}

QString myTcpclient::newConnet()
{
    return newConnetInfo;
}

myTcpclient::~myTcpclient()
{

}

void myTcpclient::sendMessage(QString str)
{
    if(NULL == tcpSocket)
        return;

    if(tcpSocket->state() == tcpSocket->ConnectedState)
        tcpSocket->write( str.toUtf8().data());

}

void myTcpclient::disConnect()
{
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}

QString myTcpclient::getLocalIpAddress()
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

