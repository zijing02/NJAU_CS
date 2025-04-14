/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   08_tcpserver
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-04-13
*******************************************************************/
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化tcp服务器与tcp套接字 */
    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    /* 开始监听按钮 */
    pushButton[0] = new QPushButton();
    /* 停止监听按钮 */
    pushButton[1] = new QPushButton();
    /* 清空聊天文本按钮 */
    pushButton[2] = new QPushButton();
    /* 发送消息按钮 */
    pushButton[3] = new QPushButton();

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

    lineEdit = new QLineEdit();
    comboBox = new QComboBox();
    spinBox = new QSpinBox();
    textBrowser = new QTextBrowser();

    label[0]->setText("监听IP地址：");
    label[1]->setText("监听端口：");

    /* 设置标签根据文本文字大小自适应大小  */
    label[0]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);
    label[1]->setSizePolicy(QSizePolicy::Fixed,
                            QSizePolicy::Fixed);

    /* 设置端口号的范围，注意不要与主机的已使用的端口号冲突 */
    spinBox->setRange(10000, 99999);

    pushButton[0]->setText("开始监听");
    pushButton[1]->setText("停止监听");
    pushButton[2]->setText("清空文本");
    pushButton[3]->setText("发送消息");

    /* 设置停止监听状态不可用 */
    pushButton[1]->setEnabled(false);

    /* 设置输入框默认的文本 */
    lineEdit->setText("www.openedv.com正点原子论坛");

    /* 水平布局一添加内容 */
    hBoxLayout[0]->addWidget(pushButton[0]);
    hBoxLayout[0]->addWidget(pushButton[1]);
    hBoxLayout[0]->addWidget(pushButton[2]);

    /* 设置水平容器一的布局为水平布局一 */
    hWidget[0]->setLayout(hBoxLayout[0]);

    /* 水平布局二添加内容 */
    hBoxLayout[1]->addWidget(label[0]);
    hBoxLayout[1]->addWidget(comboBox);
    hBoxLayout[1]->addWidget(label[1]);
    hBoxLayout[1]->addWidget(spinBox);

    /* 设置水平容器二的布局为水平布局二 */
    hWidget[1]->setLayout(hBoxLayout[1]);

    /* 水平布局三添加内容 */
    hBoxLayout[2]->addWidget(lineEdit);
    hBoxLayout[2]->addWidget(pushButton[3]);

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
            this, SLOT(startListen()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(stopListen()));
    connect(pushButton[2], SIGNAL(clicked()),
            this, SLOT(clearTextBrowser()));
    connect(pushButton[3], SIGNAL(clicked()),
            this, SLOT(sendMessages()));
    connect(tcpServer, SIGNAL(newConnection()),
            this, SLOT(clientConnected()));
}

MainWindow::~MainWindow()
{
}

/* 新的客户端连接 */
void MainWindow::clientConnected()
{
    /* 获取客户端的套接字 */
    tcpSocket = tcpServer->nextPendingConnection();
    /* 客户端的ip信息 */
    QString ip = tcpSocket->peerAddress().toString();
    /* 客户端的端口信息 */
    quint16 port = tcpSocket->peerPort();
    /* 在文本浏览框里显示出客户端的连接信息 */
    textBrowser->append("客户端已连接");
    textBrowser->append("客户端ip地址:"
                        + ip);
    textBrowser->append("客户端端口:"
                        + QString::number(port));

    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(receiveMessages()));
    connect(tcpSocket,
            SIGNAL(stateChanged(QAbstractSocket::SocketState)),
            this,
            SLOT(socketStateChange(QAbstractSocket::SocketState)));
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

/* 开始监听 */
void MainWindow::startListen()
{
    /* 需要判断当前主机是否有IP项 */
    if (comboBox->currentIndex() != -1) {
        qDebug()<<"start listen"<<endl;
        tcpServer->listen(IPlist[comboBox->currentIndex()],
                spinBox->value());

        /* 设置按钮与下拉列表框的状态 */
        pushButton[0]->setEnabled(false);
        pushButton[1]->setEnabled(true);
        comboBox->setEnabled(false);
        spinBox->setEnabled(false);

        /* 在文本浏览框里显示出服务端 */
        textBrowser->append("服务器IP地址："
                            + comboBox->currentText());
        textBrowser->append("正在监听端口："
                            + spinBox->text());
    }
}

/* 停止监听 */
void MainWindow::stopListen()
{
    qDebug()<<"stop listen"<<endl;
    /* 停止监听 */
    tcpServer->close();

    /* 如果是连接上了也应该断开，如果不断开客户端还能继续发送信息，
     * 因为socket未断开，还在监听上一次端口 */
    if (tcpSocket->state() == tcpSocket->ConnectedState)
        tcpSocket->disconnectFromHost();

    /* 设置按钮与下拉列表框的状态  */
    pushButton[1]->setEnabled(false);
    pushButton[0]->setEnabled(true);
    comboBox->setEnabled(true);
    spinBox->setEnabled(true);

    /* 将停止监听的信息添加到文本浏览框中 */
    textBrowser->append("已停止监听端口："
                        + spinBox->text());
}

/* 清除文本浏览框里的内容 */
void MainWindow::clearTextBrowser()
{
    /* 清除文本浏览器的内容 */
    textBrowser->clear();
}

/* 服务端接收消息 */
void MainWindow::receiveMessages()
{
    /* 读取接收到的消息 */
    QString messages = "客户端：" + tcpSocket->readAll();
    textBrowser->append(messages);
}

/* 服务端发送消息 */
void MainWindow::sendMessages()
{
    if(NULL == tcpSocket)
        return;

    /* 如果已经连接 */
    if(tcpSocket->state() == tcpSocket->ConnectedState) {
        /* 发送消息 */
        tcpSocket->write(lineEdit->text().toUtf8().data());

        /* 在服务端插入发送的消息 */
        textBrowser->append("服务端：" + lineEdit->text());
    }
}

/* 服务端状态改变 */
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
