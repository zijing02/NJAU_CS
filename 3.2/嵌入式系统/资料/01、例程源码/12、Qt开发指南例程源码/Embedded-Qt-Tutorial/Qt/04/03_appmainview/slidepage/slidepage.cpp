/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   slidepage
* @brief         slidepage.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-09
*******************************************************************/
#include "slidepage.h"
#include <QDebug>
#include <QPropertyAnimation>

SlidePage::SlidePage(QWidget *parent):
    QWidget(parent),
    pageIndex(0),
    pageCount(0),
    draggingFlag(false)
{
    pageIndicator.clear();
    this->setMinimumSize(400, 300);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    scrollArea = new QScrollArea(this);
    scrollArea->setAlignment(Qt::AlignCenter);

    mainWidget = new QWidget();
    mainWidget->setStyleSheet("background: transparent");

    scrollArea->setWidget(mainWidget);
    scrollArea->setStyleSheet("background: transparent");

    bottomWidget = new QWidget(this);
    bottomWidget->setStyleSheet("background: transparent");

    bottomHBoxLayout = new QHBoxLayout();
    bottomWidget->setLayout(bottomHBoxLayout);
    bottomHBoxLayout->setContentsMargins(0, 0, 0, 0);
    bottomHBoxLayout->setAlignment(Qt::AlignCenter);

    /* 关闭滚动条显示 */
    scrollArea->setVerticalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    scrollArea->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);

    /* 滚屏对象 */
    scroller = QScroller::scroller(scrollArea);
    QScroller::ScrollerGestureType gesture = QScroller::LeftMouseButtonGesture;
    scroller->grabGesture(scrollArea, gesture);

    /* 获取属性 */
    QScrollerProperties properties = scroller->scrollerProperties();

    /* 设置滑动的时间，值越大，时间越短 */
    properties.setScrollMetric(QScrollerProperties::SnapTime, 0.5);

    /* 设置滑动速度 */
    properties.setScrollMetric(QScrollerProperties::MinimumVelocity, 1);
    scroller->setScrollerProperties(properties);

    /* 布局 */
    hBoxLayout = new QHBoxLayout();

    hBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->setSpacing(0);

    mainWidget->setLayout(hBoxLayout);

    /* 定时器，用于判断用户是否是拖动屏幕，区分滑动，超过300ms表示拖动 */
    timer = new QTimer(this);

    connect(scrollArea->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(hScrollBarValueChanged(int)));
    connect(scroller, SIGNAL(stateChanged(QScroller::State)), this, SLOT(onStateChanged(QScroller::State)));
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimerTimeOut()));
    connect(this, SIGNAL(currentPageIndexChanged(int)), this, SLOT(onCurrentPageIndexChanged(int)));
}

SlidePage::~SlidePage()
{
}

void SlidePage::addPage(QWidget *w)
{
    /* 布局里添加页面 */
    hBoxLayout->addWidget(w);
    /* 页数加一 */
    pageCount++;
    QLabel *label = new QLabel();
    label->setPixmap(QPixmap(":/icons/indicator1.png"));
    pageIndicator.append(label);
    bottomHBoxLayout->addWidget(label);
}

void SlidePage::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    scrollArea->resize(this->size());
    /* mainWidget需要比scrollArea小 */
    mainWidget->resize(this->width() * pageCount, this->height() - 4);
    if (pageCount == 0)
        qDebug()<<"当前页面总数为0，请使用addPage()方法添加页面再使用！"<<endl;
    else
        onCurrentPageIndexChanged(0);
    bottomWidget->setGeometry(0, this->height() - 20, this->width(), 20);
}

void SlidePage::hScrollBarValueChanged(int)
{
    /* 滑动时判断当前页的下标 */
    pageIndex= scrollArea->horizontalScrollBar()->value() / this->width();
    pageIndex = scrollArea->horizontalScrollBar()->value()
            >= (pageIndex * this->width() + this->width() * 0.5) ? pageIndex + 1 : pageIndex;

}

void SlidePage::onStateChanged(QScroller::State state)
{
    static int pressedValue = 0;
    static int releasedValue = 0;
    static int currentPageIndex = 0;

    /* 如果页面数为0，返回，不做任何操作 */
    if (pageCount == 0)
        return;

    /* 松开 */
    if (state == QScroller::Inactive) {
        /* 停止定时器，防止检测到界面是缓慢拖动状态 */
        timer->stop();
        /* 记录松开时的坐标 */
        releasedValue = QCursor::pos().x();

        if (pressedValue == releasedValue)
            return;

        /* 判断按下与松开的距离，首先先判断是不是拖动状态，如果是拖动状态，pageIndex不会变化 */
        if (!draggingFlag) {
            if (pressedValue - releasedValue > 20 && currentPageIndex == pageIndex)
                pageIndex++;
            else
                pageIndex--;
        }

        /* 页面下标判断 */
        if (pageIndex == -1)
            pageIndex = 0;

        if (pageIndex >= pageCount)
            pageIndex = pageCount - 1;

        /* 动画 */
        QPropertyAnimation *animation = new QPropertyAnimation(scrollArea->horizontalScrollBar(), "value");
        animation->setDuration(200);
        animation->setStartValue(scrollArea->horizontalScrollBar()->value());
        animation->setEasingCurve(QEasingCurve::OutCurve);
        animation->setEndValue(pageIndex * this->width());
        animation->start();

        if (currentPageIndex != pageIndex) {
            /* 发送当前页面的位置信号 */
            emit currentPageIndexChanged(pageIndex);
        }

        /* 重新赋值*/
        pressedValue = 0;
        releasedValue = 0;
        draggingFlag = false;
    }

    /* 按下 */
    if (state == QScroller::Pressed) {
        pressedValue = QCursor::pos().x();
        currentPageIndex = scrollArea->horizontalScrollBar()->value() / this->width();
        /* 按下如果超过300ms，表示用户在拖动 */
        timer->start(300);
    }
}

void SlidePage::onTimerTimeOut()
{
    /* 拖动标志位 */
    draggingFlag = true;
    timer->stop();
}

int SlidePage::getPageCount()
{
    return pageCount;
}

int SlidePage::getCurrentPageIndex()
{
    return pageIndex;
}

void SlidePage::onCurrentPageIndexChanged(int index)
{
    for (int i = 0; i < pageIndicator.count(); i++) {
        if (i == index)
            pageIndicator[i]->setPixmap(QPixmap(":/icons/indicator2.png"));
        else
            pageIndicator[i]->setPixmap(QPixmap(":/icons/indicator1.png"));
    }
}


