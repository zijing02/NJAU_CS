/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   07_key
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-19
*******************************************************************/
#include "mainwindow.h"
#include <QGuiApplication>
#include <QScreen>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 获取屏幕的分辨率，Qt官方建议使用这
     * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
     * 注意，这是获取整个桌面系统的分辨率
     */
    QList <QScreen *> list_screen =  QGuiApplication::screens();

    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->setGeometry(0, 0, 800, 480);
#endif

    /* 标签实例化 */
    label = new QLabel(this);

    /* 设置默认文本 */
#if __arm__
    label->setText("VolumeDown松开状态");
#else
    label->setText("Down按键松开");
#endif

    /* 设置对齐方式 */
    label->setAlignment(Qt::AlignCenter);

    /* 居中显示 */
    setCentralWidget(label);
}

MainWindow::~MainWindow()
{
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
#if __arm__
    /* 判断按下的按键，也就是板子KEY0按键 */
    if(event->key() == Qt::Key_VolumeDown) {
        /* 设置label的文本 */
        label->setText("VolumeDown按键按下");
    }
#else
    /* 判断按下的按键，也就是"↓"方向键 */
    if(event->key() == Qt::Key_Down) {
        /* 设置label的文本 */
        label->setText("Down按键按下");
    }

#endif
    /* 保存默认事件 */
    QWidget::keyPressEvent(event);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
#if __arm__
    /* 判断松开的按键，也就是板子KEY0按键 */
    if(event->key() == Qt::Key_VolumeDown) {
        /* 设置label的文本 */
        label->setText("VolumeDown按键松开");
    }
#else
    /* 判断按下的按键，也就是"↓"方向键 */
    if(event->key() == Qt::Key_Down) {
        /* 设置label的文本 */
        label->setText("Down按键松开");
    }
#endif
    /* 保存默认事件 */
    QWidget::keyReleaseEvent(event);
}


