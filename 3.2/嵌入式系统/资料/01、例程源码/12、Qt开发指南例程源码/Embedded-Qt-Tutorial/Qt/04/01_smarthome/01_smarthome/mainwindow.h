/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   01_smarthome
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-26
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QListWidget>
#include <QLayout>
#include "switchbutton.h"
#include "../webapi/webapi.h"

class SwitchButton;
class Webapi;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Webapi *webapi;

    QLabel *label[4];
    QPushButton *cloudPushButton;
    QWidget *widget[11];
    QVBoxLayout *vBoxLayout[4];
    QHBoxLayout *hBoxLayout[6];
    QPushButton *pushButton[3];
    QListWidget *listWidget;
    QLabel *itemLabel[9];
    QGridLayout *gridLayout[3];
    SwitchButton *switchButton[3];

    /* 界面布局 */
    void layoutInit();

private slots:
    void onToggled(bool);
    void deviceState(QString);
};
#endif // MAINWINDOW_H
