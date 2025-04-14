#ifndef WEATHERDATA_H
#define WEATHERDATA_H
#include <QObject>

class IndexData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString descript READ descript WRITE setDescript NOTIFY descriptChanged)
    Q_PROPERTY(QString tipt READ tipt WRITE setTipt NOTIFY tiptChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString state READ state WRITE setState NOTIFY stateChanged)

public:
    IndexData(QObject *parent = nullptr);
    ~IndexData();

    QString descript() const;
    QString tipt() const;
    QString title() const;
    QString state() const;

public slots:
    void setDescript(const QString &arg);
    void setTipt(const QString &arg);
    void setTitle(const QString &arg);
    void setState(const QString &arg);

signals:
    void descriptChanged();
    void tiptChanged();
    void titleChanged();
    void stateChanged();

private:
    QString m_descript;
    QString m_tipt;
    QString m_title;
    QString m_state;
};

class WeatherData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString dayPicture READ dayPicture WRITE setDayPicture NOTIFY dayPictureChanged)
    Q_PROPERTY(QString nightPicture READ nightPicture WRITE setNightPicture NOTIFY nightPictureChanged)
    Q_PROPERTY(QString weather READ weather WRITE setWeather NOTIFY weatherChanged)
    Q_PROPERTY(QString wind READ wind WRITE setWind NOTIFY windChanged)
    Q_PROPERTY(int maxTemperature READ maxTemperature WRITE setMaxTemperature NOTIFY maxTemperatureChanged)
    Q_PROPERTY(int minTemperature READ minTemperature WRITE setMinTemperature NOTIFY minTemperatureChanged)

public:
    WeatherData(QObject *parent = nullptr);
    ~WeatherData();

    QString date() const;
    QString dayPicture() const;
    QString nightPicture() const;
    QString weather() const;
    QString wind() const;
    int maxTemperature() const;
    int minTemperature() const;

public slots:
    void setDate(const QString &arg);
    void setDayPicture(const QString &arg);
    void setNightPicture(const QString &arg);
    void setWeather(const QString &arg);
    void setWind(const QString &arg);
    void setMaxTemperature(int arg);
    void setMinTemperature(int arg);

signals:
    void dateChanged();
    void dayPictureChanged();
    void nightPictureChanged();
    void weatherChanged();
    void windChanged();
    void maxTemperatureChanged();
    void minTemperatureChanged();

private:
    QString m_date;
    QString m_dayPicture;
    QString m_nightPicture;
    QString m_weather;
    QString m_wind;
    int m_maxTemperature;
    int m_minTemperature;
};

#endif // WEATHERDATA_H
