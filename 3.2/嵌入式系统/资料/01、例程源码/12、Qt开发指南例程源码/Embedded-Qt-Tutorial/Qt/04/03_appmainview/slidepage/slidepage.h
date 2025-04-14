/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   slidepage
* @brief         slidepage.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-09
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QScroller>
#include <QScrollBar>
#include <QScrollArea>
#include <QTimer>
#include <QHBoxLayout>
#include <QLabel>

class SlidePage : public QWidget
{
    Q_OBJECT

public:
    SlidePage(QWidget *parent = nullptr);
    ~SlidePage();

    /* 添加新的页面 */
    void addPage(QWidget *);

    /* 页面总数 */
    int getPageCount();

    /* 当前页面 */
    int getCurrentPageIndex();

private:

    /* 滚动区域 */
    QScrollArea *scrollArea;

    /* 主Widget */
    QWidget *mainWidget;

    /* 水平布局 */
    QHBoxLayout *hBoxLayout;

    /* 滚屏对象 */
    QScroller *scroller;

    /* 重设大小 */
    void resizeEvent(QResizeEvent *event);

    /* 定时器，用于检测用户是否在拖动屏幕 */
    QTimer *timer;

    /* 当前页数下标 */
    int pageIndex;

    /* 总页数 */
    int pageCount;

    /* 拖动标志位 */
    bool draggingFlag;

    /* 底部Wiget，用于逗点分页显示 */
    QWidget *bottomWidget;

    /* 分点逗点 */
    QVector<QLabel *>pageIndicator;

    /* 底部分页逗点布局 */
    QHBoxLayout *bottomHBoxLayout;

private slots:
    /* 水平滚动条值变化 */
    void hScrollBarValueChanged(int);

    /* 滚动对象的状态 */
    void onStateChanged(QScroller::State);

    /* 定时器超时槽函数 */
    void onTimerTimeOut();

    /* 分页逗点槽函数 */
    void onCurrentPageIndexChanged(int);

signals:
    void currentPageIndexChanged(int);
};
#endif // MAINWINDOW_H
