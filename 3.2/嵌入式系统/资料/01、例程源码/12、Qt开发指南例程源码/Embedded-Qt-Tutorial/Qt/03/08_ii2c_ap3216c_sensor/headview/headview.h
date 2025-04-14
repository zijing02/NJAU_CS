/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   headview
* @brief         headview.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-22
*******************************************************************/
#ifndef HEADVIEW_H
#define HEADVIEW_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


class HeadView : public QWidget
{
    Q_OBJECT

public:
    HeadView(QWidget *parent = nullptr);
    ~HeadView();

private:
    QWidget *widget;
    QLabel *textLabel;
    QWidget *iconWidget;
    QWidget *lineWidget;

    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;
};
#endif // HEADVIEW_H
