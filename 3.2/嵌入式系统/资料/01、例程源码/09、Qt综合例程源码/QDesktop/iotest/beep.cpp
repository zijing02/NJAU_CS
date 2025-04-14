/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   iotest
* @brief         beep.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-20
*******************************************************************/
#include "beep.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>
#include <QEvent>

Beep::Beep(QObject *parent) : QObject (parent)
{
    keythread = new KeysThread;
    connect(keythread,SIGNAL(myKeyValueChanged()),this,SLOT(readMykeysChanged()));
}

void Beep::setKeyMonitor(bool str)
{
    if (str){
        keythread->start();
        keythread->startFlag = true;
    }
    else{
        keythread->startFlag = false;
        keythread->quit();
    }
}

void Beep::setBeepState(bool str)
{
    char const *filename = "/sys/devices/platform/leds/leds/beep/brightness";
    int err = 0;
    int fd;
    unsigned char databuf[1];

    if (str)
        databuf[0] = '1';
    else
        databuf[0] = '0';

    fd = open(filename, O_RDWR);
    if(fd < 0){
        close(fd);
        qDebug()<<"open file error!"<<endl;
        return;
    }

    err = write(fd, databuf, sizeof(databuf));
    if (err < 0){
        close(fd);
        qDebug()<<"read data error!"<<endl;
        return;
    }
    close(fd);
}

void Beep::readMykeysChanged()
{
    if(keythread->startFlag){
        key0 = keythread->key0Value;
        if (key0)
            setBeepState(true);
        else
            setBeepState(false);
        emit keysPressChanged();
    }
}

bool Beep::key0Str()
{
    return key0;
}

Beep::~Beep()
{

}

