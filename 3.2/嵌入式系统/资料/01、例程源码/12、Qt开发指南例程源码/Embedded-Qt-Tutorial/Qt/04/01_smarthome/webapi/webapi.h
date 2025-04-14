/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   webapi
* @brief         webapi.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-27
*******************************************************************/
#ifndef WEBAPI_H
#define WEBAPI_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebSocket>
#include <unistd.h>
#include <QTimer>

class Webapi : public QObject
{
    Q_OBJECT

public:
    Webapi(QObject *parent = nullptr);
    ~Webapi();

private:

    QTimer *timer;
    QString api_token;
    QString random_token;

    void sleep(double second);

    QVector<QString>groupID;
    QVector<QString>deviceID;
    QVector<QString>deviceNumber;
    QVector<QString>deviceName;
    QVector<QString>deviceConnectState;

    // HTTPS
    QNetworkAccessManager *networkAccessManager;

    /* 获取账号机构列表 */
    void getOrgURL();

    /* 获取分组列表 */
    void getGroupListUrl();

    /* 获取分组列表下的设备 */
    void getDevOfGroupUrl();

    /* 获取设备的连接状态 */
    void getConStateUrl();

    /* 返回的字符串信息 */
    QString dataString;

    /* 账号机构列表id */
    QString org_id;

    /* 分组id */
    QString group_id;

    /* 设备id */
    QString device_id;

    /* 设备编号 */
    QString device_number;

    /* 设备名字（非必须，设计时为了匹配控制面板里的控制项）*/
    QString device_name;

    /* 原子云连接URL */
    QString orgURL;

    /* 设备分组URL*/
    QString groupListUrl;

    /* 分组下的URL */
    QString devOfGroupUrl;

    /* 连接状态URL */
    QString conStateUrl;

    /* 网络返回的数据 */
    void getDataFromWeb(QUrl);

    /* 获取id */
    void getID(QString, QNetworkReply *reply);

    /* 获取设备编号 */
    void getNumber(QString data);

    /* 获取设备名字 */
    void getName(QString data);

    /* 获取设备的连接状态 */
    void getConnectState(QString data);

    // WebSocket
    QWebSocket *webSocket;

    /* 发送命令/发送命令+消息 */
    void sendCmd(QString number, QByteArray cmd);
    void sendCmdMessage(QString number, QByteArray cmd, QString message);

private slots:

    void onTimerTimeOut();
    // HTTPS
    void replyFinished();
    void readyReadData();

    // WebSocket
    void webSocketConnected();
    void onBinaryMessageReceived(QByteArray);

public:
    void whichDeviceNameSendCmd(QString name, QString message);

signals:
    void deviceStateChanged(QString deviceState);
};
#endif // WEBAPI_H
