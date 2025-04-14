/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   weather
* @brief         mymodel.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-15
*******************************************************************/
#include "mymodel.h"
#include "weatherdata.h"
#include <QSettings>
#include <QTextCodec>
#include <QDebug>
class MyModelPrivate
{
public:
    MyModelPrivate() { }
    ~MyModelPrivate() { cleanup(); }

    void cleanup() {
        if (!m_index.isEmpty()) {
            for (auto it : m_index)
                delete it;
            m_index.clear();
        }

        if (!m_data.isEmpty()) {
            for (auto it : m_data)
                delete it;
            m_data.clear();
        }
    }

public:
    QString m_city;
    QString m_pm25;
    QList<IndexData *> m_index;
    QList<WeatherData *> m_data;
};

MyModel::MyModel(QObject *parent)
    : QObject(parent),
      m_dptr(new MyModelPrivate),
      m_NCGManager(new QNetworkConfigurationManager(this)),
      m_ready(false)
{
    setNetWorkInfo();
    readSettings();
    m_timerID = startTimer(1000 * 60 * 60);
    connect(m_NCGManager, SIGNAL(onlineStateChanged(bool)), this, SLOT(downRefresh()));
}

void MyModel::setNetWorkInfo()
{
    QNetworkRequest  ipQueryNRequest;
    QNetworkAccessManager *NAManager = new  QNetworkAccessManager(this);
    //ipQueryNRequest.setUrl(QUrl("http://ip.ws.126.net/ipquery"));
    ipQueryNRequest.setUrl(QUrl("http://pv.sohu.com/cityjson?ie=utf-8"));
    QNetworkReply *reply = NAManager->get(ipQueryNRequest);
    connect(reply, SIGNAL(finished()), this, SLOT(getCityName()));
    connect(NAManager, &QNetworkAccessManager::finished, NAManager, &QNetworkAccessManager::deleteLater);

    QNetworkAccessManager *NAManager2 = new  QNetworkAccessManager(this);
    ipQueryNRequest.setUrl(QUrl("http://quan.suning.com/getSysTime.do"));
    QNetworkReply *timeReply = NAManager2->get(ipQueryNRequest);
    connect(timeReply, SIGNAL(finished()), this, SLOT(slotFinishedTimeGetReply()));
    connect(NAManager2, &QNetworkAccessManager::finished, NAManager2, &QNetworkAccessManager::deleteLater);
}

MyModel::~MyModel()
{
    delete m_dptr;
    if (m_timerID != 0)
        killTimer(m_timerID);
}

void MyModel::readSettings()
{
    QSettings setting("Settings//local.ini", QSettings::IniFormat);
    QString api = setting.value("api").toString();
    if (!api.isEmpty())
        m_api = api;
}

void MyModel::writeSettings()
{
    QSettings setting("Settings//local.ini", QSettings::IniFormat);
    setting.setValue("api", m_api);
}

void MyModel::downRefresh()
{
    setReady(false);
    //m_netManager->get(QNetworkRequest(QUrl(m_api)));
    setNetWorkInfo();
}


void MyModel::updateWeather(QNetworkReply *reply)
{
    QString str = reply->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8(), &error);
    reply->deleteLater();

    // qDebug()<<str<<endl;
    if (!doc.isNull() && (error.error == QJsonParseError::NoError)){
        m_parse.setJsonDocument(doc);
        setCurrentTemp(m_parse.getCurrentTemp());
        setCity(m_parse.getCurrentCity());
        m_dptr->m_index = m_parse.getIndexList();
        m_dptr->m_data = m_parse.getWeatherList();
        setReady(true);

        emit indexDataChanged();
        emit weatherDataChanged();
    }
}

void MyModel::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    downRefresh();
}

QQmlListProperty<IndexData> MyModel::indexData()
{
    return QQmlListProperty<IndexData>(this, m_dptr->m_index);
}

QQmlListProperty<WeatherData> MyModel::weatherData()
{
    return QQmlListProperty<WeatherData>(this, m_dptr->m_data);
}

QString MyModel::city() const
{
    return m_dptr->m_city;
}

bool MyModel::ready() const
{
    return m_ready;
}

void MyModel::setReady(bool arg)
{
    if (arg != m_ready) {
        m_ready = arg;
        emit readyChanged();
    }
}

QString MyModel::cTemp() const
{
    return m_dptr->m_pm25;
}

void MyModel::setCity(const QString &arg)
{
    if (arg != m_dptr->m_city) {
        m_dptr->m_city = arg;
        emit cityChanged();
    }
}

void MyModel::setCurrentTemp(const QString &arg)
{
    if (arg != m_dptr->m_pm25) {
        m_dptr->m_pm25 = arg;
        emit pm25Changed();
    }
}

void MyModel::slotFinishedTimeGetReply(void) {
    QNetworkReply *reply = (QNetworkReply *)sender();
    QTextCodec *codec = QTextCodec::codecForName("gbk");
    QString temp = codec->toUnicode(reply->readAll());
    reply->deleteLater();

    if (temp.isEmpty() ||
            temp.isNull()) {
        return;
    }

    int length = temp.length();
    int startIndex = temp.indexOf("sysTime2:") + 14;  // 2020-08-01 13:00:00,
    int endIndex = -1;

    if (14 == startIndex) { // -1
        return;
    }
    for (int i = startIndex; i < length; i++) {

        if (',' == temp.at(i)) {
            endIndex = i;
            break;
        }
    }

    if (-1 == endIndex) {
        return;
    }

#ifdef __arm__
    QString time = temp.mid(startIndex, endIndex - startIndex - 1);
    if (time.length() > 30)
        return;
    time = "\"" + time + "\"";
    QString cmd = "date -s "+ time;
    QByteArray cmdby = cmd.toLatin1();
    char* charCmd = cmdby.data();
    system(charCmd);
#endif
}

void MyModel::getCityName(){
    QNetworkReply *reply = (QNetworkReply *)sender();
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QString temp = codec->toUnicode(reply->readAll());
    reply->deleteLater();

    QStringList list;
    QStringList list2;
    QString cityName;

    // Shanghai, Beijing, Tianjin and Chongqing are not provinces
    if (temp.contains("省")) {
        list = temp.split("省");
        if (list.count() < 2)
            return;
        list2 = list[1].split("市");
        if (list2.count() == 0)
            return;
        cityName = list2[0];
    } else {
        list = temp.split(": \"");
        if (list.count() < 4)
            return;
        list2 = list[3].split("市");
        if (list2.count() == 0)
            return;
        cityName = list2[0];
    }

#if 0
    int length = temp.length();
    int startIndex = temp.indexOf("city:") + 6;
    int endIndex = -1;

    if (temp.isEmpty() || temp.isNull()) {
        return;
    }

    if (5 == startIndex) {
        return;
    }

    for (int i = startIndex; i < length; i++) {

        if (',' == temp.at(i)) {
            endIndex = i;
            break;
        }
    }

    if (-1 == endIndex) {
        return;
    }


    QString cityName = temp.mid(startIndex, endIndex - startIndex - 1);
#endif

    m_api = "http://wthrcdn.etouch.cn/weather_mini?city=" + cityName;
    QNetworkAccessManager *m_netManager = new QNetworkAccessManager(this);
    m_netManager->get(QNetworkRequest(QUrl(m_api)));
    connect(m_netManager, &QNetworkAccessManager::finished, this, &MyModel::updateWeather);
    connect(m_netManager, &QNetworkAccessManager::finished, m_netManager, &QNetworkAccessManager::deleteLater);
}
