/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   tcpclient
* @brief         mytcpclient.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-31
*******************************************************************/
#ifndef MYTCPCLIENT_H
#define MYTCPCLIENT_H

#include <QTcpSocket>

namespace Ui {
class myTcpclient;
}

class myTcpclient : public QObject
{
    Q_OBJECT

public:
    explicit myTcpclient(QObject *parent = 0);
    ~myTcpclient();

    QString recMessage();
    QString newConnet();
    void setRecMessage(const QString str);
    Q_PROPERTY(QString recMessage READ recMessage WRITE setRecMessage NOTIFY recMessageChanged)
    Q_PROPERTY(QString newConnet READ newConnet NOTIFY newConnetChanged)
    Q_INVOKABLE QString getLocalIpAddress();
    Q_INVOKABLE void sendMessage(QString str);
    Q_INVOKABLE void disConnect();
    Q_INVOKABLE void buttonConnectClicked(QString str1, QString str2);

private slots:
    void slot_tcp_connect();
    void slot_tcp_readData();

private:
    QTcpSocket *tcpSocket;
    QString message;
    QString newConnetInfo;

signals:
    void recMessageChanged();
    void newConnetChanged();
};

#endif // MYTCPCLIENT_H
