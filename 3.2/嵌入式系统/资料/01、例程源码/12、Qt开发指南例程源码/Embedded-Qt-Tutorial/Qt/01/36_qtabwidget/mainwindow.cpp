#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    widget = new QWidget(this);
    /* 居中 */
    this->setCentralWidget(widget);

    /* 多页面小部件 */
    tabWidget = new QTabWidget();

    /* 水平布局实例化 */
    hBoxLayout = new QHBoxLayout();
    QList <QString>strLabelList;
    strLabelList<<"标签一"<<"标签二"<<"标签三";

    QList <QString>strTabList;
    strTabList<<"页面一"<<"页面二"<<"页面三";

    QList <QString>iconList;
    iconList<<":/icons/icon1"
           <<":/icons/icon2"
          <<":/icons/icon3";

    for (int i = 0; i < 3; i++) {
        label[i] = new QLabel();
        /* 设置标签文本 */
        label[i]->setText(strLabelList[i]);
        /* 标签对齐方式（居中） */
        label[i]->setAlignment(Qt::AlignCenter);
        /* 添加页面 */
        tabWidget->addTab(label[i],
                          QIcon(iconList[i]),
                          strTabList[i]
                          );
    }
    /* 是否添加关闭按钮 */
    //tabWidget->setTabsClosable(true);
    /* 将tabWidget水平直排布 */
    hBoxLayout->addWidget(tabWidget);
    /* 将垂直布局设置到widget */
    widget->setLayout(hBoxLayout);
}

MainWindow::~MainWindow()
{
}

