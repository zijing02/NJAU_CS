/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 2021-2030. All rights reserved.
* @projectName   ocr
* @brief         ocr.cpp
* @author        Deng Zhimao
* @email         dengzhimao@alientek.com
* @link          www.openedv.com
* @date          2021-11-17
*******************************************************************/
#include "ocr.h"
#include <QCoreApplication>
#include <QBuffer>

Ocr::Ocr(QObject *parent)
    : QObject(parent)
{
    /* 网络管理 */
    networkAccessManager = new QNetworkAccessManager(this);

    /* 获取token的 Url */
    tokenUrl = QString(token_org).arg(api_key).arg(secret_key);

    QByteArray requestData;
    requestData.clear();

    /* 开始获取tokenUrl */
    requestNetwork(tokenUrl, requestData);
}

Ocr::~Ocr()
{
}

/* 请求网络 */
void Ocr::requestNetwork(QString url, QByteArray requestData)
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

    /* 设置Header参数值为application/x-www-form-urlencoded */
    networkRequest.setRawHeader("Content-Type", QString("application/x-www-form-urlencoded").toLatin1());

    /* 设置访问的地址 */
    networkRequest.setUrl(url);

    /* 网络响应 */
    QNetworkReply *newReply = networkAccessManager->post(networkRequest, requestData);

    connect(newReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(newReply, SIGNAL(readyRead()), this, SLOT(readyReadData()));

}

/* 读取数据 */
void Ocr::readyReadData()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    QByteArray data = reply->readAll();

    if (reply->url() == QUrl(tokenUrl)) {

        qDebug()<<QString(data)<<endl;
        QString key = "access_token";

        QString temp = getJsonValue(data, key);
        accessToken = temp;

        qDebug()<<accessToken<<endl;
        if (!data.contains("error")) {
            qDebug()<<"获取token成功，可以调用readyToDetection发送图片返回结果"<<endl;
        } else {
            accessToken = nullptr;
            qDebug() << "获取token失败，请检查您的APP SECRET和APPKEY，或者是否开通了车牌识别服务！"<<endl;
        }
    }

    if (reply->url() == QUrl(serverApiUrl)) {
        qDebug()<<QString(data)<<endl;
        QString key1 = "words_result";
        QString key2 = "number";
        QString temp = getJsonValue(data, key1, key2);
        emit ocrReadyData(temp);
        qDebug()<< "车牌识别结果为：" << temp<<endl;
    }
}

/* 请求完成处理，释放对象 */
void Ocr::replyFinished()
{
    QNetworkReply *reply = (QNetworkReply *)sender();
    reply->deleteLater();
    reply = nullptr;
}

/* 开始识别 */
void Ocr::getTheResult(QString fileName)
{
    QFile file;
    file.setFileName(fileName);
    if (!file.exists()) {
        qDebug()<<fileName<<"不存在!"<<endl;
        return;
    }

    QByteArray requestData;

    file.open(QIODevice::ReadOnly);
    requestData = file.readAll();
    file.close();

    /* 转成buf64 */
    QByteArray buf64 = requestData.toBase64().toPercentEncoding();
    QByteArray body = "image=" + buf64;

    serverApiUrl = QString(server_api).arg(accessToken);

    requestNetwork(serverApiUrl, body);
}

/* 开始识别 */
void Ocr::getTheResult(QImage image)
{
    QByteArray requestData;

    QBuffer buffer(&requestData);

    buffer.open(QIODevice::WriteOnly);

    image.save(&buffer, "JPEG", -1);

    /* 转成buf64 */
    QByteArray buf64 = requestData.toBase64().toPercentEncoding();
    QByteArray body = "image=" + buf64;

    serverApiUrl = QString(server_api).arg(accessToken);

    requestNetwork(serverApiUrl, body);
}

/* Json解释分离数据 */
QString Ocr::getJsonValue(QByteArray ba, QString key)
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

/* Json解释分离数据 */
QString Ocr::getJsonValue(QByteArray ba, QString key1, QString key2)
{
    QJsonParseError parseError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(ba, &parseError);

    if (parseError.error == QJsonParseError::NoError) {

        if (jsonDocument.isObject()) {
            /* jsonDocument转化为json对象 */
            QJsonObject jsonObj = jsonDocument.object();

            if (jsonObj.contains(key1)) {
                QJsonObject jsonArrObj = jsonObj.value(key1).toObject();
                if (jsonArrObj.contains(key2)) {
                    return jsonArrObj.value(key2).toString();
                } else { qDebug() << "not contains " + key2 << endl;}
            } else { qDebug() << "not contains " + key1 << endl;}
        } else { qDebug() << "is not an Object" << endl;}
    }

    return nullptr;
}

/* 准备识别 */
void Ocr::readyToDetection(QString imagePath)
{
    if (accessToken.isEmpty()) {
        qDebug() << "未获取到token!已返回！" <<endl;
        return;
    }
    getTheResult(imagePath);
}

/* 准备识别 */
void Ocr::readyToDetection(QImage image)
{
    if (accessToken.isEmpty()) {
        qDebug() << "未获取到token!已返回！" <<endl;
        return;
    }
    getTheResult(image);
}
