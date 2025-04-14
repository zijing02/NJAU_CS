/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   udpchat
* @brief         udpreciver.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-20
*******************************************************************/
#include "udpreciver.h"
#include "QThread"
UdpReciver::UdpReciver(QObject *parent) : QObject(parent)
{
    m_thread = new QThread();
    m_udpSocket = new QUdpSocket();
    QString localhost = "";
    init_port(localhost, 666);
    connect(m_udpSocket, SIGNAL(readyRead()), this, SLOT(readDatagrams()), Qt::DirectConnection);
    //this->moveToThread(m_thread);
    m_udpSocket->moveToThread(m_thread);
    m_thread->start();
}
UdpReciver::~UdpReciver()
{

}

void UdpReciver::init_port(QString tmphost, int tmport)
{
    m_port = tmport;
    m_localhost = tmphost;
    m_udpSocket->bind(QHostAddress(m_localhost), m_port);
}

void UdpReciver::readDatagrams()
{   //client ip addr
    QHostAddress client_address;
    m_data.clear();
    QString strclient_address;
    while(m_udpSocket->hasPendingDatagrams())
    {
        m_data.resize(m_udpSocket->pendingDatagramSize());
        m_udpSocket->readDatagram(m_data.data(), m_data.size(), &client_address);
        strclient_address = client_address.toString();
        deliverInfo(m_data, strclient_address);
        //qDebug() << "receive UDP's size:" << m_data.size()<<m_data;
    }

    m_data = strclient_address.toLatin1() + ":" + m_data;
    emit recMessageChanged();
}

QString UdpReciver::recMessage()
{
    return m_data;
}

void UdpReciver::setRecMessage(const QString str)
{

}
