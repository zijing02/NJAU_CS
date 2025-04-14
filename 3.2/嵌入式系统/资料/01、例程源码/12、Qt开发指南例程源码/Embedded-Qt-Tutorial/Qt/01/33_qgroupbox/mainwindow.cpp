#include "mainwindow.h"
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体位置与大小 */
    this->setGeometry(0, 0, 800, 480);
    /* 以标题为“QGroupBox示例”实例化groupBox对象 */
    groupBox = new QGroupBox(tr("QGroupBox示例"), this);
    groupBox->setGeometry(300, 100, 300, 200);

    vBoxLayout = new QVBoxLayout();

    /* 字符串链表 */
    QList <QString>list;
    list<<"选项一"<<"选项二"<<"选项三";
    for(int i = 0; i < 3; i++){
        radioButton[i] = new QRadioButton();
        radioButton[i]->setText(list[i]);
        /* 在vBoxLayout添加radioButton */
        vBoxLayout->addWidget(radioButton[i]);
    }
    /* 添加一个伸缩量1 */
    vBoxLayout->addStretch(1);
    /* vBoxLayout布局设置为groupBox布局 */
    groupBox->setLayout(vBoxLayout);
}

MainWindow::~MainWindow()
{
}

