/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   desktop
* @brief         desktop.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
#include "desktop.h"
#include <QDebug>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDir>
MyDesktop::MyDesktop(QObject *parent) : QObject (parent)
{
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(getCpuTemp()));
#ifdef __arm__
    getSystemVolume();
    timer->start(1000);
#endif
}

MyDesktop::~MyDesktop()
{

}

void MyDesktop::systemReboot()
{
    system("reboot");
    system("kill -9 $(pidof QDesktop)");
}

void MyDesktop::systemPoweroff()
{
//    system("poweroff");
//    system("kill -9 $(pidof QDesktop)");
}

QString MyDesktop::sysVolume()
{
    return sysvolume;
}
void MyDesktop::getSystemVolume()
{
#ifdef __arm__
    myProcess = new QProcess;
    connect(myProcess,&QProcess::readyReadStandardOutput,this,&MyDesktop::standardOutput_ReadVolume);
    system("echo $(amixer get Speaker |grep -E \"Left:\" |awk '{print $4}') >./volume.tmp");
    myProcess->start("cat ./volume.tmp");
    myProcess->waitForStarted();
#endif
}

void MyDesktop::setSystemVolume(QString value)
{
    //value += 80;
    if(value == "80")
        value = "0";
#ifdef __arm__
    QString cmdStr = tr("amixer set Speaker '%1','%1'").arg(value);
    system(cmdStr.toStdString().c_str());
#endif
}

void MyDesktop::standardOutput_ReadVolume()
{
    sysvolume = myProcess->readAllStandardOutput();
    //qDebug()<<"音量是" + sysvolume<<endl;
    sysvolume.remove(QChar('\n'), Qt::CaseInsensitive);
    //    if (sysvolume.toInt() <= 80)
    //        sysvolume = "0";
    //    else {
    //        sysvolume = QString::number(sysvolume.toInt() - 80);
    //    }
    emit sysVolumeChanged();
}

QString MyDesktop::cpuTemp()
{
    return cputemp;
}

QString MyDesktop::readCpuTemp()
{
    char const *filename = "/sys/class/hwmon/hwmon0/temp1_input";
    int err = 0;
    int fd;
    char buf[10];

    fd = open(filename, O_RDONLY);
    if(fd < 0){
        close(fd);
        timer->stop();
        return "open file error!";
    }

    err = read(fd, buf, sizeof(buf));
    if (err < 0){
        timer->stop();
        close(fd);
        return "read data error!";
    }
    close(fd);

    QString tempValue = buf;
    QStringList list = tempValue.split("\n");
    return list[0];
}

void MyDesktop::getCpuTemp()
{
    cputemp = readCpuTemp();
    double temp_data = cputemp.toDouble() / 1000;
    cputemp = QString::number(temp_data,'f',2);
    emit cpuTempChanged();
}

bool MyDesktop::getCoreBoardInfo()
{
    QFile file("/dev/mtd0");
    if (!file.exists())
        return true;
    else
        return false;
}

void MyDesktop::restoreMixerSettings()
{
    /* 由于某些原因需要重新设置耳机音量 */
#ifdef __arm__
    system("/usr/sbin/alsactl -f /var/lib/alsa/asound.state restore &");
#endif
}
