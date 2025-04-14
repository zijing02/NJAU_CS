#ifndef MYJSONPARSE_H
#define MYJSONPARSE_H
#include <QJsonDocument>
#include <QQmlListProperty>

class IndexData;
class WeatherData;
class MyJsonParse
{
public :
    MyJsonParse();
    MyJsonParse(const QJsonDocument &doc);
    ~MyJsonParse();

    void setJsonDocument(const QJsonDocument &doc);
    QJsonDocument jsonDocument() const;

public:
    QString getCurrentCity() const;
    QString getCurrentTemp() const;
    QList<IndexData *> getIndexList() const;
    QList<WeatherData *> getWeatherList() const;

private:
    bool m_success;
    QJsonDocument m_jsonDoc;
};

#endif
