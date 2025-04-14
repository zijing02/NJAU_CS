#include "mainwindow.h"
/* 引入QDebug */
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化对象 */
    comboBox = new QComboBox(this);

    /* 设置comboBox的显示位置与大小 */
    comboBox->setGeometry(300, 200, 150, 30);

    /* 添加项，我们添加三个省份，作为comboBox的三个选项 */
    comboBox->addItem("广东(默认)");
    comboBox->addItem("湖南");
    comboBox->addItem("四川");

    /* 信号槽连接 */
    connect(comboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(comboBoxIndexChanged(int)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::comboBoxIndexChanged(int index)
{
    /* 打印出选择的省份 */
    qDebug()<<"您选择的省份是"<< comboBox->itemText(index)<<endl;
}

