/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   12_imagedownload
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-16
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    /* 网络管理 */
    QNetworkAccessManager *networkAccessManager;

    /* 标签 */
    QLabel *label[3];

    /* 按钮 */
    QPushButton *pushButton;

    /* 下载进度条 */
    QProgressBar *progressBar;

    /* 水平布局 */
    QHBoxLayout *hBoxLayout[2];

    /* 垂直布局 */
    QVBoxLayout *vBoxLayout;

    /* 水平容器 */
    QWidget *hWidget[2];

    /* 垂直容器 */
    QWidget *vWidget;

    /* 链接输入框 */
    QLineEdit *lineEdit;

private slots:
    /* 读取数据 */
    void readyReadData();

    /* 读取数据 */
    void readyReadData(QNetworkReply*);

    /* 响应完成处理 */
    void replyFinished();

    /* 下载进度管理 */
    void imageDownloadProgress(qint64, qint64);

    /* 点击开始下载 */
    void startDownload();

    /* 响应错误处理函数 */
    void networkReplyError(QNetworkReply::NetworkError);
};
#endif // MAINWINDOW_H
