/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   07_networkhostinfo
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-10
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 点击获取和清空文本按钮 */
    QPushButton *pushButton[2];

    /* 文本浏览框用于显示本机的信息 */
    QTextBrowser *textBrowser;

    /* 水平Widget容器和垂直Widget容器*/
    QWidget *hWidget;
    QWidget *vWidget;

    /* 水平布局和垂直布局 */
    QHBoxLayout *hBoxLayout;
    QVBoxLayout *vBoxLayout;

    /* 定时器 */
    QTimer *timer;

    /* 获取本机的网络的信息，返回类型是QString */
    QString getHostInfo();

private slots:
    /* 定时器槽函数，点击按钮后定时触发 */
    void timerTimeOut();

    /* 显示本机信息 */
    void showHostInfo();

    /* 启动定时器 */
    void timerStart();

    /* 清空textBrowser的信息 */
    void clearHostInfo();
};
#endif // MAINWINDOW_H
