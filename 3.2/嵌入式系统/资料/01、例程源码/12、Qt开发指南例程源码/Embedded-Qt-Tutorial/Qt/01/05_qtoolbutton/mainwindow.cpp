#include "mainwindow.h"
#include <QApplication>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置和大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化QToolBar对象 */
    toolBar = new QToolBar(this);
    /* 设置toolBar的位置和大小 */
    toolBar->setGeometry(0, 0, 800, 100);

    /* 实例化QStyle类对象，用于设置风格，调用系统类自带的图标 */
    QStyle *style = QApplication::style();

    /* 使用Qt自带的标准图标，可以在帮助文档里搜索QStyle::StandardPixmap */
    QIcon icon = style->standardIcon(QStyle::SP_TitleBarContextHelpButton);

    /* 实例化QToolButton对象 */
    toolButton = new QToolButton();

    /* 设置图标 */
    toolButton->setIcon(icon);
    /* 设置要显示的文本 */
    toolButton->setText("帮助");
    /* 调用setToolButtonStyle()方法，设置toolButoon的样式，设置为文本置于图标下方 */
    toolButton->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    /* 最后将toolButton添加到ToolBar里 */
    toolBar->addWidget(toolButton);
}

MainWindow::~MainWindow()
{
}

