/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         adc.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
#include "adc.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>

using namespace std;
Adc::Adc(QObject *parent) : QObject (parent)
{
    timer = new QTimer();
    connect(timer,SIGNAL(timeout()),this,SLOT(getAdcData()));
}

Adc::~Adc()
{

}

QString Adc::readADC()
{
    char const *filename = "/sys/devices/platform/soc/48003000.adc/48003000.adc:adc@0/iio:device0/in_voltage19_raw";
    int err = 0;
    int fd;
    char buf[10];

    fd = open(filename, O_RDWR);
    if(fd < 0){
        close(fd);
        setCapture(0);
        return "open file error!";
    }

    err = read(fd, buf, sizeof(buf));
    if (err < 0){
        close(fd);
        return "read data error!";
    }
    close(fd);

    QString adcValue = buf;
    QStringList list = adcValue.split("\n");
    return list[0];
}

QString Adc::adcData()
{
    return adcdata;
}

void Adc::getAdcData()
{
    adcdata = readADC();
    double temp_data = adcdata.toDouble() * 0.050354003;
    adcdata = QString::number(temp_data,'f',1);
    emit adcDataChanged();
}

void Adc::setCapture(bool str)
{
    if(str)
        timer->start(500);
    else
        timer->stop();
}
