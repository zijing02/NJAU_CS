/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   05_qthread_example1
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-06
*******************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 对象实例化 */
    pushButton = new QPushButton(this);
    workerThread = new WorkerThread(this);

    /* 按钮设置大小与文本 */
    pushButton->resize(100, 40);
    pushButton->setText("开启线程");

    /* 信号槽连接 */
    connect(workerThread, SIGNAL(resultReady(QString)),
            this, SLOT(handleResults(QString)));
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
    /* 进程退出，注意本例run()方法没写循环，此方法需要有循环才生效 */
    workerThread->quit();

    /* 阻塞等待2000ms检查一次进程是否已经退出 */
    if (workerThread->wait(50)) {
        qDebug()<<"线程已经结束！"<<endl;
    }
}

void MainWindow::handleResults(const QString &result)
{
    /* 打印出线程发送过来的结果 */
    qDebug()<<result<<endl;
}

void MainWindow::pushButtonClicked()
{
    /* 检查线程是否在运行，如果没有则开始运行 */
    if (!workerThread->isRunning())
        workerThread->start();
}


