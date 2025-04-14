/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   led
* @brief         led.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-01
*******************************************************************/
#include "led.h"
#include <QDebug>

Led::Led(QObject *parent)
{
    this->setParent(parent);
#if __arm__
    /* 默认是出厂系统的LED心跳的触发方式,想要控制LED，
     * 需要改变LED的触发方式，改为none，即无 */
    system("echo none > /sys/class/leds/sys-led/trigger");
#endif
    /* 开发板的LED控制接口 */
    file.setFileName("/sys/devices/platform/leds/leds/sys-led/brightness");
}

Led::~Led()
{
}

void Led::setLedState(bool flag)
{
    qDebug()<<flag<<endl;

    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[2] = {"0", "1"};

    /* 写0或1,1~255都可以点亮LED */
    if (flag)
        file.write(buf[1]);
    else
        file.write(buf[0]);

    /* 关闭文件 */
    file.close();
}

