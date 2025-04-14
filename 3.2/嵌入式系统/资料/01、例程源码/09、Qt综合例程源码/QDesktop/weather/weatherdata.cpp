#include "weatherdata.h"

IndexData::IndexData(QObject *parent)
    : QObject(parent),
      m_descript("未知"),
      m_tipt("未知"),
      m_title("未知"),
      m_state("未知")
{

}

IndexData::~IndexData()
{

}

QString IndexData::descript() const
{
    return m_descript;
}

QString IndexData::tipt() const
{
    return m_tipt;
}

QString IndexData::title() const
{
    return m_title;
}

QString IndexData::state() const
{
    return m_state;
}

void IndexData::setDescript(const QString &arg)
{
    if (arg != m_descript)
    {
        m_descript = arg;
        emit descriptChanged();
    }
}

void IndexData::setTipt(const QString &arg)
{
    if (arg != m_tipt)
    {
        m_tipt = arg;
        emit tiptChanged();
    }
}

void IndexData::setTitle(const QString &arg)
{
    if (arg != m_title)
    {
        m_title = arg;
        emit titleChanged();
    }
}

void IndexData::setState(const QString &arg)
{
    if (arg != m_state)
    {
        m_state = arg;
        emit stateChanged();
    }
}


WeatherData::WeatherData(QObject *parent)
    : QObject(parent),
      m_date("未知"),
      m_dayPicture(""),
      m_nightPicture(""),
      m_weather("未知"),
      m_wind("未知"),
      m_maxTemperature(0),
      m_minTemperature(0)
{

}

WeatherData::~WeatherData()
{

}

QString WeatherData::date() const
{
    return m_date;
}

QString WeatherData::dayPicture() const
{
    return m_dayPicture;
}

QString WeatherData::nightPicture() const
{
    return m_nightPicture;
}

QString WeatherData::weather() const
{
    return m_weather;
}

QString WeatherData::wind() const
{
    return m_wind;
}

int WeatherData::maxTemperature() const
{
    return m_maxTemperature;
}

int WeatherData::minTemperature() const
{
    return m_minTemperature;
}

void WeatherData::setDate(const QString &arg)
{
    if (arg != m_date)
    {
        m_date = arg;
        emit dateChanged();
    }
}

void WeatherData::setDayPicture(const QString &arg)
{
    if (arg != m_dayPicture)
    {
        m_dayPicture = arg;
        emit dayPictureChanged();
    }
}

void WeatherData::setNightPicture(const QString &arg)
{
    if (arg != m_nightPicture)
    {
        m_nightPicture = arg;
        emit nightPictureChanged();
    }
}

void WeatherData::setWeather(const QString &arg)
{
    if (arg != m_weather)
    {
        m_weather = arg;
        emit weatherChanged();
    }
}

void WeatherData::setWind(const QString &arg)
{
    if (arg != m_wind)
    {
        m_wind = arg;
        emit windChanged();
    }
}

void WeatherData::setMaxTemperature(int arg)
{
    if (arg != m_maxTemperature)
    {
        m_maxTemperature = arg;
        emit maxTemperatureChanged();
    }
}

void WeatherData::setMinTemperature(int arg)
{
    if (arg != m_minTemperature)
    {
        m_minTemperature = arg;
        emit minTemperatureChanged();
    }
}
