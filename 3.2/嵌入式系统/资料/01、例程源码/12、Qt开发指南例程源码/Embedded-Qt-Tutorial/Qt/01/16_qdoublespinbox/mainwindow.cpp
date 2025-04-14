#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    /* 实例化和设置显示的位置与大小 */
    doubleSpinBox = new QDoubleSpinBox(this);
    doubleSpinBox->setGeometry((this->width() - 200) / 2,
                               (this->height() - 30) / 2,
                               200, 30);
    /* 设置前缀 */
    doubleSpinBox->setPrefix("窗口大小");

    /* 设置后缀 */
    doubleSpinBox->setSuffix("%");

    /* 设置范围 */
    doubleSpinBox->setRange(50.00, 100.00);

    /* 设置初始值 */
    doubleSpinBox->setValue(100.00);

    /* 设置步长 */
    doubleSpinBox->setSingleStep(0.1);

    /* 信号槽连接 */
    connect(doubleSpinBox, SIGNAL(valueChanged(double)),
            SLOT(doubleSpinBoxValueChanged(double)));

}

MainWindow::~MainWindow()
{
}

void MainWindow::doubleSpinBoxValueChanged(double value)
{
    /* 重新计算窗口的宽与高 */
    int w = 800 * value / 100;
    int h = 480 * value / 100;

    /* 重新设置窗口的宽与高 */
    this->setGeometry(0, 0, w, h);

    /* 重新设置doubleSpinBox的显示位置 */
    doubleSpinBox->setGeometry((this->width() - 200) / 2,
                               (this->height() - 30) / 2,
                               200, 30);

}

