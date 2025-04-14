#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的大小与位置 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化 */
    tableWidget = new QTableWidget(this);
    /* 设置tableWidget表居中 */
    setCentralWidget(tableWidget);
    /* 设置列数 */
    tableWidget->setColumnCount(2);
    /* 设置行数 */
    tableWidget->setRowCount(2);
    /* 使用标签设置水平标题标签 */
    tableWidget->setHorizontalHeaderLabels(
                QStringList()<<"姓名"<<"性别"
                );

    /* 字符串类型链表 */
    QList <QString> strList;
    strList<<"小明"<<"小红"<<"男"<<"女";

    for (int i = 0; i < 4; i++) {
        /* 实例化 */
        tableWidgetItem[i] = new QTableWidgetItem(strList[i]);
        /* 设置文本居中对齐 */
        tableWidgetItem[i]->setTextAlignment(Qt::AlignCenter);
    }
    /* 插入数据，表的index就是一个二维数组数据 */
    tableWidget->setItem(0, 0, tableWidgetItem[0]);
    tableWidget->setItem(1, 0, tableWidgetItem[1]);
    tableWidget->setItem(0, 1, tableWidgetItem[2]);
    tableWidget->setItem(1, 1, tableWidgetItem[3]);

}

MainWindow::~MainWindow()
{
}

