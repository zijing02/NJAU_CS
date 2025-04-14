/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         dac.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-25
*******************************************************************/
#ifndef DAC_H
#define DAC_H
#include <QObject>
class Dac : public QObject
{
    Q_OBJECT

public:
    explicit Dac(QObject *parent = 0);
    ~Dac();

    Q_INVOKABLE void setDac(QString dac);
};
#endif // DAC_H
