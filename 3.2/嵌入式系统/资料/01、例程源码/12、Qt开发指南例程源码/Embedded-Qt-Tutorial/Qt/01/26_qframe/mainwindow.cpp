#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化 */
    hline = new QFrame(this);
    /* 确定起始点，设置长和宽，绘制距形 */
    hline->setGeometry(QRect(200, 100, 400, 40));

    /* 设置框架样式为Hline，水平，可设置为其他样式例如Box，
     * 由于是样式选择HLine，所以只显示一条水平直线
     */
    hline->setFrameShape(QFrame::HLine);
    /* 绘制阴影 */
    hline->setFrameShadow(QFrame::Sunken);

    /* 实例化 */
    vline = new QFrame(this);
    /* 确定起始点，设置长和宽，绘制距形 */
    vline->setGeometry(QRect(300, 100, 2, 200));

    /* 设置框架样式为Vline，垂直，可设置为其他样式例如Box，
     * 由于是样式选择Vline，所以只显示一条垂直直线
     */
    vline->setFrameShape(QFrame::VLine);
    /* 绘制阴影 */
    vline->setFrameShadow(QFrame::Sunken);
}

MainWindow::~MainWindow()
{
}

