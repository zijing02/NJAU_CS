#include "mainwindow.h"

#include <QStandardItemModel>
#include <QStandardItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置窗口的位置与大小 */
    this->setGeometry(0, 0, 800, 480);
    /* 实例化QTreeView对象 */
    treeView = new QTreeView(this);
    /* 居中 */
    setCentralWidget(treeView);

    /* 构建Model */
    QStandardItemModel *sdiModel = new QStandardItemModel(treeView);
    sdiModel->setHorizontalHeaderLabels(
                QStringList()<<QStringLiteral("标题")
                << QStringLiteral("名称")
                );

    for(int i = 0; i < 5; i++) {
        /* 一级标题 */
        QList<QStandardItem*> items1;
        QStandardItem* item1 =
                new QStandardItem(QString::number(i));
        QStandardItem* item2 =
                new QStandardItem(QStringLiteral("一级标题"));
        /* 添加项一 */
        items1.append(item1);
        /* 添加项二 */
        items1.append(item2);
        /* appendRow方法添加到model上 */
        sdiModel->appendRow(items1);

        for(int j = 0; j < 5; j++) {
            /* 在一级标题后面插入二级标题 */
            QList<QStandardItem*> items2;
            QStandardItem* item3 =
                    new QStandardItem(QString::number(j));
            QStandardItem* item4 =
                    new QStandardItem(QStringLiteral("二级标题"));
            items2.append(item3);
            items2.append(item4);
            /* 使用appendRow方法添加到item1上 */
            item1->appendRow(items2);
        }
    }
    /* 设置Model给treeView */
    treeView->setModel(sdiModel);
}

MainWindow::~MainWindow()
{
}

