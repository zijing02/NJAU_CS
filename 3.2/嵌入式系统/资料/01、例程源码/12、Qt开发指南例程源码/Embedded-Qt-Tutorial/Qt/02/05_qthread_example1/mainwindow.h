/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   05_qthread_example1
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-06
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QDebug>
#include <QPushButton>

/* 使用下面声明的WorkerThread线程类 */
class WorkerThread;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 在MainWindow类里声明对象 */
    WorkerThread *workerThread;

    /* 声明一个按钮，使用此按钮点击后开启线程 */
    QPushButton *pushButton;

private slots:
    /* 槽函数，用于接收线程发送的信号 */
    void handleResults(const QString &result);

    /* 点击按钮开启线程 */
    void pushButtonClicked();
};

/* 新建一个WorkerThread类继承于QThread */
class WorkerThread : public QThread
{
    /* 用到信号槽即需要此宏定义 */
    Q_OBJECT

public:
    WorkerThread(QWidget *parent = nullptr) {
        Q_UNUSED(parent);
    }

    /* 重写run方法，继承QThread的类，只有run方法是在新的线程里 */
    void run() override {
        QString result = "线程开启成功";

        /* 这里写上比较耗时的操作 */
        // ...
        // 延时2s，把延时2s当作耗时操作
        sleep(2);

        /* 发送结果准备好的信号 */
        emit resultReady(result);
    }

signals:
    /* 声明一个信号，译结果准确好的信号 */
    void resultReady(const QString &s);
};

#endif // MAINWINDOW_H
