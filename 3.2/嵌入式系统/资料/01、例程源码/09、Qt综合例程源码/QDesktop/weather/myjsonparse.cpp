/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   weather
* @brief         myjsonparse.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-15
*******************************************************************/
#include "myjsonparse.h"
#include "weatherdata.h"
#include <QVariant>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

MyJsonParse::MyJsonParse()
    : m_success(false)
{

}

MyJsonParse::MyJsonParse(const QJsonDocument &doc)
    : m_success(false),
      m_jsonDoc(doc)
{

}

MyJsonParse::~MyJsonParse()
{

}

void MyJsonParse::setJsonDocument(const QJsonDocument &doc)
{
    if (!doc.isNull())
        m_jsonDoc = doc;
    if (m_jsonDoc.isObject()) {
        QJsonObject object = m_jsonDoc.object();
        m_success = object.value("desc").toString() == "OK";
    }
}

QJsonDocument MyJsonParse::jsonDocument() const
{
    return m_jsonDoc;
}

QString MyJsonParse::getCurrentCity() const
{
    if (m_success){
        QJsonObject object = m_jsonDoc.object();
        object = object.value("data").toObject();
        return  object.value("city").toString();
    }
    return "获取城市失败";
}

QString MyJsonParse::getCurrentTemp() const
{
    if (m_success){
        QJsonObject object = m_jsonDoc.object();
        object = object.value("data").toObject();
        return object.value("wendu").toString();
    }
    return "获取当前温度失败";
}

QList<IndexData *> MyJsonParse::getIndexList() const
{
    QList<IndexData *> indexs;
    if (m_success) {
        QJsonObject object = m_jsonDoc.object();
        object = object.value("data").toObject();
        IndexData *indexData = new IndexData;
        indexData->setDescript(object.value("ganmao").toString());
        indexs.push_back(indexData);
    }
    return indexs;
}

QList<WeatherData *> MyJsonParse::getWeatherList() const
{
    QList<WeatherData *> weathers;
    if (m_success){
        QJsonObject object = m_jsonDoc.object();
        object = object.value("data").toObject();
        QJsonArray array = object.value("forecast").toArray();
        for (auto it : array){
            object = it.toObject();
            WeatherData *weatherData = new WeatherData;

            if (object.value("date").toString().length() == 6)
                weatherData->setDate(object.value("date").toString().mid(3, 3));
            else
                weatherData->setDate(object.value("date").toString().mid(2, 3));

            QString temperature = object.value("high").toString().mid(3, 2);
            weatherData->setMaxTemperature(temperature.toInt());

            temperature = object.value("low").toString().mid(3, 2);
            weatherData->setMinTemperature(temperature.toInt());

            QString weather = object.value("type").toString();
            weatherData->setWeather(weather);

            QString imagestr;
            if (weather.left(1) == "晴")
                imagestr = "fine";
            else if ((weather.left(2) == "多云" ) || weather.left(1) == "阴")
                imagestr = "cloudy";
            else if (weather.contains("雨"))
                imagestr = "rain";
            else if (weather.contains("雪"))
                imagestr = "snow";
            else imagestr = "fine";

            weatherData->setDayPicture("qrc:/weather/image/" + imagestr + "_sun.png");
            weatherData->setNightPicture("qrc:/weather/image/" + imagestr + "_moon.png");
            weatherData->setWind(object.value("fengxiang").toString());
            weathers.push_back(weatherData);
        }
    }

    return weathers;
}
