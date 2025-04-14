/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   03_appmainview
* @brief         widget.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-09
*******************************************************************/
#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>

#include "../slidepage/slidepage.h"
#include "../appdemo/appdemo.h"

class SlidePage;
class AppDemo;

class AppMainView : public QWidget
{
    Q_OBJECT

public:
    AppMainView(QWidget *parent = nullptr);
    ~AppMainView();

private:
    /* 背景 */
    QWidget *bgWidget ;

    /* 滑动页面管理对象 */
    SlidePage *mySlidePage;

    /* 重设大小 */
    void resizeEvent(QResizeEvent *event);

    AppDemo *appDemo[3];

};
#endif // WIDGET_H
