/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   esp8266
* @brief         esp8266.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-27
*******************************************************************/
#include "esp8266.h"
#include <unistd.h>
#include <QDebug>

Esp82266::Esp82266(QWidget *parent)
{
    Q_UNUSED(parent)
    /* 串口对象，用于与Esp8266模块通信 */
    serialPort = new QSerialPort(this);

    /* 定时器对象，用于定时发送设备在线的心跳包 */
    timer = new QTimer();

    /* led对象，用于串口接收到云发过来的数据，然后控制板子的LED */
    led = new Led(this);

    /* 设置串口名 */
#if __arm__
    serialPort->setPortName("ttymxc2");
#else
    serialPort->setPortName("ttyUSB0");
#endif

    /* 设置波特率 */
    serialPort->setBaudRate(115200);

    /* 设置数据位数 */
    serialPort->setDataBits(QSerialPort::Data8);

    /* 设置奇偶校验 */
    serialPort->setParity(QSerialPort::NoParity);

    /* 设置停止位 */
    serialPort->setStopBits(QSerialPort::OneStop);

    /* 设置流控制 */
    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort->open(QIODevice::ReadWrite))
        qDebug()<<"串口无法打开！可能正在被使用！"<<endl;
    else {
        qDebug()<<"串口打开成功！"<<endl;
    }

    /* 开始连接云 */
    connectToClound();

    connect(serialPort, SIGNAL(readyRead()),
            this, SLOT(serialPortReadyRead()));

    connect(timer, SIGNAL(timeout()),
            this, SLOT(onTimerTimeOut()));
}

void Esp82266::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */
    QByteArray buf = serialPort->readAll();

    QString temp = QString(buf);
    readData.append(temp);
    qDebug()<<temp<<endl;

    if  (readData.contains("ready")) {
        /* 如果复位成功 */
        sendCmdToEsp8266("AT+CWMODE=1");
        readData.clear();
    }

    if (readData.contains("OK") && readData.contains("AT+CWMODE")) {
        qDebug()<<"设置STA模式成功"<<endl;
        sendCmdToEsp8266("AT+CWJAP=\"ALIENTEK-YF\",\"15902020353\"");
        qDebug()<<"开始连接WIFI"<<endl;
        readData.clear();
    }

    if  (temp.contains("WIFI GOT IP")) {
        qDebug()<<"连接WIFI成功"<<endl;
        sleep(2);
        /* 原子云的设备号及密码 */
        sendCmdToEsp8266("AT+ATKCLDSTA=\"02314701717851074890\",\"12345678\"");
        qDebug()<<"开始连接原子云请等待"<<endl;
    }

    if (temp.contains("CLOUD CONNECTED")) {
        qDebug()<<"连接原子云成功"<<endl;
        sleep(2);
        /* 15s就发送一次心跳包 */
        timer->start(15000);
    }

    if (temp == "开")
        led->setLedState(true);
    else if (temp == "关")
        led->setLedState(false);

}

Esp82266::~Esp82266()
{
    serialPort->close();
    delete timer;
    timer = nullptr;
}

void Esp82266::sendCmdToEsp8266(QString cmd)
{
    cmd = cmd + "\r\n";

    QByteArray data = cmd.toUtf8();
    serialPort->write(data);
}

void Esp82266::connectToClound()
{
    /* 重启模块，注意若已经连接上原子云，
     * 需要重新上电或者短接RST脚来复位模块 */
    sendCmdToEsp8266("AT+RST");
}

void Esp82266::sleep(int second)
{
    usleep(second * 1000000);
}

void Esp82266::sendTextMessage(QString message)
{
    serialPort->write(message.toLatin1());
}

void Esp82266::onTimerTimeOut()
{
    sendTextMessage("online");
    qDebug()<<"发送设备在线心跳包"<<endl;
}
