/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   iotest
* @brief         keysthread.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-20
*******************************************************************/
#ifndef KEYSTHREAD_H
#define KEYSTHREAD_H
#include <QThread>

class KeysThread : public QThread
{
    Q_OBJECT
public:
    KeysThread(QObject *parent = nullptr);
    ~KeysThread();
    bool key0Value;
    bool startFlag;

private:
    virtual void run();

signals:
    void myKeyValueChanged();

public slots:

};
#endif // KEYSTHREAD_H
