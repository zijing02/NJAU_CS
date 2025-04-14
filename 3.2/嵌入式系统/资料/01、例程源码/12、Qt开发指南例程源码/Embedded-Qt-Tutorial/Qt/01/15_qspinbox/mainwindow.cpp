#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    /* 设置主窗口背景颜色，rgb颜色标准，a代表不透明度（0～100）*/
    this->setStyleSheet("QMainWindow{background-color: "
                        "rgba(100, 100, 100, 100%) }");

    spinBox = new QSpinBox(this);
    spinBox->setGeometry(350, 200, 150, 30);

    /* 设置范围0～100 */
    spinBox->setRange(0, 100);

    /* 设置步长为10 */
    spinBox->setSingleStep(10);

    /* 设置初始值为100 */
    spinBox->setValue(100);

    /* 设置后缀 */
    spinBox->setSuffix("%不透明度");

    /* 信号槽连接 */
    connect(spinBox,SIGNAL(valueChanged(int)), this,
            SLOT(spinBoxValueChanged(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::spinBoxValueChanged(int opacity)
{
    /* 转换为double数据类型 */
    double  dobleopacity = (double)opacity / 100;

    /* 设置窗体不透明度，范围是0.0～1.0。1则为不透明，0为全透明 */
    this->setWindowOpacity(dobleopacity);
}

