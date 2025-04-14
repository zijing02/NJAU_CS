/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   webapi
* @brief         webapi.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-27
*******************************************************************/
#include "webapi.h"
#include <QUuid>
#include <QRegularExpression>

Webapi::Webapi(QObject *parent)
{
    this->setParent(parent);
    /* 数组清空 */
    groupID.clear();
    deviceID.clear();
    deviceNumber.clear();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));

    networkAccessManager = new QNetworkAccessManager(this);

    orgURL = "https://cloud.alientek.com/api/orgs";
    /* 请填写自己的token信息！！！ */
    api_token = "bf591984c8fa417584d18f6328e0ef73";

    /* 获取账号机构列表 */
    getOrgURL();

    QUuid uuid = QUuid::createUuid();
    random_token = uuid.toString();

    webSocket = new QWebSocket();
    /* 需要加一些安全配置才能访问https */
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    webSocket->setSslConfiguration(config);

    connect(webSocket, SIGNAL(connected()),
            this, SLOT(webSocketConnected()));
    connect(webSocket, SIGNAL(binaryMessageReceived(QByteArray)),
            this, SLOT(onBinaryMessageReceived(QByteArray)));
}

Webapi::~Webapi()
{
    delete timer;
    delete webSocket;
    webSocket = nullptr;
}

void Webapi::getOrgURL()
{
    getDataFromWeb(QUrl(orgURL));
}

/* 获取设备分组列表 */
void Webapi::getGroupListUrl()
{
    getDataFromWeb(QUrl(groupListUrl));
}

/* 获取设备的信息 */
void Webapi::getDevOfGroupUrl()
{
    getDataFromWeb(QUrl(devOfGroupUrl));
}

/* 获取设备连接状态  */
void Webapi::getConStateUrl()
{
    getDataFromWeb(QUrl(conStateUrl));
}

/* 从云服务器获取数据 */
void Webapi::getDataFromWeb(QUrl url)
{
    /* 网络请求 */
    QNetworkRequest networkRequest;

    /* 需要加一些安全配置才能访问https */
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    networkRequest.setSslConfiguration(config);

    /* 设置访问的地址 */
    networkRequest.setUrl(url);

    /* 网络响应 */
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/json;charset=UTF-8");

    /* 参数二为原子云帐号的token信息,填写自己的 */
    networkRequest.setRawHeader("token", api_token.toLatin1());

    QNetworkReply *newReply =
            networkAccessManager->get(networkRequest);

    connect(newReply, SIGNAL(finished()),
            this, SLOT(replyFinished()));
    connect(newReply, SIGNAL(readyRead()),
            this, SLOT(readyReadData()));

}
void Webapi::replyFinished()
{
    QNetworkReply *reply = (QNetworkReply *)sender();

    if (reply->url() == QUrl(orgURL)) {
        /* 设备分组列表ID */
        getID(dataString, reply);
    }

    if (reply->url() == QUrl(groupListUrl)) {
        /* 列表ID */
        getID(dataString, reply);

        /* 获取到组ID再开启定时器 */
        if (!timer->isActive())
            timer->start(2000);
    }

    /* 设备的信息 */
    if (reply->url() == QUrl(devOfGroupUrl)) {
        getID(dataString, reply);
        getNumber(dataString);
        getName(dataString);
    }

    /* 设备的连接状态 */
    if (reply->url() == QUrl(conStateUrl)) {
        getConnectState(dataString);
    }

    reply->deleteLater();
    reply = nullptr;
}
void Webapi::readyReadData()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QByteArray data = reply->readAll();
    dataString =  QString(data);
    qDebug()<<dataString<<endl;
}

/* 获取ID，包括分组id，设备id */
void Webapi::getID(QString data, QNetworkReply *reply)
{
    /* 多个匹配，因为可能有多个合适的字段 */
    QRegularExpression pattern("\"id\":(\\d+)");

    QRegularExpressionMatchIterator i = pattern.globalMatch(data);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            if (reply->url() == QUrl(orgURL)) {
                org_id =  match.captured(1);
                groupListUrl = "https://cloud.alientek.com/api/orgs/"
                        + org_id + "/grouplist";
                getGroupListUrl();
                /* Socket连接 */
                webSocket->open(QUrl(QString("wss://cloud.alientek.com/connection/%1/org/%2?token=%3")
                                     .arg(api_token).arg(org_id).arg(random_token)));
            }

            if (reply->url() == QUrl(groupListUrl)) {
                group_id = match.captured(1);
                /* 存储组ID，再由定时器根据组的ID获取设备信息 */
                groupID.append(group_id);
                qDebug()<<"组ID:"<<group_id<<endl;

            }

            if (reply->url() == QUrl(devOfGroupUrl)) {
                device_id = match.captured(1);
                /* 存储设备ID,再由定时器根据设备的ID获取连接状态 */
                deviceID.append(device_id);
                qDebug()<<"设备ID:"<<device_id<<endl;
            }
        }
    }
}

