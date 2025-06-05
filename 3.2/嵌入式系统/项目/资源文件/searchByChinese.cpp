#include "mainwindow.h"

void MainWindow::searchByChinese()
{
    QWidget *page7 = ui->stackedWidget->widget(7);
    QTableWidget *tableWidget = page7->findChild<QTableWidget *>("search");

    if (!tableWidget)
    {
        QVBoxLayout *layout = new QVBoxLayout(page7);
        QHBoxLayout *searchLayout = new QHBoxLayout();

        QLineEdit *searchLineEdit = new QLineEdit();
        searchLineEdit->setPlaceholderText("输入中文含义进行搜索...");
        searchLineEdit->setObjectName("searchLineEdit");

        QPushButton *searchButton = new QPushButton("搜索");
        searchButton->setObjectName("searchButton");

        searchLayout->addWidget(searchLineEdit);
        searchLayout->addWidget(searchButton);

        tableWidget = new QTableWidget();
        tableWidget->setObjectName("search");
        tableWidget->setColumnCount(2);
        tableWidget->setHorizontalHeaderLabels(QStringList() << "单词" << "含义");
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // 让所有列等宽
        tableWidget->horizontalHeader()->setStretchLastSection(true);

        layout->addLayout(searchLayout);
        layout->addWidget(tableWidget);
        page7->setLayout(layout);

        connect(searchButton, &QPushButton::clicked, this, [this, searchLineEdit, tableWidget]()
                {
            QString searchText = searchLineEdit->text().trimmed();
            if (!searchText.isEmpty()) {
                performChineseSearch(searchText, tableWidget);
            } });

        connect(searchLineEdit, &QLineEdit::returnPressed, this, [this, searchLineEdit, tableWidget]()
                {
            QString searchText = searchLineEdit->text().trimmed();
            if (!searchText.isEmpty()) {
                performChineseSearch(searchText, tableWidget);
            } });
    }
}

void MainWindow::performChineseSearch(const QString &searchText, QTableWidget *tableWidget)
{
    qDebug() << "正在搜索中文含义:" << searchText;

    tableWidget->blockSignals(true);
    tableWidget->setRowCount(0);

    QSqlQuery query(db);
    query.prepare("SELECT name, explanation FROM words WHERE explanation LIKE ? ORDER BY name");
    query.addBindValue(searchText + "%"); // Prefix search

    if (!query.exec())
    {
        qDebug() << "搜索错误:" << query.lastError().text();
        qDebug() << "执行的SQL:" << query.lastQuery();
    }
    else
    {
        int row = 0;
        while (query.next())
        {
            tableWidget->insertRow(row);
            tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
            tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
            row++;
        }

        if (row == 0)
        {
            tableWidget->setRowCount(1);
            QTableWidgetItem *noDataItem = new QTableWidgetItem("未找到匹配结果");
            noDataItem->setTextAlignment(Qt::AlignCenter);
            tableWidget->setItem(0, 0, noDataItem);
            tableWidget->setSpan(0, 0, 1, 2);
        }
    }

    tableWidget->blockSignals(false);

    qDebug() << "搜索完成";
}
