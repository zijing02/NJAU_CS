#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的显示的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化标题为停靠窗口 */
    dockWidget = new QDockWidget("停靠窗口", this);

    /* 实例化文本编辑框 */
    textEdit = new QTextEdit(dockWidget);

    textEdit->setText("这是一个测试");

    /* 停靠窗口添加文本编辑框 */
    dockWidget->setWidget(textEdit);

    /* 放在主窗体的顶部 */
    this->addDockWidget(Qt::TopDockWidgetArea, dockWidget);
}

MainWindow::~MainWindow()
{
}

