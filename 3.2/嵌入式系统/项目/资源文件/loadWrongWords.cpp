#include "mainwindow.h"

void MainWindow::changeWrongTimes(int row, int column, QTableWidget *tableWidget)
{
    if (column == 2)
    {
        QString word = tableWidget->item(row, 0)->text();
        QString newError = tableWidget->item(row, 2)->text();
        bool ok = false;
        int saveValue = newError.toInt(&ok);
        if (!ok)
        {
            QMessageBox::warning(this, "无效输入", "请输入数字！");
        }
        if (saveValue == 0)
        {
            QMessageBox::information(this, "完成操作", QString("单词：%1 已经设置正确单词").arg(word));
        }
        else if (saveValue != 0)
        {
            QMessageBox::information(this, "完成操作", QString("单词：%1 已经设置错误次数为：%2").arg(word).arg(saveValue));
        }
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE words SET error = ? WHERE name = ?");
        updateQuery.addBindValue(saveValue);
        updateQuery.addBindValue(word);
        if (!updateQuery.exec())
        {
            qDebug() << "更新数据库失败:" << updateQuery.lastError().text();
        }
        else
        {
            qDebug() << "已同步到数据库:" << word << " 错误次数=" << saveValue;
        }
        loadWrongWords();
    }
}

void MainWindow::loadWrongWords()
{
    QWidget *page4 = ui->stackedWidget->widget(4);
    QTableWidget *tableWidget = page4->findChild<QTableWidget *>("wrongWordsTable");
    QPushButton *cancelAllButton = page4->findChild<QPushButton *>("cancelAllSaveButton");

    // 初始化界面，如果存在则不在建立
    if (!tableWidget)
    {
        QVBoxLayout *layout = new QVBoxLayout(page4);
        cancelAllButton = new QPushButton("一键正确", page4);
        cancelAllButton->setObjectName("cancelAllSaveButton");
        layout->addWidget(cancelAllButton);
        tableWidget = new QTableWidget();
        tableWidget->setObjectName("wrongWordsTable");
        tableWidget->setColumnCount(4);
        tableWidget->setHorizontalHeaderLabels(QStringList() << "单词" << "含义" << "错误次数" << "设置为正确");
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 让所有列等宽
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(tableWidget);
        page4->setLayout(layout);
        tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
        // 修改的槽函数，当输入新的错误次数时可以对数据库的内容进行更新

        connect(tableWidget, &QTableWidget::cellChanged, this,
                [this, tableWidget](int row, int column)
                {
                    changeWrongTimes(row, column, tableWidget);
                });
        connect(cancelAllButton, &QPushButton::clicked, this, [this, tableWidget]()
                {
                    if (QMessageBox::question(this, "确认操作", "确定要将所有单词设置为正确吗？") == QMessageBox::Yes) {
                        QSqlQuery query;
                        if (!query.exec("UPDATE words SET error = 0 WHERE error != 0")) {
                            QMessageBox::warning(this, "操作失败", "批量设置正确失败！");
                        } else {
                            QMessageBox::information(this, "操作成功", "所有错误单词已设置正确！");
                            loadSaveWords();
                        }
                    } });
    }
    qDebug() << "加载错误单词";
    // 加载之前锁定数据库
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(0);
    QSqlQuery query(db);
    QString sql = "SELECT name, explanation, error FROM words WHERE error != 0";
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
            // 将信息输出到终端以及界面
            qDebug() << query.value(0).toString() << "|" << query.value(1).toString() << "|" << query.value(2).toString();
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(2).toString()));

            QPushButton *cancelBtn = new QPushButton("设置正确");
            tableWidget->setCellWidget(row, 3, cancelBtn);

            connect(cancelBtn, &QPushButton::clicked, this, [this, tableWidget, row]()
                    {
                           QString word = tableWidget->item(row, 0)->text();
                           QSqlQuery updateQuery;
                           updateQuery.prepare("UPDATE words SET error = 0 WHERE name = ?");
                           updateQuery.addBindValue(word);
                           if (!updateQuery.exec()) {
                               QMessageBox::warning(this, "操作失败", "设置正确失败！");
                           } else {
                               QMessageBox::information(this, "操作成功", QString("单词：%1 已设置正确！").arg(word));
                               loadWrongWords();
                           } });
            row++;
        }
        if (row == 0)
        {
            tableWidget->setRowCount(1);
            QTableWidgetItem *noDataItem = new QTableWidgetItem("暂无错题");
            noDataItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(0, 0, noDataItem);
            tableWidget->setSpan(0, 0, 1, 2); // 合并单元格
        }
    }
    // 加载之后释放数据库
    tableWidget->blockSignals(false);
    qDebug() << "结束加载错误单词";
}
