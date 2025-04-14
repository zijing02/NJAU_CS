/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   cameramedia
* @brief         cameramedia.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-11-18
*******************************************************************/
#include "cameramedia.h"
#include <QDebug>

CameraMedia::CameraMedia(QObject *parent) : QObject (parent)
{
    process = new QProcess(this);
}

void CameraMedia::setCameraState(bool str)
{
#ifdef __arm__
    if (str)
        process->start("gst-launch-1.0 -v imxv4l2src device=/dev/video1 ! \"video/x-raw, format=(string)YUY2, width=(int)1024, height=(int)768, framerate=(fraction)30/1\" ! imxv4l2sink ");
    else {
        process->kill();
        process->start("killall gst-launch-1.0");
    }
#endif
}

void CameraMedia::setMediaState(bool str)
{
#ifdef __arm__
    if (str)
        process->start("gst-play-1.0 /media/test_movie.avi");
    else {
        process->kill();
        process->start("killall gst-play-1.0");
    }
#endif
}

CameraMedia::~CameraMedia()
{

}

