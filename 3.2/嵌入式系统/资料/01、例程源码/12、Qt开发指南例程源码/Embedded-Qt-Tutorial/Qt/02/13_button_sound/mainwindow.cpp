/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   13_button_sound
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-20
*******************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化按钮 */
    pushButton = new QPushButton(this);

    /* 设置按钮文本 */
    pushButton->setText("按钮音效测试");

    /* 设置按钮的位置与大小 */
    pushButton->setGeometry(340, 220, 120, 40);

    /* 信号槽连接 */
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::pushButtonClicked()
{
    /* 异步的方式播放 */
    QSound::play(":/audio/bell.wav");
}

