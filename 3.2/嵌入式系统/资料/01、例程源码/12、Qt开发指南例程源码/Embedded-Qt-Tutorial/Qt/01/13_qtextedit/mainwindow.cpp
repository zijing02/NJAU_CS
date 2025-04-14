#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体显示的位置和大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例和对象，设置位置和显示大小 */
    textEdit = new QTextEdit(this);
    textEdit->setGeometry(0, 0, 800, 400);

    /* 实例和对象，设置位置和显示大小，设置文本 */
    pushButtonSelectAll = new QPushButton(this);
    pushButtonSelectAll->setGeometry(200, 420, 50, 20);
    pushButtonSelectAll->setText("全选");

    /* 实例和对象，设置位置和显示大小，设置文本 */
    pushButtonClearAll = new QPushButton(this);
    pushButtonClearAll->setGeometry(500, 420, 50, 20);
    pushButtonClearAll->setText("清除");

    /* 信号槽连接 */
    connect(pushButtonSelectAll, SIGNAL(clicked()), this,
            SLOT(pushButtonSelectAllClicked()));
    connect(pushButtonClearAll, SIGNAL(clicked()), this,
            SLOT(pushButtonClearAllClicked()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::pushButtonSelectAllClicked()
{
    /* 设置焦点为textEdit */
    textEdit->setFocus();
    /* 判断文本编辑框内容是否为空，不为空则全选 */
    if(!textEdit->toPlainText().isEmpty()){
        /* 全选 */
        textEdit->selectAll();
    }
}

void MainWindow::pushButtonClearAllClicked()
{
    /* 清空textEdit里的文本内容 */
    textEdit->clear();
}

