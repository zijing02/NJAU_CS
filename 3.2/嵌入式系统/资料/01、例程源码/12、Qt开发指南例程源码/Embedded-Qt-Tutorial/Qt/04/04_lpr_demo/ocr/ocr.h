/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 2021-2030. All rights reserved.
* @projectName   ocr
* @brief         ocr.h
* @author        Deng Zhimao
* @email         dengzhimao@alientek.com
* @link          www.openedv.com
* @date          2021-11-17
*******************************************************************/
#ifndef OCR_H
#define OCR_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QHostInfo>

#include <QFile>
#include <QImage>

class Ocr : public QObject
{
    Q_OBJECT

public:
    Ocr(QObject *parent = nullptr);
    ~Ocr();

    /* 请求网络 */
    void requestNetwork(QString, QByteArray);

    /* 获取识别结果 */
    void getTheResult(QString fileName);
    void getTheResult(QImage image);

private:
    /* 存储获取tokenUrl地址 */
    QString tokenUrl;

    /* 存储serverapi地址 */
    QString serverApiUrl;

    /* 最终需要访问token的地址 */
    QString accessToken;

    /* 获取token的接口*/
    const QString token_org = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=%1&client_secret=%2&";

    /* 填写网页上申请的appkey 如api_key="g8eBUMSokVB1BHGmgxxxxxx" */
    const QString api_key = "填写自己的APP KEY";

    /* 填写网页上申请的APP SECRET 如secret_key="94dc99566550d87f8fa8ece112xxxxx" */
    const QString secret_key = "填写自己的APP SECRET";

    /* 百度服务器API接口，发送图片可返回识别结果 */
    const QString server_api = "https://aip.baidubce.com/rest/2.0/ocr/v1/license_plate?access_token=%1";

    /* 网络管理 */
    QNetworkAccessManager *networkAccessManager;

    /* 处理Json数据 */
    QString getJsonValue(QByteArray ba, QString key);

    /* 处理Json数据 */
    QString getJsonValue(QByteArray ba, QString key1, QString key2);

public slots:

    /* 准备读取响应返回来的数据 */
    void readyReadData();

    /* 响应完成处理 */
    void replyFinished();

    /* 开始识别 */
    void readyToDetection(QString);
    void readyToDetection(QImage);

signals:
    /* 识别到车牌，发送信号 */
    void ocrReadyData(QString);

};
#endif // OCR_H
