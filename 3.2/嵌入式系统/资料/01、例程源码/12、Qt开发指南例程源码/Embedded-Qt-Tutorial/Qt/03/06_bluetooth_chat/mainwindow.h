/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-19
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothsocket.h>
#include <qbluetoothhostinfo.h>
#include <QDebug>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QTextBrowser>
#include <QLineEdit>

class ChatServer;
class ChatClient;
class RemoteSelector;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    /* 暴露的接口，主动连接设备*/
    Q_INVOKABLE void connectToDevice();

signals:
    /* 发送消息信号 */
    void sendMessage(const QString &message);

    /* 连接断开信号 */
    void disconnect();

    /* 发现完成信号 */
    void discoveryFinished();

    /* 找到新服务信号 */
    void newServicesFound(const QStringList &list);

public slots:
    /* 停止搜索 */
    void searchForDevices();

    /* 开始搜索 */
    void stopSearch();

    /* 找到新服务 */
    void newServiceFound(QListWidgetItem*);

    /* 已连接 */
    void connected(const QString &name);

    /* 显示消息 */
    void showMessage(const QString &sender, const QString &message);

    /* 发送消息 */
    void sendMessage();

    /* 作为客户端断开连接 */
    void clientDisconnected();

    /* 主动断开连接 */
    void toDisconnected();

    /* 作为服务端时，客户端断开连接 */
    void disconnected(const QString &name);

private:
    /* 选择本地蓝牙，些方法未没使用 */
    int adapterFromUserSelection() const;

    /* 本地蓝牙的Index */
    int currentAdapterIndex;

    /* 蓝牙本地适配器初始化 */
    void localAdapterInit();

    /* 布局初始化 */
    void layoutInit();

    /* 服务端*/
    ChatServer *server;

    /* 多个客户端 */
    QList<ChatClient *> clients;

    /* 远程选择器，使用本地蓝牙去搜索蓝牙，可过滤蓝牙等 */
    RemoteSelector *remoteSelector;

    /* 本地蓝牙 */
    QList<QBluetoothHostInfo> localAdapters;

    /* 本地蓝牙名称 */
    QString localName;

    /* tabWidget视图，用于切换页面 */
    QTabWidget *tabWidget;

    /* 3个按钮，扫描按钮，连接按钮，发送按钮 */
    QPushButton *pushButton[5];

    /* 2个垂直布局，一个用于页面一，另一个用于页面二 */
    QVBoxLayout *vBoxLayout[2];

    /* 2个水平布局，一个用于页面一，另一个用于页面二 */
    QHBoxLayout *hBoxLayout[2];

    /* 页面一和页面二容器 */
    QWidget *pageWidget[2];

    /* 用于布局, pageWidget包含subWidget */
    QWidget *subWidget[2];

    /* 蓝牙列表 */
    QListWidget *listWidget;

    /* 显示对话的内容 */
    QTextBrowser *textBrowser;

    /* 发送消息输入框 */
    QLineEdit *lineEdit;

};
#endif // MAINWINDOW_H
