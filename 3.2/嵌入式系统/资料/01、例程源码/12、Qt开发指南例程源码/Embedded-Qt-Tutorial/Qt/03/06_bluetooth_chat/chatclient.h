/******************************************************************
Copyright (C) 2015 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         chatclient.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-20
*******************************************************************/
#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <qbluetoothserviceinfo.h>
#include <QBluetoothSocket>
#include <QtCore/QObject>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

class ChatClient : public QObject
{
    Q_OBJECT

public:
    explicit ChatClient(QObject *parent = nullptr);
    ~ChatClient();

    /* 开启客户端 */
    void startClient(const QBluetoothServiceInfo &remoteService);

    /* 停止客户端 */
    void stopClient();

public slots:
    /* 发送消息 */
    void sendMessage(const QString &message);

    /* 主动断开连接 */
    void disconnect();

signals:
    /* 接收到消息信号 */
    void messageReceived(const QString &sender, const QString &message);

    /* 连接信号 */
    void connected(const QString &name);

    /* 断开连接信号 */
    void disconnected();

private slots:
    /* 从socket里读取消息 */
    void readSocket();

    /* 连接 */
    void connected();

private:
    /* socket通信 */
    QBluetoothSocket *socket;
};

#endif // CHATCLIENT_H
