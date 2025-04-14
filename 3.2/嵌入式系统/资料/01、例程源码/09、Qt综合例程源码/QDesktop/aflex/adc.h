/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   aflex
* @brief         adc.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-25
*******************************************************************/
#ifndef ADC_H
#define ADC_H
#include <QObject>
#include <QTimer>

class Adc : public QObject
{
    Q_OBJECT

public:
    explicit Adc(QObject *parent = 0);
    ~Adc();

    Q_INVOKABLE void setCapture(bool str);
    QString readADC();

private:
    QTimer *timer;
    QString adcdata;

    QString adcData();
    Q_PROPERTY(QString adcData READ adcData NOTIFY adcDataChanged)

public slots:
    void getAdcData();

signals:
    void adcDataChanged();
};

#endif // ADC_H
