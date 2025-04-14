/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   radio
* @brief         radio.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-08-19
*******************************************************************/
#ifndef MYRADIO_H
#define MYRADIO_H
#include <QObject>

namespace Ui {
class myRadio;
}

class myRadio : public QObject
{
    Q_OBJECT

public:
    explicit myRadio(QObject *parent = 0);
    ~myRadio();
    Q_INVOKABLE void setFrequency(const QString str);
    Q_INVOKABLE void setMuteFlag(bool flag);
    Q_PROPERTY(QString readLevel READ readLevel NOTIFY levelChanged)

private:
    QString readLevel();
    QString level;
    bool muteFlag;
    unsigned char *setRadioFreq(unsigned char radio[5], double frequency);
signals:
    void levelChanged();

};

#endif // MYRADIO_H
