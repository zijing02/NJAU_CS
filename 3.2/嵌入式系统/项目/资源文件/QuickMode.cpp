#include "mainwindow.h"

void MainWindow::initQuickMode()
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
    loadQuickWords();
}

void MainWindow::loadQuickWords()
{

    // 重置状态
    needRedisplay = false;

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
    updateCollectButton(ui->pushButton_28);

    // 显示当前单词
    ui->label_11->setText(currentWord.first);
    ui->label_10->setText("");
}

void MainWindow::on_pushButton_27_clicked() // "下一个单词"按钮
{
    QSqlQuery query(db);
    QString name;
    query.exec(QString("SELECT name FROM words WHERE id = %1").arg(currentWordId));
    if (query.next()){
        name = query.value(0).toString();
    }
    if(!needRedisplay)displayedWords.insert(name);
    loadQuickWords();
}

void MainWindow::on_pushButton_28_clicked() // "收藏"按钮
{
    toggleSaveStatus(ui->pushButton_28);
}

void MainWindow::on_pushButton_29_clicked() // "认识"按钮
{
    needRedisplay=false;
    QSqlQuery query(db);
    QString name;
    query.exec(QString("SELECT name FROM words WHERE id = %1").arg(currentWordId));
    if (query.next()){
        name = query.value(0).toString();
    }
    displayedWords.insert(name);
    loadQuickWords();
}

void MainWindow::on_pushButton_30_clicked() // "不认识"按钮
{
    needRedisplay=true;
    QSqlQuery query(db);
    QString explanation;
    query.exec(QString("SELECT explanation FROM words WHERE id = %1").arg(currentWordId));
    if (query.next()){
        explanation = query.value(0).toString();
    }
    ui->label_10->setText(explanation);
}

