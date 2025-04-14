#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    widget = new QWidget(this);
    /* 居中widget */
    this->setCentralWidget(widget);

    /* 实例化对象 */
    vSpacer  =  new QSpacerItem(10, 10,
                                QSizePolicy::Minimum,
                                QSizePolicy::Expanding
                                );
    hSpacer  =  new QSpacerItem(10, 10,
                                QSizePolicy::Expanding,
                                QSizePolicy::Minimum
                                );

    vBoxLayout = new QVBoxLayout();
    hBoxLayout = new QHBoxLayout();
    mainLayout = new QHBoxLayout();

    /* 在vBoxLayout添加垂直间隔 */
    vBoxLayout->addSpacerItem(vSpacer);

    QList <QString>list;
    /* 将字符串值插入list */
    list<<"按钮1"<<"按钮2"<<"按钮3"<<"按钮4";
    /* 用一个循环实例化4个按钮 */
    for(int i = 0; i < 4 ; i++){
        bt[i] = new QPushButton();
        bt[i]->setText(list[i]);
        if (i == 0){
            /* 按钮1,设置为100*100 */
            bt[i]->setFixedSize(100, 100);
            /* 在vBoxLayout添加按钮1 */
            vBoxLayout->addWidget(bt[i]);
        } else {
            /* 按钮2～4,设置为60*60 */
            bt[i]->setFixedSize(60, 60);
            /* 在hBoxLayout添加按钮2～4 */
            hBoxLayout->addWidget(bt[i]);
        }
    }
    /* 在hBoxLayout添加水平间隔 */
    hBoxLayout->addSpacerItem(hSpacer);

    /* 在主布局里添加垂直布局 */
    mainLayout->addLayout(vBoxLayout);
    /* 在主布局里添加水平布局 */
    mainLayout->addLayout(hBoxLayout);

    /* 设置部件间距 */
    mainLayout->setSpacing(50);
    /* 将主布局设置为widget的布局 */
    widget->setLayout(mainLayout);

}

MainWindow::~MainWindow()
{
}

