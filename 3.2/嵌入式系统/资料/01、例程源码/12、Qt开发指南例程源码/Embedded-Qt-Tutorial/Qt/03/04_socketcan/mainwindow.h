/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   04_socketcan
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-15
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCanBusDevice>
#include <QCanBus>
#include <QPushButton>
#include <QTextBrowser>
#include <QLineEdit>
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
    /* CAN设备 */
    QCanBusDevice *canDevice;

    /* 用作接收数据 */
    QTextBrowser *textBrowser;

    /* 用作发送数据 */
    QLineEdit *lineEdit;

    /* 按钮 */
    QPushButton *pushButton[2];

    /* 下拉选择盒子 */
    QComboBox *comboBox[3];

    /* 标签 */
    QLabel *label[4];

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

    /* 插件类型项初始化 */
    void pluginItemInit();

    /* 比特率项初始化 */
    void bitrateItemInit();

private slots:
    /* 发送消息 */
    void sendFrame();

    /* 接收消息 */
    void receivedFrames();

    /* 插件发生改变 */
    void pluginChanged(int);

    /* 处理can错误 */
    void canDeviceErrors(QCanBusDevice::CanBusError) const;

    /* 连接或者断开can */
    void connectDevice();
};
#endif // MAINWINDOW_H
