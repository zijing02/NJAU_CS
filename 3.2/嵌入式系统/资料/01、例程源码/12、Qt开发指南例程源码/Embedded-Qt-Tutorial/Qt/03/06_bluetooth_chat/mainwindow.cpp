/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   06_bluetooth_chat
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-19
*******************************************************************/
#include "mainwindow.h"
#include "remoteselector.h"
#include "chatserver.h"
#include "chatclient.h"
#include <qbluetoothuuid.h>
#include <qbluetoothserver.h>
#include <qbluetoothservicediscoveryagent.h>
#include <qbluetoothdeviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>
#include <QTimer>
#include <QDebug>
#include <QTabBar>
#include <QHeaderView>
#include <QTableView>


static const QLatin1String
serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 本地蓝牙初始化 */
    localAdapterInit();

    /* 界面布局初始化 */
    layoutInit();
}

MainWindow::~MainWindow()
{
    qDeleteAll(clients);
    delete server;
}

/* 初始化本地蓝牙，作为服务端 */
void MainWindow::localAdapterInit()
{
    /* 查找本地蓝牙的个数 */
    localAdapters = QBluetoothLocalDevice::allDevices();
    qDebug() << "localAdapter: " << localAdapters.count();

    QBluetoothLocalDevice localDevice;
    localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    QBluetoothAddress adapter = QBluetoothAddress();
    remoteSelector = new RemoteSelector(adapter, this);
    connect(remoteSelector,
            SIGNAL(newServiceFound(QListWidgetItem*)),
            this, SLOT(newServiceFound(QListWidgetItem*)));

    /* 初始化服务端 */
    server = new ChatServer(this);

    connect(server, SIGNAL(clientConnected(QString)),
            this, SLOT(connected(QString)));

    connect(server, SIGNAL(clientDisconnected(QString)),
            this, SLOT(disconnected(QString)));

    connect(server, SIGNAL(messageReceived(QString, QString)),
            this, SLOT(showMessage(QString, QString)));

    connect(this, SIGNAL(sendMessage(QString)),
            server, SLOT(sendMessage(QString)));

    connect(this, SIGNAL(disconnect()),
            server, SLOT(disconnect()));

    server->startServer();

    /* 获取本地蓝牙的名称 */
    localName = QBluetoothLocalDevice().name();
}

