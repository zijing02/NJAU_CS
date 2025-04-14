#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗口位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化 */
    listView = new QListView(this);
    /* 将listView居中 */
    setCentralWidget(listView);

    QStringList strList;
    strList<<"高三（1）班"<<"高三（2）班"<<"高三（3）班";

    /* 实例化，字符串模型 */
    stringListModel = new QStringListModel(strList);

    /* 向表中插入一段数据 */
    listView->setModel(stringListModel);
    /* 设置为视图为图标模式 */
    listView->setViewMode(QListView::IconMode);
    /* 设置为不可拖动 */
    listView->setDragEnabled(false);
}

MainWindow::~MainWindow()
{
}

