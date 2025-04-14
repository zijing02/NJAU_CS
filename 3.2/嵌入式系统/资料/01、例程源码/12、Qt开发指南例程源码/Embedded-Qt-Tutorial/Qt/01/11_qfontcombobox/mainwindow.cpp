#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的显示位置和大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化对象 */
    fontComboBox = new QFontComboBox(this);
    label = new QLabel(this);

    /* 设置显示的位置与大小 */
    fontComboBox->setGeometry(280, 200, 200, 30);
    label->setGeometry(280, 250, 300, 50);

    /* 信号与槽连接 */
    connect(fontComboBox, SIGNAL(currentFontChanged(QFont)), this,
            SLOT(fontComboBoxFontChanged(QFont)));
}

MainWindow::~MainWindow()
{
}

/* 槽函数实现 */
void MainWindow::fontComboBoxFontChanged(QFont font)
{
    /* 将label里的文本内容设置为所选择的字体 */
    label->setFont(font);

    /* 定义一个字符串接收当前项的字体 */
    QString str = "用此标签显示字体效果\n设置的字体为：" +
            fontComboBox->itemText(fontComboBox->currentIndex());

    /* 将字符串的内容作为label的显示内容 */
    label->setText(str);
}

