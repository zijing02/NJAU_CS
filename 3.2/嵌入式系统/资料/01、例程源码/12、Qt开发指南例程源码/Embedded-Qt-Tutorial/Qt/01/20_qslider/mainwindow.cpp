#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    /* 实例化水平滑动条对象*/
    horizontalSlider = new QSlider(Qt::Horizontal, this);

    /* 设置显示的位置与大小 */
    horizontalSlider->setGeometry(250, 100, 200, 20);

    /* 设置值范围 */
    horizontalSlider->setRange(0, 100);

    /* 实例化垂直滑动条对象 */
    verticalSlider = new QSlider(Qt::Vertical, this);

    /* 设置显示的位置与大小 */
    verticalSlider->setGeometry(200, 50, 20, 200);

    /* 设置值范围 */
    verticalSlider->setRange(0, 100);

    /* 实例化标签文本 */
    label = new QLabel("滑动条值：0", this);
    label->setGeometry(250, 200, 100, 20);

    /* 信号槽连接 */
    connect(horizontalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(horizontalSliderValueChanged(int)));
    connect(verticalSlider, SIGNAL(valueChanged(int)),
            this, SLOT(verticalSliderValueChanged(int)));

}

MainWindow::~MainWindow()
{
}

void MainWindow::horizontalSliderValueChanged(int val)
{
    /* 当水平滑动条的值改变时，改变垂直滑动条的值 */
    verticalSlider->setSliderPosition(val);

    /* 将int类型转变成字符 */

    QString str = "滑动条值：" + QString::number(val);

    /* 显示当前垂直或水平滑动条的值 */
    label->setText(str);

}

void MainWindow::verticalSliderValueChanged(int val)
{
    /* 当垂直滑动条的值改变时，改变水平滑动条的值 */
    horizontalSlider->setSliderPosition(val);
}

