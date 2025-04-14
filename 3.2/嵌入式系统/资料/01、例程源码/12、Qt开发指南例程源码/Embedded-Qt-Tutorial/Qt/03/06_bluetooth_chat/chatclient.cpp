/******************************************************************
Copyright (C) 2015 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         chatclient.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-20
*******************************************************************/

#include "chatclient.h"
#include <qbluetoothsocket.h>

ChatClient::ChatClient(QObject *parent)
    :   QObject(parent), socket(0)
{
}

ChatClient::~ChatClient()
{
    stopClient();
}

/* 开启客户端 */
void ChatClient::startClient(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Create socket";
    socket->connectToService(remoteService);
    qDebug() << "ConnectToService done";

    connect(socket, SIGNAL(readyRead()),
            this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()),
            this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),
            this, SIGNAL(disconnected()));
}

/* 停止客户端 */
void ChatClient::stopClient()
{
    delete socket;
    socket = 0;
}

/* 从Socket读取消息 */
void ChatClient::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(),
                                               line.length()));
    }
}

/* 发送的消息 */
void ChatClient::sendMessage(const QString &message)
{
    qDebug()<<"Sending data in client: " + message;

    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);
}

/* 主动连接 */
void ChatClient::connected()
{
    emit connected(socket->peerName());
}

/* 主动断开连接*/
void ChatClient::disconnect() {
    qDebug()<<"Going to disconnect in client";
    if (socket) {
        qDebug()<<"diconnecting...";
        socket->close();
    }
}
