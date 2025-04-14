#include "mainwindow.h"
#include <QDir>
#include <QTextStream>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置当前程序的工作目录为可执行程序的工作目录 */
    QDir::setCurrent(QCoreApplication::applicationDirPath());

    this->setGeometry(0, 0, 800, 480);

    plainTextEdit = new QPlainTextEdit(this);
    plainTextEdit->setGeometry(0, 50, 800, 430);

    radioButton = new QRadioButton(this);
    radioButton->setGeometry(650, 20, 100, 20);
    radioButton->setText("只读模式");

    /* 打开可执行程序目录里的moc_mainwindow.cpp，注意如果是Windows下
   moc_mainwindow.cpp并不在当前目录，而在上一级目录"../moc_mainwindow.cpp"*/
    QFile file("moc_mainwindow.cpp");

    /* 以只读模式打开，但是可以在plainTextEdit里编辑 */
    file.open((QFile::ReadOnly | QFile::Text));

    /* 加载到文件流 */
    QTextStream in(&file);

    /* 从文本流中读取全部 */
    plainTextEdit->insertPlainText(in.readAll());

    /* 信号槽连接 */
    connect(radioButton, SIGNAL(clicked()), this,
            SLOT(radioButtonClicked()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::radioButtonClicked()
{
    /* 检查radioButton是否选中 */
    if(radioButton->isChecked()) {
        /* 设置为只读模式 */
        plainTextEdit->setReadOnly(true);
    } else {
        /* 设置为非只读模式 */
        plainTextEdit->setReadOnly(false);
    }
}

