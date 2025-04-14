#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的大小与位置 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化与设置显示的位置大小*/
    lcdNumber = new QLCDNumber(this);
    lcdNumber->setGeometry(300, 200, 200, 50);

    /* 设置显示的位数8位 */
    lcdNumber->setDigitCount(8);
    /* 设置样式 */
    lcdNumber->setSegmentStyle(QLCDNumber::Flat);

    /* 设置lcd显示为当前系统时间 */
    QTime time = QTime::currentTime();

    /* 设置显示的样式 */
    lcdNumber->display(time.toString("hh:mm:ss"));

    timer = new QTimer(this);
    /* 设置定时器1000毫秒发送一个timeout()信号 */
    timer->start(1000);

    /* 信号槽连接 */
    connect(timer, SIGNAL(timeout()), this,
            SLOT(timerTimeOut()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::timerTimeOut()
{
    /* 当定时器计时1000毫秒后，刷新lcd显示当前系统时间 */
    QTime time = QTime::currentTime();
    /* 设置显示的样式 */
    lcdNumber->display(time.toString("hh:mm:ss"));
}

