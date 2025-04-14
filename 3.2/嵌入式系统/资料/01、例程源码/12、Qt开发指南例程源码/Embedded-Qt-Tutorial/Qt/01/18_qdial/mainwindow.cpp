#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化对象和设置显示位置与大小 */
    dial = new QDial(this);
    dial->setGeometry(300, 100, 200, 200);

    /* 设置页长（两个最大刻度的间距）*/
    dial->setPageStep(10);

    /* 设置刻度可见 */
    dial->setNotchesVisible(true);

    /* 设置两个凹槽之间的目标像素数 */
    dial->setNotchTarget(1.00);

    /* 设置dial值的范围 */
    dial->setRange(0,100);

    /* 开启后可指向圆的任何角度 */
    //dial->setWrapping(true);

    /* 实例化对象和设置显示位置与大小 */
    label = new QLabel(this);
    label->setGeometry(370, 300, 200, 50);

    /* 初始化为0km/h */
    label->setText("0km/h");

    /* 信号槽连接 */
    connect(dial, SIGNAL(valueChanged(int)),
            this, SLOT(dialValueChanged(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::dialValueChanged(int val)
{
    /* QString::number()转换成字符串 */
    label->setText(QString::number(val) + "km/h");
}

