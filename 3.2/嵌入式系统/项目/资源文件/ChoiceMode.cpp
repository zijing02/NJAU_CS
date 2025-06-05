#include "mainwindow.h"

void MainWindow::initChoiceMode()
{
    // 清空现有数据
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
    loadChoiceWords();
}

void MainWindow::loadChoiceWords()
{

    // 重置状态
    resetButtonColors();
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
    updateCollectButton(ui->pushButton_11);

    // 显示当前单词
    ui->label_7->setText(currentWord.first);
    displayedWords.insert(currentWord.first);

    // 准备选项(1正确+3干扰)
    QVector<QString> options;
    options.append(currentWord.second);

    // 生成3个不同的干扰项
    QVector<QString> wrongMeanings = allMeanings;
    wrongMeanings.removeAll(currentWord.second); // 移除正确答案

    // 随机选择3个干扰项
    for (int i = 0; i < 3 && !wrongMeanings.isEmpty(); ++i)
    {
        int randomIndex = QRandomGenerator::global()->bounded(wrongMeanings.size());
        options.append(wrongMeanings.takeAt(randomIndex));
    }

    // 打乱选项顺序
    std::random_shuffle(options.begin(), options.end());

    // 保存正确答案位置
    correctAnswerIndex = options.indexOf(currentWord.second) + 1;

    // 更新按钮
    ui->choice_1->setText(options[0]);
    ui->choice_2->setText(options[1]);
    ui->choice_3->setText(options[2]);
    ui->choice_4->setText(options[3]);
}

void MainWindow::updateErrorCount() // 错题记录
{
    if (hasRecorded)

        return;

    QSqlQuery query(db);
    query.exec(QString("UPDATE words SET error = error + 1 WHERE id = %1").arg(currentWordId));
}

void MainWindow::toggleSaveStatus(QPushButton *button) // 收藏功能
{
    QSqlQuery query(db);
    query.exec(QString("SELECT save FROM words WHERE id = %1").arg(currentWordId));
    if (query.next())
    {
        int currentSave = query.value(0).toInt();
        int newSave = currentSave ^ 1; // 异或切换0/1

        query.exec(QString("UPDATE words SET save = %1 WHERE id = %2")
                       .arg(newSave)
                       .arg(currentWordId));

        updateCollectButton(button);
    }
}

void MainWindow::updateCollectButton(QPushButton *button) // 收藏按钮外观更新
{
    QSqlQuery query(db);
    query.exec(QString("SELECT save FROM words WHERE id = %1").arg(currentWordId));
    if (query.next())
    {
        bool isSaved = query.value(0).toBool();
        button->setText(isSaved ? "已收藏" : "收藏");
        button->setStyleSheet(isSaved ? "color: #FFD700;" : "");
    }
}

void MainWindow::on_pushButton_10_clicked() // "下一个单词"按钮
{
    loadChoiceWords();
}

void MainWindow::on_pushButton_11_clicked() // "收藏"按钮
{
    toggleSaveStatus(ui->pushButton_11);
}

/* 答题反馈实现 */
void MainWindow::onChoiceButtonClicked()
{
    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (!clickedButton)
        return;

    // 其他按钮恢复默认
    resetButtonColors();

    // 判断正误
    bool isCorrect = (clickedButton == getButtonByIndex(correctAnswerIndex));

    // 错题记录
    if (!isCorrect)
    {
        updateErrorCount();
    }

    // 答过题就记录
    hasRecorded = true;

    // 显示反馈
    highlightAnswer(clickedButton, isCorrect);
}

void MainWindow::highlightAnswer(QPushButton *button, bool isCorrect)
{
    QString color = isCorrect ? "green" : "red";
    // 使用 !important 强制覆盖其他样式
    button->setStyleSheet(QString(
                              "QPushButton {"
                              "   color: %1 !important;"
                              "   border: 2px solid %1 !important;"
                              "}")
                              .arg(color));
}

void MainWindow::resetButtonColors()
{
    QList<QPushButton *> buttons = {ui->choice_1, ui->choice_2, ui->choice_3, ui->choice_4};
    foreach (QPushButton *btn, buttons)
    {
        btn->setStyleSheet("color: black; font-weight: normal;");
    }
}

QPushButton *MainWindow::getButtonByIndex(int index)
{
    switch (index)
    {
    case 1:
        return ui->choice_1;
    case 2:
        return ui->choice_2;
    case 3:
        return ui->choice_3;
    case 4:
        return ui->choice_4;
    default:
        return nullptr;
    }
}
