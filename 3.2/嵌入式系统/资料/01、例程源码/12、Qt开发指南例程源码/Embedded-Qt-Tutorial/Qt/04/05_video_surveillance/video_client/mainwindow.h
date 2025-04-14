/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 2021-2030. All rights reserved.
* @projectName   video_client
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         dengzhimao@alientek.com
* @link          www.openedv.com
* @date          2021-11-20
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 用于接收数据 */
    QUdpSocket *udpSocket;

    /* 显示接收的图像数据 */
    QLabel *videoLabel;

    void resizeEvent(QResizeEvent *event) override;

private slots:
    /* 图像更新 */
    void videoUpdate();
};
#endif // MAINWINDOW_H
