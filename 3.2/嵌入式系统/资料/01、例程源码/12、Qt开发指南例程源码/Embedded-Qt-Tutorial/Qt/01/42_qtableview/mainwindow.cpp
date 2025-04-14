#include "mainwindow.h"
#include <QStandardItemModel>
#include <QHeaderView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置窗口的位置与大小 */
    this->setGeometry(0, 0, 800, 480);
    tableView = new QTableView(this);
    setCentralWidget(tableView);
    /* 显示网格线 */
    tableView->setShowGrid(true);

    QStandardItemModel* model = new QStandardItemModel();
    QStringList labels =
            QObject::tr("语文,数学,英语").simplified().split(",");
    /* 设置水平头标签 */
    model->setHorizontalHeaderLabels(labels);

    /* item */
    QStandardItem* item = 0;
    /* model插入项内容 */
    for(int i = 0; i < 5; i++){
        item = new QStandardItem("80");
        model->setItem(i, 0, item);
        item = new QStandardItem("99");
        model->setItem(i, 1, item);
        item = new QStandardItem("100");
        model->setItem(i, 2, item);
    }
    /* 将model设置给tableView */
    tableView->setModel(model);
    /* 平均分列 */
    tableView->horizontalHeader()
            ->setSectionResizeMode(QHeaderView::Stretch);
    /* 平均分行 */
    tableView->verticalHeader()
            ->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
}

