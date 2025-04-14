/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   esp8266
* @brief         esp8266.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-27
*******************************************************************/
#ifndef ESP8266_H
#define ESP8266_H

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include "../led/led.h"

class Esp82266 : public QObject
{
    Q_OBJECT

public:
    Esp82266(QWidget *parent = nullptr);
    ~Esp82266();

private:
    /* led对象 */
    Led *led;

    /* 定时器，发送心跳包 */
    QTimer *timer;

    /* 串口对象 */
    QSerialPort *serialPort;

    /* 发送命令到esp8266模块 */
    void sendCmdToEsp8266(QString cmd);

    /* 接收到的数据 */
    QString readData = nullptr;

    /* 延时函数 */
    void sleep(int);

    /* 连接到云服务器 */
    void connectToClound();

private slots:
    /* 串口读消息 */
    void serialPortReadyRead();

    /* 串口发送消息 */
    void sendTextMessage(QString);

    /* 定时器时间到，发送心跳包 */
    void onTimerTimeOut();
};
#endif // ESP8266_H
