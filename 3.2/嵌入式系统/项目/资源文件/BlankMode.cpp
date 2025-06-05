#include "mainwindow.h"

void MainWindow::initBlankMode()
{
    // 清空现有数据(与选择模式共享)
    allWords.clear();
    allMeanings.clear();
    displayedWords.clear();

    // 从数据库加载所有单词
    QSqlQuery query(db);
    if (!query.exec("SELECT name, explanation FROM words"))
    {
        QMessageBox::critical(this, "错误", "无法从数据库读取单词");
        return;
    }

    while (query.next())
    {
        QString word = query.value(0).toString();
        QString meaning = query.value(1).toString();
        allWords.append(qMakePair(word, meaning));
        allMeanings.append(meaning);
    }

    // 初始加载第一个单词
    loadBlankWords();
}

void MainWindow::loadBlankWords()
{

    // 重置状态
    ui->lineEdit->setStyleSheet("");  // 重置样式
    hasRecorded = false;

    // 检查是否需要重置(所有单词已显示)
    if (displayedWords.size() >= allWords.size())
    {
        QMessageBox::information(this, "提示", "所有单词已复习一遍，将重新开始");
        displayedWords.clear();
    }

    // 收集未显示的单词
    QVector<QPair<QString, QString>> availableWords;
    for (const auto &wordPair : allWords)
    {
        if (!displayedWords.contains(wordPair.first))
        {
            availableWords.append(wordPair);
        }
    }

    // 随机选择一个未显示的单词
    if (availableWords.isEmpty())
    {
        displayedWords.clear();
        availableWords = allWords;
    }

    int randomIndex = QRandomGenerator::global()->bounded(availableWords.size());
    QPair<QString, QString> currentWord = availableWords[randomIndex];
    // 获取id
    QSqlQuery idQuery(db);
    idQuery.prepare("SELECT id FROM words WHERE name = ?");
    idQuery.addBindValue(currentWord.first);
    if (idQuery.exec() && idQuery.next())
    {
        currentWordId = idQuery.value(0).toInt();
    }
    else
    {
        currentWordId = -1;
        qDebug() << "获取ID失败：" << idQuery.lastError();
    }

    // 更新按钮收藏状态
    updateCollectButton(ui->pushButton_13);

    // 显示当前单词
    ui->label_2->setText(currentWord.second);
    ui->lineEdit->setText("");
    ui->label_8->setText("");
    displayedWords.insert(currentWord.first);
}

void MainWindow::highlightBlankAnswer(QLineEdit *lineEdit, bool isCorrect)
{
    QString color = isCorrect ? "green" : "red";
    lineEdit->setStyleSheet(QString(
                              "QLineEdit {"
                              "   color: %1 !important;"
                              "   border: 2px solid %1 !important;"
                              "}")
                              .arg(color));
}

void MainWindow::on_pushButton_12_clicked() // "下一个单词"按钮
{
    loadBlankWords();
}

void MainWindow::on_pushButton_13_clicked() // "收藏"按钮
{
    toggleSaveStatus(ui->pushButton_13);
}

void MainWindow::on_pushButton_14_clicked() // "确认"按钮
{
    // 判断正误
    QSqlQuery query(db);
    QString answer;
    query.exec(QString("SELECT name FROM words WHERE id = %1").arg(currentWordId));
    if (query.next()){
        answer = query.value(0).toString();
    }
    bool isCorrect = (ui->lineEdit->text() == answer);

    // 错题记录
    if (!isCorrect)
    {
        updateErrorCount();
        ui->label_8->setText("正确答案：" + answer);
    }
    else ui->label_8->setText("回答正确！");

    // 答过题就记录
    hasRecorded = true;

    // 显示反馈
    highlightBlankAnswer(ui->lineEdit, isCorrect);

}

