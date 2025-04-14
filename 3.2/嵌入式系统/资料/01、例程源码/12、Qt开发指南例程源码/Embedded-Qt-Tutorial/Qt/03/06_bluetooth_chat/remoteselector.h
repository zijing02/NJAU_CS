/******************************************************************
Copyright (C) 2015 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         remoteselector.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-20
*******************************************************************/

#ifndef REMOTESELECTOR_H
#define REMOTESELECTOR_H

#include <qbluetoothuuid.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothservicediscoveryagent.h>
#include <QListWidgetItem>

/* 声明一个蓝牙适配器类 */
class RemoteSelector : public QObject
{
    Q_OBJECT

public:
    explicit RemoteSelector(QBluetoothAddress&,
                            QObject *parent = nullptr);
    ~RemoteSelector();

    /* 开启发现蓝牙 */
    void startDiscovery(const QBluetoothUuid &uuid);

    /* 停止发现蓝牙 */
    void stopDiscovery();

    /* 蓝牙服务 */
    QBluetoothServiceInfo service() const;

signals:
    /* 找到新服务 */
    void newServiceFound(QListWidgetItem*);

    /* 完成 */
    void finished();

private:
    /* 蓝牙服务代理，用于发现蓝牙服务 */
    QBluetoothServiceDiscoveryAgent *m_discoveryAgent;

    /* 服务信息 */
    QBluetoothServiceInfo m_serviceInfo;

private slots:
    /* 服务发现完成 */
    void serviceDiscovered(const QBluetoothServiceInfo &serviceInfo);

    /* 蓝牙发现完成 */
    void discoveryFinished();

public:
    /* 键值类容器 */
    QMap<QString, QBluetoothServiceInfo> m_discoveredServices;
};

#endif // REMOTESELECTOR_H
