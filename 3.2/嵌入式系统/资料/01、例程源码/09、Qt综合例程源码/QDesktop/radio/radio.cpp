/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   radio
* @brief         radio.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-08-19
*******************************************************************/
#include "radio.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <QDebug>
#ifdef __arm__
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#endif

#define I2C_DEV       "/dev/i2c-1"
#define I2C_TEA5767_ID   0x60

myRadio::myRadio(QObject *parent) : QObject (parent)
{
    muteFlag = false;
}

QString myRadio::readLevel()
{
    return level;
}

unsigned char* myRadio::setRadioFreq(unsigned char radio[5], double frequency)
{
    unsigned char frequencyH = 0;
    unsigned char frequencyL = 0;

    unsigned int frequencyB;

    frequencyB = 4 * (frequency * 1000000 + 225000) / 32768;
    frequencyH = frequencyB >> 8;
    frequencyL = frequencyB & 0XFF;
    if (muteFlag)
        radio[0] |= 0x80;
    else
        radio[0] = frequencyH;
    radio[1] = frequencyL;
    radio[2] = 0xB0;
    radio[3] = 0x10;
    radio[4] = 0x00;
    return radio;
}

void myRadio::setFrequency(const QString str)
{
#ifdef __arm__
    double frequency;
    static unsigned char radio[5] = {0};
    char* temChar;
    int fd;

    QByteArray ba = str.toLatin1();
    temChar = ba.data();

    //open i2c device
    if ((fd = open(I2C_DEV, O_RDWR)) < 0) {
        printf("error opening i2c dev %s \r\n", I2C_DEV);
        return;
    }

    //set slave id
    if (ioctl (fd, I2C_SLAVE, I2C_TEA5767_ID) < 0) {
        printf("error set i2c slave id %02x \r\n", I2C_TEA5767_ID);
        return;
    }

    frequency = strtod(temChar, NULL);


    write(fd, setRadioFreq(radio, frequency), 5);

    int rlen = read(fd, radio, 5);
    if (rlen == 5) {
        unsigned int levelTemp = radio[3] >> 4;
        level = QString::number(levelTemp);
    }else {
        level = "";
    }
    emit levelChanged();
    //usleep(50000);
    close(fd);
#endif

}

void myRadio::setMuteFlag(bool flag)
{
    muteFlag = flag;
}
myRadio::~myRadio()
{

}


