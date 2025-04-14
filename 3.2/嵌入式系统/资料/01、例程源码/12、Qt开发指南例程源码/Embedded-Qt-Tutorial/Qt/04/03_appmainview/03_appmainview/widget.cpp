/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   03_appmainview
* @brief         widget.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-09
*******************************************************************/
#include "widget.h"
#include <QPushButton>
#include <QDebug>

AppMainView::AppMainView(QWidget *parent)
{
    this->setParent(parent);
    this->setGeometry(0, 0, 800, 480);
    this->setMinimumSize(800, 480);

    bgWidget = new QWidget(this);
    bgWidget->setStyleSheet("border-image: url(:/images/bg.png)");

    mySlidePage = new SlidePage(this);
    mySlidePage->resize(this->size());

    for (int i = 0; i < 3; i++) {
        appDemo[i] = new AppDemo();
        mySlidePage->addPage(appDemo[i]);
    }

}

AppMainView::~AppMainView()
{
}


void AppMainView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    mySlidePage->resize(this->size());
    bgWidget->resize(this->size());
}
