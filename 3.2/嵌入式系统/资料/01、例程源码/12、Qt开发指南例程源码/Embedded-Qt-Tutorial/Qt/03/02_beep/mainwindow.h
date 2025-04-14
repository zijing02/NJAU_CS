/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   02_beep
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-11
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QFile>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 按钮 */
    QPushButton *pushButton;

    /* 文件 */
    QFile file;

    /* 设置BEEP的状态 */
    void setBeepState();

    /* 获取BEEP的状态 */
    bool getBeepState();

private slots:
    /* 槽函数 */
    void pushButtonClicked();
};
#endif // MAINWINDOW_H
