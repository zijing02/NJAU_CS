/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   tcpserver
* @brief         mytcpserver.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-25
*******************************************************************/
#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class myTcpserver;
}

class myTcpserver : public QObject
{
    Q_OBJECT

public:
    explicit myTcpserver(QObject *parent = 0);
    ~myTcpserver();

    QString recMessage();
    QString newConnet();
    void setRecMessage(const QString str);
    Q_PROPERTY(QString recMessage READ recMessage WRITE setRecMessage NOTIFY recMessageChanged)
    Q_PROPERTY(QString newConnet READ newConnet NOTIFY newConnetChanged)
    Q_INVOKABLE QString getLocalIpAddress();
    Q_INVOKABLE void sendMessage(QString str);
    Q_INVOKABLE void disConnect();

private slots:
    void slot_tcp_connect();
    void slot_tcp_readData();

private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString message;
    QString newConnetInfo;

signals:
    void recMessageChanged();
    void newConnetChanged();
};

#endif // MYTCPSERVER_H
