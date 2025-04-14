/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   iotest
* @brief         beep.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-20
*******************************************************************/
#ifndef BEEP_H
#define BEEP_H
#include <QObject>
#include <QKeyEvent>
#include "keysthread.h"

class Beep : public QObject
{
    Q_OBJECT

private:
    KeysThread *keythread;
public:
    explicit Beep(QObject *parent = 0);
    ~Beep();
    bool key0;

    bool key0Str();

    Q_INVOKABLE void setBeepState(bool str);
    Q_INVOKABLE void setKeyMonitor(bool str);
    Q_PROPERTY(bool key0Str READ key0Str NOTIFY keysPressChanged())
public slots:
    void readMykeysChanged();
signals:

    void keysPressChanged();

};
#endif // BEEP_H
