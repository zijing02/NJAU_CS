/******************************************************************
Copyright (C) 2015 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         chatserver.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-20
*******************************************************************/

#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <qbluetoothserviceinfo.h>
#include <qbluetoothaddress.h>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QBluetoothServer>
#include <QBluetoothSocket>


class ChatServer : public QObject
{
    Q_OBJECT

public:
    explicit ChatServer(QObject *parent = nullptr);
    ~ChatServer();

    /* 开启服务端 */
    void startServer(const QBluetoothAddress &localAdapter = QBluetoothAddress());

    /* 停止服务端 */
    void stopServer();

public slots:
    /* 发送消息 */
    void sendMessage(const QString &message);

    /* 服务端主动断开连接 */
    void disconnect();

signals:
    /* 接收到消息信号 */
    void messageReceived(const QString &sender, const QString &message);

    /* 客户端连接信号 */
    void clientConnected(const QString &name);

    /* 客户端断开连接信号 */
    void clientDisconnected(const QString &name);

private slots:

    /* 客户端连接 */
    void clientConnected();

    /* 客户端断开连接 */
    void clientDisconnected();

    /* 读socket */
    void readSocket();

private:
    /* 使用rfcomm协议 */
    QBluetoothServer *rfcommServer;

    /* 服务器蓝牙信息 */
    QBluetoothServiceInfo serviceInfo;

    /* 用于保存客户端socket */
    QList<QBluetoothSocket *> clientSockets;

    /* 用于保存客户端的名字 */
    QList<QString> socketsPeername;
};

#endif // CHATSERVER_H
