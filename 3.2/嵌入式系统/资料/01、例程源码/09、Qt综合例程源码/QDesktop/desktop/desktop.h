/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   desktop
* @brief         desktop头文件
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-01
*******************************************************************/
#ifndef DESKTOP_H
#define DESKTOP_H
#include <QObject>
#include <QProcess>
#include <QTimer>
class MyDesktop : public QObject
{
    Q_OBJECT

public:
    explicit MyDesktop(QObject *parent = 0);
    ~MyDesktop();

    Q_INVOKABLE void systemReboot();
    Q_INVOKABLE void systemPoweroff();
    Q_INVOKABLE void getSystemVolume();
    Q_INVOKABLE void setSystemVolume(QString value);
    Q_INVOKABLE void getCpuTemp();
    Q_INVOKABLE bool getCoreBoardInfo();
    Q_INVOKABLE void restoreMixerSettings();
    QString readCpuTemp();
    QString sysvolume;
    QString sysVolume();
    QString cputemp;
    QString cpuTemp();
    Q_PROPERTY(QString sysVolume READ sysVolume NOTIFY sysVolumeChanged)
    Q_PROPERTY(QString cpuTemp READ cpuTemp NOTIFY cpuTempChanged)
private:
    QProcess *myProcess;
    QTimer *timer;
private slots:
    void standardOutput_ReadVolume();
signals:
    void sysVolumeChanged();
    void cpuTempChanged();

};

#endif // DESKTOP_H
