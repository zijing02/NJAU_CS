/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   led
* @brief         led.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-01
*******************************************************************/
#ifndef LED_H
#define LED_H

#include <QObject>
#include <QFile>
class Led : public QObject
{
    Q_OBJECT

public:
    Led(QObject *parent = nullptr);
    ~Led();

    /* 设置lED的状态 */
    void setLedState(bool);

private:
    /* 文件 */
    QFile file;
};
#endif // LED_H
