/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   udpchat
* @brief         udpreciver.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-20
*******************************************************************/
#ifndef UDPRECIVER_H
#define UDPRECIVER_H

#include <QUdpSocket>
class UdpReciver : public QObject
{
    Q_OBJECT
public:
    UdpReciver(QObject *parent = NULL);
    ~UdpReciver();
    void init_port(QString tmphost, int tmport);
    void setRecMessage(const QString str);
    QString recMessage();
    Q_PROPERTY(QString recMessage READ recMessage WRITE setRecMessage NOTIFY recMessageChanged)
signals:
    void deliverInfo(QByteArray info, QString clientip);
    void recMessageChanged();
public slots:
    void readDatagrams(); //listen UDP data


private:
    QUdpSocket *m_udpSocket;
    QString m_localhost;
    int m_port;
    QByteArray m_data;
    QThread *m_thread;
};

#endif // UDPRECIVER_H
