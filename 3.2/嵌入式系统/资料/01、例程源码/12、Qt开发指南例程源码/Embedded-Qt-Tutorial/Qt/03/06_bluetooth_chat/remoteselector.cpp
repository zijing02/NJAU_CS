/******************************************************************
Copyright (C) 2015 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         remoteselector.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-20
*******************************************************************/
#include "remoteselector.h"

/* 初始化本地蓝牙 */
RemoteSelector::RemoteSelector(QBluetoothAddress &localAdapter, QObject *parent)
    : QObject(parent)
{
    m_discoveryAgent = new QBluetoothServiceDiscoveryAgent(localAdapter);

    connect(m_discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(serviceDiscovered(QBluetoothServiceInfo)));
    connect(m_discoveryAgent, SIGNAL(finished()), this, SLOT(discoveryFinished()));
    connect(m_discoveryAgent, SIGNAL(canceled()), this, SLOT(discoveryFinished()));
}

RemoteSelector::~RemoteSelector()
{
    delete m_discoveryAgent;
}

/* 开启发现模式，这里无需设置过滤uuid，否则搜索不到手机
 * uuid会过滤符合条件的uuid服务都会返回相应的蓝牙设备
 */
void RemoteSelector::startDiscovery(const QBluetoothUuid &uuid)
{
    Q_UNUSED(uuid);
    qDebug()<<"startDiscovery";
    if (m_discoveryAgent->isActive()) {
        qDebug()<<"stop the searching first";
        m_discoveryAgent->stop();
    }

    //m_discoveryAgent->setUuidFilter(uuid);
    m_discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
}

/* 停止发现 */
void RemoteSelector::stopDiscovery()
{
    qDebug()<<"stopDiscovery";
    if (m_discoveryAgent){
        m_discoveryAgent->stop();
    }
}

QBluetoothServiceInfo RemoteSelector::service() const
{
    return m_serviceInfo;
}

/* 扫描蓝牙服务信息 */
void RemoteSelector::serviceDiscovered(const QBluetoothServiceInfo &serviceInfo)
{
#if 0
    qDebug() << "Discovered service on"
             << serviceInfo.device().name() << serviceInfo.device().address().toString();
    qDebug() << "\tService name:" << serviceInfo.serviceName();
    qDebug() << "\tDescription:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceDescription).toString();
    qDebug() << "\tProvider:"
             << serviceInfo.attribute(QBluetoothServiceInfo::ServiceProvider).toString();
    qDebug() << "\tL2CAP protocol service multiplexer:"
             << serviceInfo.protocolServiceMultiplexer();
    qDebug() << "\tRFCOMM server channel:" << serviceInfo.serverChannel();
#endif

    QMapIterator<QString, QBluetoothServiceInfo> i(m_discoveredServices);
    while (i.hasNext()){
        i.next();
        if (serviceInfo.device().address() == i.value().device().address()){
            return;
        }
    }

    QString remoteName;
    if (serviceInfo.device().name().isEmpty())
        remoteName = serviceInfo.device().address().toString();
    else
        remoteName = serviceInfo.device().name();

    qDebug()<<"adding to the list....";
    qDebug()<<"remoteName: "<< remoteName;
    QListWidgetItem *item =
            new QListWidgetItem(QString::fromLatin1("%1%2")
                                .arg(remoteName, serviceInfo.serviceName()));
    m_discoveredServices.insert(remoteName, serviceInfo);
    emit newServiceFound(item);
}

/* 发现完成 */
void RemoteSelector::discoveryFinished()
{
    qDebug()<<"discoveryFinished";
    emit finished();
}

