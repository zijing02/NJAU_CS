#include "mainwindow.h"
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);
    QStandardItemModel *model = new QStandardItemModel;

    /* 省份 */
    QStandardItem *province = new QStandardItem("广东省");

    /* 城市 */
    QStandardItem *city1 = new QStandardItem("茂名市");
    QStandardItem *city2 = new QStandardItem("中山市");

    /* 添加城市到省份下 */
    province->appendRow(city1);
    province->appendRow(city2);

    QStandardItem *town1 = new QStandardItem("电白镇");
    QStandardItem *town2 = new QStandardItem("南头镇");

    /* 添加城镇到城市下 */
    city1->appendRow(town1);
    city2->appendRow(town2);

    columnView = new QColumnView;

    /* 建立model */
    model->appendRow(province);

    /* 设置model */
    columnView->setModel(model);

    /* 设置居中 */
    setCentralWidget(columnView);
}

MainWindow::~MainWindow()
{
}

