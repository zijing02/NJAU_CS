/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   appdemo
* @brief         appdemo.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-10
*******************************************************************/
#ifndef APPDEMO_H
#define APPDEMO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class AppDemo : public QWidget
{
    Q_OBJECT

public:
    AppDemo(QWidget *parent = nullptr);
    ~AppDemo();

private:
    /* 主widget */
    QWidget *mainWidget;

    /* 顶部widget */
    QWidget *topWidget;

    /* 中间widget */
    QWidget *middleWidget;

    /* 底部widget */
    QWidget *bottomWidget;

    /* 中间_左边Widget，也就是人物Widget */
    QWidget *mid_left_Widget;

    /* 中间_右边Widget，也就是小方格Widget */
    QWidget *mid_right_Widget;

    /* 中间_右边_顶部Widget */
    QWidget *mid_right_top_Widget;

    /* 中间_右边_底部Widget */
    QWidget *mid_right_bottom_Widget;

    /* 主布局 */
    QVBoxLayout *mainVLayout;

    /* 顶部布局 */
    QHBoxLayout *topHLayout;

    /* 中间布局 */
    QHBoxLayout *middleHLayout;

    /* 底部布局 */
    QHBoxLayout *bottomHLayout;

    /* 中间_左边布局 */
    QVBoxLayout *mid_left_VLayout;

    /* 中间_右边布局 */
    QVBoxLayout *mid_right_VLayout;

    /* 中间_右边顶部布局*/
    QHBoxLayout *mid_right_top_HLayout;

    /* 中间_右边底部布局*/
    QHBoxLayout *mid_right_bottom_HLayout;

    /* 中间按钮，从左到右，从上到下分布 */
    QPushButton *mid_pushButton[6];

    /* 顶部按钮，从左到右 */
    QPushButton *top_pushButton[5];

    /* 底部按钮，从左到右 */
    QPushButton *bottom_pushButton[7];

    /* 重设大小 */
    void resizeEvent(QResizeEvent *event);

    /* 加载qss */
    void loadStyle();
};
#endif // APPDEMO_H