void MainWindow::layoutInit()
{
    /* 获取屏幕的分辨率，Qt官方建议使用这
     * 种方法获取屏幕分辨率，防上多屏设备导致对应不上
     * 注意，这是获取整个桌面系统的分辨率
     */
    QList <QScreen *> list_screen =  QGuiApplication::screens();

    /* 如果是ARM平台，直接设置大小为屏幕的大小 */
#if __arm__
    /* 重设大小 */
    this->resize(list_screen.at(0)->geometry().width(),
                 list_screen.at(0)->geometry().height());
#else
    /* 否则则设置主窗体大小为800x480 */
    this->resize(800, 480);
#endif

    /* 主视图 */
    tabWidget = new QTabWidget(this);

    /* 设置主窗口居中视图为tabWidget */
    setCentralWidget(tabWidget);

    /* 页面一对象实例化 */
    vBoxLayout[0] = new QVBoxLayout();
    hBoxLayout[0] = new QHBoxLayout();
    pageWidget[0] = new QWidget();
    subWidget[0] = new QWidget();
    listWidget = new QListWidget();
    /* 0为扫描按钮，1为连接按钮 */
    pushButton[0] = new QPushButton();
    pushButton[1] = new QPushButton();
    pushButton[2] = new QPushButton();
    pushButton[3] = new QPushButton();
    pushButton[4] = new QPushButton();

    /* 页面二对象实例化 */
    hBoxLayout[1] = new QHBoxLayout();
    vBoxLayout[1] = new QVBoxLayout();
    subWidget[1] = new QWidget();
    textBrowser = new QTextBrowser();
    lineEdit = new QLineEdit();
    pushButton[2] = new QPushButton();
    pageWidget[1] = new QWidget();


    tabWidget->addTab(pageWidget[1], "蓝牙聊天");
    tabWidget->addTab(pageWidget[0], "蓝牙列表");

    /* 页面一 */
    vBoxLayout[0]->addWidget(pushButton[0]);
    vBoxLayout[0]->addWidget(pushButton[1]);
    vBoxLayout[0]->addWidget(pushButton[2]);
    vBoxLayout[0]->addWidget(pushButton[3]);
    subWidget[0]->setLayout(vBoxLayout[0]);
    hBoxLayout[0]->addWidget(listWidget);
    hBoxLayout[0]->addWidget(subWidget[0]);
    pageWidget[0]->setLayout(hBoxLayout[0]);
    pushButton[0]->setMinimumSize(120, 40);
    pushButton[1]->setMinimumSize(120, 40);
    pushButton[2]->setMinimumSize(120, 40);
    pushButton[3]->setMinimumSize(120, 40);
    pushButton[0]->setText("开始扫描");
    pushButton[1]->setText("停止扫描");
    pushButton[2]->setText("连接");
    pushButton[3]->setText("断开");

    /* 页面二 */
    hBoxLayout[1]->addWidget(lineEdit);
    hBoxLayout[1]->addWidget(pushButton[4]);
    subWidget[1]->setLayout(hBoxLayout[1]);
    vBoxLayout[1]->addWidget(textBrowser);
    vBoxLayout[1]->addWidget(subWidget[1]);
    pageWidget[1]->setLayout(vBoxLayout[1]);
    pushButton[4]->setMinimumSize(120, 40);
    pushButton[4]->setText("发送");
    lineEdit->setMinimumHeight(40);
    lineEdit->setText("正点原子论坛网址www.openedv.com");

    /* 设置表头的大小 */
    QString str = tr("QTabBar::tab {height:40; width:%1};")
            .arg(this->width()/2);
    tabWidget->setStyleSheet(str);

    /* 开始搜寻蓝牙 */
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(searchForDevices()));

    /* 停止搜寻蓝牙 */
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(stopSearch()));

    /* 点击连接按钮，本地蓝牙作为客户端去连接外界的服务端 */
    connect(pushButton[2], SIGNAL(clicked()),
            this, SLOT(connectToDevice()));

    /* 点击断开连接按钮，断开连接 */
    connect(pushButton[3], SIGNAL(clicked()),
            this, SLOT(toDisconnected()));

    /* 发送消息 */
    connect(pushButton[4], SIGNAL(clicked()),
            this, SLOT(sendMessage()));
}

/* 作为客户端去连接 */
void MainWindow::connectToDevice()
{
    if (listWidget->currentRow() == -1)
        return;

    QString name = listWidget->currentItem()->text();
    qDebug() << "Connecting to " << name;

    // Trying to get the service
    QBluetoothServiceInfo service;
    QMapIterator<QString,QBluetoothServiceInfo>
            i(remoteSelector->m_discoveredServices);
    bool found = false;
    while (i.hasNext()){
        i.next();

        QString key = i.key();

        /* 判断连接的蓝牙名称是否在发现的设备里 */
        if (key == name) {
            qDebug() << "The device is found";
            service = i.value();
            qDebug() << "value: " << i.value().device().address();
            found = true;
            break;
        }
    }

    /* 如果找到，则连接设备 */
    if (found) {
        qDebug() << "Going to create client";
        ChatClient *client = new ChatClient(this);
        qDebug() << "Connecting...";

        connect(client, SIGNAL(messageReceived(QString,QString)),
                this, SLOT(showMessage(QString,QString)));
        connect(client, SIGNAL(disconnected()),
                this, SLOT(clientDisconnected()));;
        connect(client, SIGNAL(connected(QString)),
                this, SLOT(connected(QString)));
        connect(this, SIGNAL(sendMessage(QString)),
                client, SLOT(sendMessage(QString)));
        connect(this, SIGNAL(disconnect()),
                client, SLOT(disconnect()));

        qDebug() << "Start client";
        client->startClient(service);

        clients.append(client);
    }
}

