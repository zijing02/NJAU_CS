#include "mainwindow.h"

void MainWindow::loadInformation()
{
    QWidget *page6 = ui->stackedWidget->widget(6);
    QTableWidget *tableWidget = page6->findChild<QTableWidget *>("words");
    if (!tableWidget)
    {
        QVBoxLayout *layout = new QVBoxLayout(page6);
        tableWidget = new QTableWidget();
        tableWidget->setObjectName("words");
        tableWidget->setColumnCount(4);
        tableWidget->setHorizontalHeaderLabels(QStringList() << "单词" << "含义" << "错误次数" << "是否已经收藏");
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 让所有列等宽
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(tableWidget);
        page6->setLayout(layout);
        tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
        connect(tableWidget, &QTableWidget::cellChanged, this,
                [this, tableWidget](int row, int column)
                {
                    changeSaveTime(row, column, tableWidget, 1);
                });
        connect(tableWidget, &QTableWidget::cellChanged, this,
                [this, tableWidget](int row, int column)
                {
                    changeWrongTimes(row, column, tableWidget);
                });
    }

    qDebug() << "加载所有信息";
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(0);
    QSqlQuery query(db);
    QString sql = "SELECT name,explanation,error,save FROM words";
    if (!query.exec(sql))
    {
        qDebug() << "Error: " << query.lastError().text();
        qDebug() << "Executed SQL: " << query.lastQuery();
    }
    else
    {
        int row = 0;
        while (query.next())
        {
            qDebug() << query.value(0).toString() << "|" << query.value(1).toString() << "|" << query.value(2).toString() << "|" << query.value(3).toString();
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));
            tableWidget->setItem(row, 3, new QTableWidgetItem(query.value(3).toString()));
            row++;
        }
        if (row == 0)
        {
            tableWidget->setRowCount(1);
            QTableWidgetItem *noDataItem = new QTableWidgetItem("暂无信息");
            noDataItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(0, 0, noDataItem);
            tableWidget->setSpan(0, 0, 1, 2);
        }
    }
    tableWidget->blockSignals(false);

    qDebug() << "结束加载所有信息";
}
