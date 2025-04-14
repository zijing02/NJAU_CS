/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   03_serialport
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-12
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QPushButton>
#include <QTextBrowser>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include <QMessageBox>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 串口对象 */
    QSerialPort *serialPort;

    /* 用作接收数据 */
    QTextBrowser *textBrowser;

    /* 用作发送数据 */
    QTextEdit *textEdit;

    /* 按钮 */
    QPushButton *pushButton[2];

    /* 下拉选择盒子 */
    QComboBox *comboBox[5];

    /* 标签 */
    QLabel *label[5];

    /* 垂直布局 */
    QVBoxLayout *vboxLayout;

    /* 网络布局 */
    QGridLayout *gridLayout;

    /* 主布局 */
    QWidget *mainWidget;

    /* 设置功能区域 */
    QWidget *funcWidget;

    /* 布局初始化 */
    void layoutInit();

    /* 扫描系统可用串口 */
    void scanSerialPort();

    /* 波特率项初始化 */
    void baudRateItemInit();

    /* 数据位项初始化 */
    void dataBitsItemInit();

    /* 检验位项初始化 */
    void parityItemInit();

    /* 停止位项初始化 */
    void stopBitsItemInit();

private slots:
    void sendPushButtonClicked();
    void openSerialPortPushButtonClicked();
    void serialPortReadyRead();
};
#endif // MAINWINDOW_H
