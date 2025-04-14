/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   udpchat
* @brief         udpsender.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-20
*******************************************************************/
#ifndef UDPSEND_H
#define UDPSEND_H
#include <QUdpSocket>
namespace Ui {
class UdpSender;
}

class UdpSender : public QObject
{
    Q_OBJECT
public:
    UdpSender();
    ~UdpSender();
    void initSender(QString desHost, int port);
    Q_INVOKABLE void send(QByteArray msg);

    Q_INVOKABLE QString getLocalIpAddress();
    Q_INVOKABLE void setUdpReciverInfo(QString desHost, QString port);

public slots:
    void recMsg(QByteArray msgInfo);
private:
    QUdpSocket *m_Socket = NULL;
    QString m_address;
    int m_port;
};
#endif
