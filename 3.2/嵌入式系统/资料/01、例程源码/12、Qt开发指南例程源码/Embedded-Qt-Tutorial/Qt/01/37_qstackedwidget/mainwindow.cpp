#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    /* widget小部件实例化 */
    widget = new QWidget(this);

    /* 设置居中 */
    this->setCentralWidget(widget);

    /* 垂直布局实例化 */
    hBoxLayout = new QHBoxLayout();

    /* 堆栈部件实例化 */
    stackedWidget = new QStackedWidget();

    /* 列表实例化 */
    listWidget = new QListWidget();

    QList <QString>strListWidgetList;
    strListWidgetList<<"窗口一"<<"窗口二"<<"窗口三";

    for (int i = 0; i < 3; i++){
        /* listWidget插入项 */
        listWidget->insertItem(
                    i,
                    strListWidgetList[i]
                    );
    }

    QList <QString>strLabelList;
    strLabelList<<"标签一"<<"标签二"<<"标签三";

    for (int i = 0; i < 3; i++){
        label[i] = new QLabel();
        /* 设置标签文本 */
        label[i]->setText(strLabelList[i]);
        /* 标签对齐方式（居中） */
        label[i]->setAlignment(Qt::AlignCenter);
        /* 添加页面 */
        stackedWidget->addWidget(label[i]);
    }

    /* 设置列表的最大宽度 */
    listWidget->setMaximumWidth(200);
    /* 添加到水平布局 */
    hBoxLayout->addWidget(listWidget);
    hBoxLayout->addWidget(stackedWidget);

    /* 将widget的布局设置成hboxLayout */
    widget->setLayout(hBoxLayout);

    /* 利用listWidget的信号函数currentRowChanged()与
     * 槽函数setCurrentIndex(),进行信号与槽连接
     */
    connect(listWidget, SIGNAL(currentRowChanged(int)),
            stackedWidget, SLOT(setCurrentIndex(int)));
}

MainWindow::~MainWindow()
{
}

