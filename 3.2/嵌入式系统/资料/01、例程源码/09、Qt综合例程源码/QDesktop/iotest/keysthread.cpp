/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   iotest
* @brief         keysthread.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-20
*******************************************************************/
#include "keysthread.h"
#include <QDebug>
#include "beep.h"
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <QDebug>
#include <QEvent>
#if __arm__
#include <linux/input.h>
#endif
KeysThread::KeysThread(QObject *parent)
{
    key0Value = false;
    startFlag = false;
}

KeysThread::~KeysThread()
{

}

void KeysThread::run()
{
#if __arm__
    int fd;
    char const *filename = "/dev/input/event2";

    struct input_event input_ev;

    fd = open(filename, O_RDWR);
    if(fd < 0){
        qDebug()<<"file  open failed!"<<endl;
        return ;
    }

    for ( ; ; ) {
        if(!startFlag){
            break;
        }
        read(fd, &input_ev, sizeof(input_ev));
        if (input_ev.type == EV_KEY) {
            if (input_ev.value == 0) {
                key0Value = 0;
                if (input_ev.code == KEY_VOLUMEDOWN)
                    emit myKeyValueChanged();
            }else {
                if (input_ev.code == KEY_VOLUMEDOWN){
                    key0Value = input_ev.value;
                    emit myKeyValueChanged();
                }
            }
        }
    }
    close(fd);
#endif
}

