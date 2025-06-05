#include "mainwindow.h"

void MainWindow::changeSaveTime(int row, int column, QTableWidget *tableWidget, int pos)
{
    if ((column == 2 && pos == 0) || (column == 3 && pos == 1))
    {
        QString word = tableWidget->item(row, 0)->text();
        QString newSave = tableWidget->item(row, 2)->text();
        bool ok = false;
        int saveValue = newSave.toInt(&ok);
        if (!ok || saveValue != 0)
        {
            QMessageBox::warning(this, "无效输入", QString("单词：%1 已经收藏").arg(word));
            tableWidget->blockSignals(true);
            QSqlQuery query;
            query.prepare("SELECT save FROM words WHERE name = ?");
            query.addBindValue(word);
            if (query.exec() && query.next())
            {
                tableWidget->setItem(row, 2, new QTableWidgetItem(query.value(0).toString()));
            }
            tableWidget->blockSignals(false);
            return;
        }
        QMessageBox::information(this, "有效输入", QString("单词：%1 已经取消收藏").arg(word));
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE words SET save = ? WHERE name = ?");
        updateQuery.addBindValue(newSave);
        updateQuery.addBindValue(word);
        if (!updateQuery.exec())
        {
            qDebug() << "更新数据库失败:" << updateQuery.lastError().text();
        }
        else
        {
            qDebug() << "已同步到数据库:" << word << "已经取消收藏";
        }
        loadSaveWords();
    }
}

void MainWindow::loadSaveWords()
{
    QWidget *page5 = ui->stackedWidget->widget(5);
    QTableWidget *tableWidget = page5->findChild<QTableWidget *>("saveWordsTable");
    QPushButton *cancelAllButton = page5->findChild<QPushButton *>("cancelAllSaveButton");

    if (!tableWidget)
    {
        QVBoxLayout *layout = new QVBoxLayout(page5);
        cancelAllButton = new QPushButton("一键取消收藏", page5);
        cancelAllButton->setObjectName("cancelAllSaveButton");
        layout->addWidget(cancelAllButton);
        tableWidget = new QTableWidget();
        tableWidget->setObjectName("saveWordsTable");
        tableWidget->setColumnCount(3);
        tableWidget->setHorizontalHeaderLabels(QStringList() << "单词" << "含义" << "取消收藏");
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 让所有列等宽
        tableWidget->horizontalHeader()->setStretchLastSection(true);
        layout->addWidget(tableWidget);
        page5->setLayout(layout);
        tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked);
        connect(tableWidget, &QTableWidget::cellChanged, this,
                [this, tableWidget](int row, int column)
                {
                    changeSaveTime(row, column, tableWidget, 0);
                });
        connect(cancelAllButton, &QPushButton::clicked, this, [this, tableWidget]()
                {
                    if (QMessageBox::question(this, "确认操作", "确定要将所有收藏天数设置为0吗？") == QMessageBox::Yes) {
                        QSqlQuery query;
                        if (!query.exec("UPDATE words SET save = 0 WHERE save != 0")) {
                            QMessageBox::warning(this, "操作失败", "批量取消收藏失败！");
                        } else {
                            QMessageBox::information(this, "操作成功", "所有收藏已取消！");
                            loadSaveWords();
                        }
                    } });
        loadSaveWords();
    }

    qDebug() << "加载收藏单词";
    qDebug() << "当前工作路径:" << QDir::currentPath();
    QString dbPath = QSqlDatabase::database().databaseName();
    QFileInfo dbInfo(dbPath);
    qDebug() << "数据库完整路径:" << dbInfo.absoluteFilePath();
    tableWidget->blockSignals(true);
    tableWidget->setRowCount(0);
    QSqlQuery query(db);
    QString sql = "SELECT name, explanation, save FROM words WHERE save != 0";
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
            qDebug() << query.value(0).toString() << "|" << query.value(1).toString();
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));

            QPushButton *cancelBtn = new QPushButton("取消收藏");
            tableWidget->setCellWidget(row, 2, cancelBtn);
            connect(cancelBtn, &QPushButton::clicked, this, [this, tableWidget, row]()
                    {
                           QString word = tableWidget->item(row, 0)->text();
                           QSqlQuery updateQuery;
                           updateQuery.prepare("UPDATE words SET save = 0 WHERE name = ?");
                           updateQuery.addBindValue(word);
                           if (!updateQuery.exec()) {
                               QMessageBox::warning(this, "操作失败", "取消收藏失败！");
                           } else {
                               QMessageBox::information(this, "操作成功", QString("单词：%1 已取消收藏！").arg(word));
                               loadSaveWords();
                           } });
            row++;
        }
        if (row == 0)
        {
            tableWidget->setRowCount(1);
            QTableWidgetItem *noDataItem = new QTableWidgetItem("暂无收藏");
            noDataItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(0, 0, noDataItem);
            tableWidget->setSpan(0, 0, 1, 2);
        }
    }
    tableWidget->blockSignals(false);

    qDebug() << "结束加载收藏夹";
}
