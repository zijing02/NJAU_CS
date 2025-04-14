/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   02_beep
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-11
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
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
    this->resize(800, 480);
#endif

    pushButton = new QPushButton(this);

    /* 居中显示 */
    pushButton->setMinimumSize(200, 50);
    pushButton->setGeometry((this->width() - pushButton->width()) /2 ,
                            (this->height() - pushButton->height()) /2,
                            pushButton->width(),
                            pushButton->height()
                            );
    /* 开发板的蜂鸣器控制接口 */
    file.setFileName("/sys/devices/platform/leds/leds/beep/brightness");

    if (!file.exists())
        /* 设置按钮的初始化文本 */
        pushButton->setText("未获取到BEEP设备！");

    /* 获取BEEP的状态 */
    getBeepState();

    /* 信号槽连接 */
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(pushButtonClicked()));
}


MainWindow::~MainWindow()
{
}

void MainWindow::setBeepState()
{
    /* 在设置BEEP状态时先读取 */
    bool state = getBeepState();

    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QByteArray buf[2] = {"0", "1"};

    if (state)
        file.write(buf[0]);
    else
        file.write(buf[1]);

    file.close();

    getBeepState();
}

bool MainWindow::getBeepState()
{
    /* 如果文件不存在，则返回 */
    if (!file.exists())
        return false;

    if(!file.open(QIODevice::ReadWrite))
        qDebug()<<file.errorString();

    QTextStream in(&file);

    /* 读取文件所有数据 */
    QString buf = in.readLine();

    /* 打印出读出的值 */
    qDebug()<<"buf: "<<buf<<endl;
    file.close();
    if (buf == "1") {
        pushButton->setText("BEEP开");
        return true;
    } else {
        pushButton->setText("BEEP关");
        return false;
    }
}

void MainWindow::pushButtonClicked()
{
    /* 设置蜂鸣器的状态 */
    setBeepState();
}