/* 本地蓝牙选择，默认使用第一个蓝牙 */
int MainWindow::adapterFromUserSelection() const
{
    int result = 0;
    QBluetoothAddress newAdapter = localAdapters.at(0).address();
    return result;
}

/* 开始搜索 */
void MainWindow::searchForDevices()
{
    /* 先清空 */
    listWidget->clear();
    qDebug() << "search for devices!";
    if (remoteSelector) {
        delete remoteSelector;
        remoteSelector = NULL;
    }

    QBluetoothAddress adapter = QBluetoothAddress();
    remoteSelector = new RemoteSelector(adapter, this);

    connect(remoteSelector,
            SIGNAL(newServiceFound(QListWidgetItem*)),
            this, SLOT(newServiceFound(QListWidgetItem*)));

    remoteSelector->m_discoveredServices.clear();
    remoteSelector->startDiscovery(QBluetoothUuid(serviceUuid));
    connect(remoteSelector, SIGNAL(finished()),
            this, SIGNAL(discoveryFinished()));
}

/* 停止搜索 */
void MainWindow::stopSearch()
{
    qDebug() << "Going to stop discovery...";
    if (remoteSelector) {
        remoteSelector->stopDiscovery();
    }
}

/* 找到蓝牙服务 */
void MainWindow::newServiceFound(QListWidgetItem *item)
{
    /* 设置项的大小 */
    item->setSizeHint(QSize(listWidget->width(), 50));

    /* 添加项 */
    listWidget->addItem(item);

    /* 设置当前项 */
    listWidget->setCurrentRow(listWidget->count() - 1);

    qDebug() << "newServiceFound";

    // get all of the found devices
    QStringList list;

    QMapIterator<QString, QBluetoothServiceInfo>
            i(remoteSelector->m_discoveredServices);
    while (i.hasNext()){
        i.next();
        qDebug() << "key: " << i.key();
        qDebug() << "value: " << i.value().device().address();
        list << i.key();
    }

    qDebug() << "list count: "  << list.count();

    emit newServicesFound(list);
}

/* 已经连接 */
void MainWindow::connected(const QString &name)
{
    textBrowser->insertPlainText(tr("%1:已连接\n").arg(name));
    tabWidget->setCurrentIndex(0);
    textBrowser->moveCursor(QTextCursor::End);
}

/* 接收消息 */
void MainWindow::showMessage(const QString &sender,
                             const QString &message)
{
    textBrowser->insertPlainText(QString::fromLatin1("%1: %2\n")
                                 .arg(sender, message));
    tabWidget->setCurrentIndex(0);
    textBrowser->moveCursor(QTextCursor::End);
}

/* 发送消息 */
void MainWindow::sendMessage()
{
    showMessage(localName, lineEdit->text());
    emit sendMessage(lineEdit->text());
}

/* 作为客户端断开连接 */
void MainWindow::clientDisconnected()
{
    ChatClient *client = qobject_cast<ChatClient *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }

    tabWidget->setCurrentIndex(0);
    textBrowser->moveCursor(QTextCursor::End);
}

/* 主动断开连接 */
void MainWindow::toDisconnected()
{
    emit disconnect();
    textBrowser->moveCursor(QTextCursor::End);
    tabWidget->setCurrentIndex(0);
}

/* 作为服务端时，客户端断开连接 */
void MainWindow::disconnected(const QString &name)
{
    textBrowser->insertPlainText(tr("%1:已断开\n").arg(name));
    tabWidget->setCurrentIndex(0);
    textBrowser->moveCursor(QTextCursor::End);
}
