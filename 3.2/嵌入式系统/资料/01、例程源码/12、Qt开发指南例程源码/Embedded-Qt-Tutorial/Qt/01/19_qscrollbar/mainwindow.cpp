#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体大小，位置 */
    this->setGeometry(0,0,800,480);

    /* 实例化水平滚动条及设置位置大小 */
    horizontalScrollBar = new QScrollBar(Qt::Horizontal, this);
    horizontalScrollBar->setGeometry(0, 450, 800, 30);

    /* 实例化垂直滚动条及设置位置大小 */
    verticalScrollBar = new QScrollBar(Qt::Vertical, this);
    verticalScrollBar->setGeometry(770, 0, 30, 480);

    /* 实例化，标签文本 */
    label = new QLabel(this);
    /* 设置文本 */
    label->setText("这是一个测试");
    /* 设置位置大小 */
    label->setGeometry(300, 200, 100, 20);
}

MainWindow::~MainWindow()
{
}