void Webapi::getNumber(QString data)
{
    QRegularExpression pattern("\"number\":\"(\\d+)\"");

    QRegularExpressionMatchIterator i = pattern.globalMatch(data);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            device_number = match.captured(1);
            deviceNumber.append(device_number);
            qDebug()<<"设备编号："<<device_number<<endl;
        }
    }
}

void Webapi::getName(QString data)
{
    /* 匹配中文字符，设备起名需要为中文 */
    QRegularExpression pattern("\"name\":\"([\u4e00-\u9fa5]*)");

    QRegularExpressionMatchIterator i = pattern.globalMatch(data);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            device_name = match.captured(1);
            deviceName.append(device_name);
            qDebug()<<"设备名称："<<device_name<<endl;
        }
    }
}

/* 获取设备的连接状态 */
void Webapi::getConnectState(QString data)
{
    QString pattern = "\"data\":\"(\\S*)\"";
    QRegularExpression regularExpression(pattern);
    QRegularExpressionMatch  match = regularExpression.match(data, 0);
    if(match.hasMatch()) {
        qDebug()<<"设备连接状态"<<match.captured(1);
        deviceConnectState.append(match.captured(1));
    }
}

void Webapi::webSocketConnected()
{
    qDebug()<<"webSocket连接原子云成功"<<endl;
}

void Webapi::onBinaryMessageReceived(QByteArray str)
{

    QString temp(str);
    if (temp.contains("online")) {
        for (int i = 0; i < deviceNumber.count() ; i++) {
            if (temp.contains(deviceNumber[i])) {
                /* 发送如客厅灯在线信号*/
                emit deviceStateChanged(deviceName[i] + "|在线");
                qDebug()<<deviceName[i] + "|在线"<<endl;
                break;
            }
        }
    }
}

/* 延时函数 */
void Webapi::sleep(double second)
{
    usleep(second * 1000000);
}

void Webapi::onTimerTimeOut()
{
    static int i = 0;
    if (i < groupID.count()) {
        /* 获取分组下的设备列表 */
        devOfGroupUrl = "https://cloud.alientek.com/api/orgs/"
                + org_id + "/groups/"
                + groupID[i] + "/devices";
        dataString.clear();
        getDevOfGroupUrl();
    } else if (i >= groupID.count()
               && i < groupID.count() + deviceID.count() ) {
        timer->start(1000);
        conStateUrl = "https://cloud.alientek.com/api/orgs/"
                + org_id + "/devicestate/"
                + deviceID[i - groupID.count()];
        getConStateUrl();

    } else {
        /* 订阅设备的消息 */
        for (int j = 0; j < deviceNumber.count(); j++) {
            QByteArray cmd;
            cmd[0] = 0x01;
            sendCmd(deviceNumber[j], cmd);
        }

        timer->stop();
    }

    i++;
}

/* 订阅指定设备的消息，cmd = 0x01 */
void Webapi::sendCmd(QString number, QByteArray cmd)
{
    QStringList list = number.split("");
    for (int i = 0; i < list.count(); i++) {
        if (!list[i].isEmpty()) {
            cmd.append(list[i]);
        }
    }

    webSocket->sendBinaryMessage(cmd);
}

/* 发送消息到指定设备，cmd = 0x03 */
void Webapi::sendCmdMessage(QString number,
                            QByteArray cmd, QString message)
{
    QStringList list = number.split("");
    for (int i = 0; i < list.count(); i++) {
        if (!list[i].isEmpty()) {
            cmd.append(list[i]);
        }
    }

    cmd.append(message);

    webSocket->sendBinaryMessage(cmd);
}

void Webapi::whichDeviceNameSendCmd(QString name,
                                    QString message) {

    for (int i = 0; i < deviceName.count(); i++) {
        if (name ==  deviceName[i]) {
            QByteArray cmd;
            cmd[0] = 0x03;
            sendCmdMessage(deviceNumber[i], cmd, message);
            break;
        }
    }
}
