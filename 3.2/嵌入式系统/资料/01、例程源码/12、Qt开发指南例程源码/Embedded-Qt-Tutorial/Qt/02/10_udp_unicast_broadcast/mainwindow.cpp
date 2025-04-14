/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   10_udp_unicast_broadcast
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-14
*******************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* udp套接字 */
    udpSocket = new QUdpSocket(this);

    /* 绑定端口按钮 */
    pushButton[0] = new QPushButton();
    /* 解绑端口按钮 */
    pushButton[1] = new QPushButton();
    /* 清空聊天文本按钮 */
    pushButton[2] = new QPushButton();
    /* 发送消息按钮 */
    pushButton[3] = new QPushButton();
    /* 广播消息按钮 */
    pushButton[4] = new QPushButton();

    /* 水平布局一 */
    hBoxLayout[0] = new QHBoxLayout();
    /* 水平布局二 */
    hBoxLayout[1] = new QHBoxLayout();
    /* 水平布局三 */
    hBoxLayout[2] = new QHBoxLayout();
    /* 水平布局四 */
    hBoxLayout[3] = new QHBoxLayout();

    /* 水平容器一 */
    hWidget[0] =  new QWidget();
    /* 水平容器二 */
    hWidget[1] =  new QWidget();
    /* 水平容器三 */
    hWidget[2] =  new QWidget();


    vWidget = new QWidget();
    vBoxLayout = new QVBoxLayout();

    /* 标签实例化 */
    label[0] = new QLabel();
    label[1] = new QLabel();
    label[2] = new QLabel();

    lineEdit = new QLineEdit();
    comboBox = new QComboBox();
    spinBox[0] = new QSpinBox();
    spinBox[1] = new QSpinBox();
    textBrowser = new QTextBrowser();

    label[0]->setText("目标IP地址：");
    label[1]->setText("绑定端口：");
    label[2]->setText("目标端口：");

    /* 设置标签根据文本文字大小自适应大小  */
    label[0]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    label[1]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    label[2]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);

    /* 设置端口号的范围，注意不要与主机的已使用的端口号冲突 */
    spinBox[0]->setRange(10000, 99999);
    spinBox[1]->setRange(10000, 99999);

    pushButton[0]->setText("绑定端口");
    pushButton[1]->setText("解除绑定");
    pushButton[2]->setText("清空文本");
    pushButton[3]->setText("发送消息");
    pushButton[4]->setText("广播消息");

    /* 设置停止监听状态不可用 */
    pushButton[1]->setEnabled(false);

    /* 设置输入框默认的文本 */
    lineEdit->setText("你好！");

    /* 水平布局一添加内容 */
    hBoxLayout[0]->addWidget(pushButton[0]);
    hBoxLayout[0]->addWidget(pushButton[1]);
    hBoxLayout[0]->addWidget(pushButton[2]);

    /* 设置水平容器的布局为水平布局一 */
    hWidget[0]->setLayout(hBoxLayout[0]);

    hBoxLayout[1]->addWidget(label[0]);
    hBoxLayout[1]->addWidget(comboBox);
    hBoxLayout[1]->addWidget(label[1]);
    hBoxLayout[1]->addWidget(spinBox[0]);
    hBoxLayout[1]->addWidget(label[2]);
    hBoxLayout[1]->addWidget(spinBox[1]);

    /* 设置水平容器的布局为水平布局二 */
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* 水平布局三添加内容 */
    hBoxLayout[2]->addWidget(lineEdit);
    hBoxLayout[2]->addWidget(pushButton[3]);
    hBoxLayout[2]->addWidget(pushButton[4]);

    /* 设置水平容器三的布局为水平布局一 */
    hWidget[2]->setLayout(hBoxLayout[2]);

    /* 垂直布局添加内容 */
    vBoxLayout->addWidget(textBrowser);
    vBoxLayout->addWidget(hWidget[1]);
    vBoxLayout->addWidget(hWidget[0]);
    vBoxLayout->addWidget(hWidget[2]);

    /* 设置垂直容器的布局为垂直布局 */
    vWidget->setLayout(vBoxLayout);

    /* 居中显示 */
    setCentralWidget(vWidget);

    /* 获取本地ip */
    getLocalHostIP();

    /* 信号槽连接 */
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(bindPort()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(unbindPort()));
    connect(pushButton[2], SIGNAL(clicked()),
            this, SLOT(clearTextBrowser()));
    connect(pushButton[3], SIGNAL(clicked()),
            this, SLOT(sendMessages()));
    connect(pushButton[4], SIGNAL(clicked()),
            this, SLOT(sendBroadcastMessages()));
    connect(udpSocket, SIGNAL(readyRead()),
            this, SLOT(receiveMessages()));
    connect(udpSocket,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(socketStateChange(QAbstractSocket::SocketState)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::bindPort()
{
    quint16 port = spinBox[0]->value();

    /* 绑定端口需要在socket的状态为UnconnectedState */
    if (udpSocket->state() != QAbstractSocket::UnconnectedState)
        udpSocket->close();

    if (udpSocket->bind(port)) {
        textBrowser->append("已经成功绑定端口："
                            + QString::number(port));

        /* 设置界面中的元素的可用状态 */
        pushButton[0]->setEnabled(false);
        pushButton[1]->setEnabled(true);
        spinBox[1]->setEnabled(false);
    }
}

void MainWindow::unbindPort()
{
    /* 解绑，不再监听 */
    udpSocket->abort();

    /* 设置界面中的元素的可用状态 */
    pushButton[0]->setEnabled(true);
    pushButton[1]->setEnabled(false);
    spinBox[1]->setEnabled(true);
}

/* 获取本地IP */
void MainWindow::getLocalHostIP()
{
    // /* 获取主机的名称 */
    // QString hostName = QHostInfo::localHostName();

    // /* 主机的信息 */
    // QHostInfo hostInfo = QHostInfo::fromName(hostName);

    // /* ip列表,addresses返回ip地址列表，注意主机应能从路由器获取到
    // * IP，否则可能返回空的列表（ubuntu用此方法只能获取到环回IP） */
    // IPlist = hostInfo.addresses();
    // qDebug()<<IPlist<<endl;

    // /* 遍历IPlist */
    // foreach (QHostAddress ip, IPlist) {
    //      if (ip.protocol() == QAbstractSocket::IPv4Protocol)
    //          comboBox->addItem(ip.toString());
    //    }

    /* 获取所有的网络接口，
     * QNetworkInterface类提供主机的IP地址和网络接口的列表 */
    QList<QNetworkInterface> list
            = QNetworkInterface::allInterfaces();

    /* 遍历list */
    foreach (QNetworkInterface interface, list) {

        /* QNetworkAddressEntry类存储IP地址子网掩码和广播地址 */
        QList<QNetworkAddressEntry> entryList
                = interface.addressEntries();

        /* 遍历entryList */
        foreach (QNetworkAddressEntry entry, entryList) {
            /* 过滤IPv6地址，只留下IPv4 */
            if (entry.ip().protocol() ==
                    QAbstractSocket::IPv4Protocol) {
                comboBox->addItem(entry.ip().toString());
                /* 添加到IP列表中 */
                IPlist<<entry.ip();
            }
        }
    }
}

/* 清除文本浏览框里的内容 */
void MainWindow::clearTextBrowser()
{
    /* 清除文本浏览器的内容 */
    textBrowser->clear();
}

/* 客户端接收消息 */
void MainWindow::receiveMessages()
{
    /* 局部变量，用于获取发送者的IP和端口 */
    QHostAddress peerAddr;
    quint16 peerPort;

    /* 如果有数据已经准备好 */
    while (udpSocket->hasPendingDatagrams()) {
        /* udpSocket发送的数据报是QByteArray类型的字节数组 */
        QByteArray datagram;

        /* 重新定义数组的大小 */
        datagram.resize(udpSocket->pendingDatagramSize());

        /* 读取数据，并获取发送方的IP地址和端口 */
        udpSocket->readDatagram(datagram.data(),
                                datagram.size(),
                                &peerAddr,
                                &peerPort);
        /* 转为字符串 */
        QString str = datagram.data();

        /* 显示信息到文本浏览框窗口 */
        textBrowser->append("接收来自"
                            + peerAddr.toString()
                            + ":"
                            + QString::number(peerPort)
                            + str);
    }
}

/* 客户端发送消息 */
void MainWindow::sendMessages()
{
    /* 文本浏览框显示发送的信息 */
    textBrowser->append("发送：" + lineEdit->text());

    /* 要发送的信息，转为QByteArray类型字节数组，数据一般少于512个字节 */
    QByteArray data = lineEdit->text().toUtf8();

    /* 要发送的目标Ip地址 */
    QHostAddress peerAddr = IPlist[comboBox->currentIndex()];

    /* 要发送的目标端口号 */
    quint16 peerPort = spinBox[1]->value();

    /* 发送消息 */
    udpSocket->writeDatagram(data, peerAddr, peerPort);
}

void MainWindow::sendBroadcastMessages()
{
    /* 文本浏览框显示发送的信息 */
    textBrowser->append("发送：" + lineEdit->text());

    /* 要发送的信息，转为QByteArray类型字节数组，数据一般少于512个字节 */
    QByteArray data = lineEdit->text().toUtf8();

    /* 广播地址，一般为255.255.255.255，
     * 同一网段内监听目标端口的程序都会接收到消息 */
    QHostAddress peerAddr = QHostAddress::Broadcast;

    /* 要发送的目标端口号 */
    quint16 peerPort = spinBox[1]->text().toInt();

    /* 发送消息 */
    udpSocket->writeDatagram(data, peerAddr, peerPort);
}
/* socket状态改变 */
void MainWindow::socketStateChange(QAbstractSocket::SocketState state)
{
    switch (state) {
    case QAbstractSocket::UnconnectedState:
        textBrowser->append("scoket状态：UnconnectedState");
        break;
    case QAbstractSocket::ConnectedState:
        textBrowser->append("scoket状态：ConnectedState");
        break;
    case QAbstractSocket::ConnectingState:
        textBrowser->append("scoket状态：ConnectingState");
        break;
    case QAbstractSocket::HostLookupState:
        textBrowser->append("scoket状态：HostLookupState");
        break;
    case QAbstractSocket::ClosingState:
        textBrowser->append("scoket状态：ClosingState");
        break;
    case QAbstractSocket::ListeningState:
        textBrowser->append("scoket状态：ListeningState");
        break;
    case QAbstractSocket::BoundState:
        textBrowser->append("scoket状态：BoundState");
        break;
    default:
        break;
    }
}
