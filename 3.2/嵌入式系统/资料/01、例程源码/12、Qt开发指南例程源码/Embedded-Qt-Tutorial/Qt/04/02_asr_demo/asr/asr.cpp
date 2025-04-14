/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   asr
* @brief         asr.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-03
*******************************************************************/
#include "asr.h"
#include <QCoreApplication>

Asr::Asr(QWidget *parent)
    : QWidget(parent)
{
    networkAccessManager = new QNetworkAccessManager(this);

    QString fileName = QCoreApplication::applicationDirPath() + "/16k.wav";
    file.setFileName(fileName);

    if (file.exists())
        qDebug()<<fileName<<endl;

    tokenUrl = QString(token_org).arg(api_key).arg(secret_key);

    QByteArray requestData;
    requestData.clear();

    requestNetwork(tokenUrl, requestData);
}

Asr::~Asr()
{
}

/* 请求网络 */
void Asr::requestNetwork(QString url, QByteArray requestData)
{
    /* 网络请求 */
    QNetworkRequest networkRequest;

    /* 开发板需要加一些安全配置才能访问https */
    QSslConfiguration config;
    config.setPeerVerifyMode(QSslSocket::VerifyNone);
    config.setProtocol(QSsl::TlsV1SslV3);
    networkRequest.setSslConfiguration(config);

    /* 以json格式返回 */
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,
                             "application/json;charset=UTF-8");

    /* 设置pcm编码，采样率为16000 */
    networkRequest.setRawHeader("Content-Type", QString("audio/pcm;rate=16000").toLatin1());

    /* 设置访问的地址 */
    networkRequest.setUrl(url);

    /* 网络响应 */
    QNetworkReply *newReply = networkAccessManager->post(networkRequest, requestData);

    connect(newReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(newReply, SIGNAL(readyRead()), this, SLOT(readyReadData()));

}

/* 读取数据 */
void Asr::readyReadData()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QByteArray data = reply->readAll();

    if (reply->url() == QUrl(tokenUrl)) {

        qDebug()<<QString(data)<<endl;
        QString key = "access_token";

        QString temp = getJsonValue(data, key);
        accessToken = temp;

        qDebug()<<accessToken<<endl;
        qDebug()<<"获取token成功，可以调用getTheResult设置语音路径返回结果"<<endl;
    }

    if (reply->url() == QUrl(serverApiUrl)) {
        qDebug()<<QString(data)<<endl;
        QString key = "result";

        QString temp = getJsonValue(data, key);

        emit asrReadyData(temp);
        qDebug()<<temp<<endl;
    }

}

/* 请求完成处理，释放对象 */
void Asr::replyFinished()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    if (reply->url() == QUrl(tokenUrl) ) {
        //        QByteArray requestData;

        //        file.open(QIODevice::ReadOnly);
        //        requestData = file.readAll();
        //        file.close();

        //        serverApiUrl = QString(server_api).arg(QHostInfo::localHostName()).arg(accessToken);

        //        requestNetwork(serverApiUrl, requestData);
    }

    reply->deleteLater();
    reply = nullptr;
}

void Asr::getTheResult(QString fileName)
{
    file.setFileName(fileName);
    if (!file.exists()) {
        qDebug()<<"已返回，文件"<<fileName<<"不存在"<<endl;
        return;
    }

    QByteArray requestData;

    file.open(QIODevice::ReadOnly);
    requestData = file.readAll();
    file.close();

    serverApiUrl = QString(server_api).arg(QHostInfo::localHostName()).arg(accessToken);

    requestNetwork(serverApiUrl, requestData);
}

/* Json解释分离数据 */
QString Asr::getJsonValue(QByteArray ba, QString key)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &parseError);

    if (parseError.error == QJsonParseError::NoError) {

        if (jsonDocument.isObject()) {
            /* jsonDocument转化为json对象 */
            QJsonObject jsonObj = jsonDocument.object();

            if (jsonObj.contains(key)) {
                QJsonValue jsonVal= jsonObj.value(key);

                if (jsonVal.isString()) {
                    return jsonVal.toString();

                } else if (jsonVal.isArray()) {
                    /* 转换成jsonArray */
                    QJsonArray arr = jsonVal.toArray();
                    /* 获取第一个元素 */
                    QJsonValue jv = arr.at(0);
                    return jv.toString();
                }
            }
        }
    }

    return nullptr;
}
