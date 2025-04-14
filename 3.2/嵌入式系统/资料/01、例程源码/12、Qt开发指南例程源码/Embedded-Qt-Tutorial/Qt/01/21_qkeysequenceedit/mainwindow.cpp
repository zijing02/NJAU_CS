#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 主窗体设置位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化 */
    keySequenceEdit = new QKeySequenceEdit(this);

    /* 设置位置与大小 */
    keySequenceEdit->setGeometry(350, 200, 150, 30);

    /* 信号槽连接 */
    connect(keySequenceEdit,
            SIGNAL(keySequenceChanged(const QKeySequence &)),
            this,
            SLOT(KSEKeySequenceChanged(const QKeySequence &))
            );

}

MainWindow::~MainWindow()
{
}

void MainWindow::KSEKeySequenceChanged(
        const QKeySequence &keySequence)
{
    /* 判断输入的组合键是否为Ctrl + Q，如果是则退出程序 */
    if(keySequence == QKeySequence(tr("Ctrl+Q"))) {
        /* 结束程序 */
        this->close();
    }else {
        /* 打印出按下的组合键 */
        qDebug()<<keySequence.toString()<<endl;
    }
}

