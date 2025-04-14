/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 2021-2030. All rights reserved.
* @projectName   04_lpr_demo
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         dengzhimao@alientek.com
* @link          www.openedv.com
* @date          2021-11-17
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../ocr/ocr.h"
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 自定义的车牌识别类 */
    Ocr *ocr;

    /* 按钮，用于点击开始识别车牌 */
    QPushButton *pushButton;

    /* 标签，用于显示车牌结果 */
    QLabel *resultLabel;

public slots:

    /* 按钮点击开始获取车牌结果 */
    void pushButtonClicked();

    /* 显示车牌结果 */
    void disPlaylprResult(QString);
};
#endif // MAINWINDOW_H
