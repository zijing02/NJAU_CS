#ifndef MYMODEL_H
#define MYMODEL_H
#include <QObject>
#include <QQmlListProperty>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "myjsonparse.h"
#include <QNetworkConfigurationManager>


class MyModelPrivate;
class MyModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQmlListProperty<IndexData> indexData READ indexData NOTIFY indexDataChanged)
    Q_PROPERTY(QQmlListProperty<WeatherData> weatherData READ weatherData NOTIFY weatherDataChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString cTemp READ cTemp WRITE setCurrentTemp NOTIFY pm25Changed)
    Q_PROPERTY(bool ready READ ready WRITE setReady NOTIFY readyChanged)

public:
    MyModel(QObject *parent = nullptr);
    ~MyModel();

    QQmlListProperty<IndexData> indexData();
    QQmlListProperty<WeatherData> weatherData();

    QString city() const;
    QString cTemp() const;
    bool ready() const;
    void readSettings();
    void writeSettings();
    void setNetWorkInfo();

public slots:
    void downRefresh(); //下拉刷新
    void setReady(bool arg);
    void setCity(const QString &arg);
    void setCurrentTemp(const QString &arg);
    void updateWeather(QNetworkReply *reply);
    void getCityName();
    void slotFinishedTimeGetReply();

protected:
    void timerEvent(QTimerEvent *event);

signals:
    void indexDataChanged();
    void weatherDataChanged();
    void readyChanged();
    void cityChanged();
    void pm25Changed();

private:
    MyModelPrivate *m_dptr;
    MyJsonParse m_parse;
    QString m_api;
    QNetworkConfigurationManager *m_NCGManager;
    bool m_ready;
    int m_timerID;
};

#endif // MYMODEL_H
