#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置大小与位置 */
    this->setGeometry(0, 0, 800, 480);

    /* 使用资源里的文件时格式是  :+前缀+文件路径  */
    QPixmap pixmap(":images/openedv.png");

    labelImage = new QLabel(this);

    /* 标签大小为452×132,根据图像的大小来设置 */
    labelImage->setGeometry(180, 150, 452, 132);
    /* 设置图像 */
    labelImage->setPixmap(pixmap);
    /* 开启允许缩放填充 */
    labelImage->setScaledContents(true);

    labelString = new QLabel(this);
    labelString->setText("标签演示文本");
    labelString->setGeometry(300, 300, 100, 20);
}

MainWindow::~MainWindow()
{
}

