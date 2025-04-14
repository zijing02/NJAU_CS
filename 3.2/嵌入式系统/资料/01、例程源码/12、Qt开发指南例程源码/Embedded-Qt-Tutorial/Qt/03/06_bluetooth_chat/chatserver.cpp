/******************************************************************
Copyright (C) 2015 The Qt Company Ltd.
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         chatserver.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-20
*******************************************************************/

#include "chatserver.h"

#include <qbluetoothserver.h>
#include <qbluetoothsocket.h>
#include <qbluetoothlocaldevice.h>

static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");
ChatServer::ChatServer(QObject *parent)
    :   QObject(parent), rfcommServer(0)
{
}

ChatServer::~ChatServer()
{
    stopServer();
}

/* 开启服务端，设置服务端使用rfcomm协议与serviceInfo的一些属性 */
void ChatServer::startServer(const QBluetoothAddress& localAdapter)
{
    if (rfcommServer)
        return;

    rfcommServer = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(rfcommServer, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    bool result = rfcommServer->listen(localAdapter);
    if (!result) {
        qWarning()<<"Cannot bind chat server to"<<localAdapter.toString();
        return;
    }

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceRecordHandle, (uint)0x00010010);

    QBluetoothServiceInfo::Sequence classId;

    classId<<QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::SerialPort));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,
                             classId);

    classId.prepend(QVariant::fromValue(QBluetoothUuid(serviceUuid)));

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList,classId);

    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, tr("Bt Chat Server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription,
                             tr("Example bluetooth chat server"));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceProvider, tr("qt-project.org"));

    serviceInfo.setServiceUuid(QBluetoothUuid(serviceUuid));

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse<< QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList,
                             publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol<< QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol<< QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::Rfcomm))
            << QVariant::fromValue(quint8(rfcommServer->serverPort()));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList,
                             protocolDescriptorList);

    serviceInfo.registerService(localAdapter);
}

/* 停止服务端 */
void ChatServer::stopServer()
{
    // Unregister service
    serviceInfo.unregisterService();

    // Close sockets
    qDeleteAll(clientSockets);

    // Close server
    delete rfcommServer;
    rfcommServer = 0;
}

/* 主动断开连接 */
void ChatServer::disconnect()
{
    qDebug()<<"Going to disconnect in server";

    foreach (QBluetoothSocket *socket, clientSockets) {
        qDebug()<<"sending data in server!";
        socket->close();
    }
}

/* 发送消息 */
void ChatServer::sendMessage(const QString &message)
{
    qDebug()<<"Going to send message in server: " << message;
    QByteArray text = message.toUtf8() + '\n';

    foreach (QBluetoothSocket *socket, clientSockets) {
        qDebug()<<"sending data in server!";
        socket->write(text);
    }
    qDebug()<<"server sending done!";
}

/* 客户端连接 */
void ChatServer::clientConnected()
{
    qDebug()<<"clientConnected";

    QBluetoothSocket *socket = rfcommServer->nextPendingConnection();
    if (!socket)
        return;

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    clientSockets.append(socket);
    socketsPeername.append(socket->peerName());
    emit clientConnected(socket->peerName());
}

/* 客户端断开连接 */
void ChatServer::clientDisconnected()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    if (clientSockets.count() != 0) {
        QString peerName;

        if (socket->peerName().isEmpty())
            peerName = socketsPeername.at(clientSockets.indexOf(socket));
        else
            peerName = socket->peerName();

        emit clientDisconnected(peerName);

        clientSockets.removeOne(socket);
        socketsPeername.removeOne(peerName);
    }

    socket->deleteLater();

}

/* 从Socket里读取数据 */
void ChatServer::readSocket()
{
    QBluetoothSocket *socket = qobject_cast<QBluetoothSocket *>(sender());
    if (!socket)
        return;

    while (socket->bytesAvailable()) {
        QByteArray line = socket->readLine().trimmed();
        qDebug()<<QString::fromUtf8(line.constData(), line.length())<<endl;
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
        qDebug()<<QString::fromUtf8(line.constData(), line.length())<<endl;
    }
}

