/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   cameramedia
* @brief         cameramedia.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-11-18
*******************************************************************/
#ifndef CAMERAMEDIA_H
#define CAMERAMEDIA_H
#include <QObject>
#include <QProcess>

class CameraMedia : public QObject
{
    Q_OBJECT

private:
    QProcess *process;
public:
    explicit CameraMedia(QObject *parent = 0);
    ~CameraMedia();
    Q_INVOKABLE void setCameraState(bool str);
    Q_INVOKABLE void setMediaState(bool str);
};
#endif // CAMERAMEDIA
