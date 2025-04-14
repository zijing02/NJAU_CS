/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   03_serialport
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-12
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
#include <QGuiApplication>
#include <QScreen>
#include <QRect>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 布局初始化 */
    layoutInit();

    /* 扫描系统的串口 */
    scanSerialPort();

    /* 波特率项初始化 */
    baudRateItemInit();

    /* 数据位项初始化 */
    dataBitsItemInit();

    /* 检验位项初始化 */
    parityItemInit();

    /* 停止位项初始化 */
    stopBitsItemInit();
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
    /* 初始化 */
    serialPort = new QSerialPort(this);
    textBrowser = new QTextBrowser();
    textEdit = new QTextEdit();
    vboxLayout = new QVBoxLayout();
    funcWidget = new QWidget();
    mainWidget = new QWidget();
    gridLayout = new QGridLayout();

    /* QList链表，字符串类型 */
    QList <QString> list1;
    list1<<"串口号:"<<"波特率:"<<"数据位:"<<"检验位:"<<"停止位:";

    for (int i = 0; i < 5; i++) {
        label[i] = new QLabel(list1[i]);
        /* 设置最小宽度与高度 */
        label[i]->setMinimumSize(80, 30);
        /* 自动调整label的大小 */
        label[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        /* 将label[i]添加至网格的坐标(0, i) */
        gridLayout->addWidget(label[i], 0, i);
    }

    for (int i = 0; i < 5; i++) {
        comboBox[i] = new QComboBox();
        comboBox[i]->setMinimumSize(80, 30);
        /* 自动调整label的大小 */
        comboBox[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        /* 将comboBox[i]添加至网格的坐标(1, i) */
        gridLayout->addWidget(comboBox[i], 1, i);
    }

    /* QList链表，字符串类型 */
    QList <QString> list2;
    list2<<"发送"<<"打开串口";

    for (int i = 0; i < 2; i++) {
        pushButton[i] = new QPushButton(list2[i]);
        pushButton[i]->setMinimumSize(80, 30);
        /* 自动调整label的大小 */
        pushButton[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        /* 将pushButton[0]添加至网格的坐标(i, 5) */
        gridLayout->addWidget(pushButton[i], i, 5);
    }
    pushButton[0]->setEnabled(false);

    /* 布局 */
    vboxLayout->addWidget(textBrowser);
    vboxLayout->addWidget(textEdit);
    funcWidget->setLayout(gridLayout);
    vboxLayout->addWidget(funcWidget);
    mainWidget->setLayout(vboxLayout);
    this->setCentralWidget(mainWidget);

    /* 占位文本 */
    textBrowser->setPlaceholderText("接收到的消息");
    textEdit->setText("www.openedv.com");

    /* 信号槽连接 */
    connect(pushButton[0], SIGNAL(clicked()),
            this, SLOT(sendPushButtonClicked()));
    connect(pushButton[1], SIGNAL(clicked()),
            this, SLOT(openSerialPortPushButtonClicked()));

    connect(serialPort, SIGNAL(readyRead()),
            this, SLOT(serialPortReadyRead()));
}

void MainWindow::scanSerialPort()
{
    /* 查找可用串口 */
    foreach (const QSerialPortInfo &info,
            QSerialPortInfo::availablePorts()) {
        comboBox[0]->addItem(info.portName());
    }
}

void MainWindow::baudRateItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1200"<<"2400"<<"4800"<<"9600"
       <<"19200"<<"38400"<<"57600"
      <<"115200"<<"230400"<<"460800"
     <<"921600";
    for (int i = 0; i < 11; i++) {
        comboBox[1]->addItem(list[i]);
    }
    comboBox[1]->setCurrentIndex(7);
}

void MainWindow::dataBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"5"<<"6"<<"7"<<"8";
    for (int i = 0; i < 4; i++) {
        comboBox[2]->addItem(list[i]);
    }
    comboBox[2]->setCurrentIndex(3);
}

void MainWindow::parityItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    for (int i = 0; i < 5; i++) {
        comboBox[3]->addItem(list[i]);
    }
    comboBox[3]->setCurrentIndex(0);
}

void MainWindow::stopBitsItemInit()
{
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"1"<<"2";
    for (int i = 0; i < 2; i++) {
        comboBox[4]->addItem(list[i]);
    }
    comboBox[4]->setCurrentIndex(0);
}

void MainWindow::sendPushButtonClicked()
{
    /* 获取textEdit数据,转换成utf8格式的字节流 */
    QByteArray data = textEdit->toPlainText().toUtf8();
    serialPort->write(data);
}

void MainWindow::openSerialPortPushButtonClicked()
{
    if (pushButton[1]->text() == "打开串口") {
        /* 设置串口名 */
        serialPort->setPortName(comboBox[0]->currentText());
        /* 设置波特率 */
        serialPort->setBaudRate(comboBox[1]->currentText().toInt());
        /* 设置数据位数 */
        switch (comboBox[2]->currentText().toInt()) {
        case 5:
            serialPort->setDataBits(QSerialPort::Data5);
            break;
        case 6:
            serialPort->setDataBits(QSerialPort::Data6);
            break;
        case 7:
            serialPort->setDataBits(QSerialPort::Data7);
            break;
        case 8:
            serialPort->setDataBits(QSerialPort::Data8);
            break;
        default: break;
        }
        /* 设置奇偶校验 */
        switch (comboBox[3]->currentIndex()) {
        case 0:
            serialPort->setParity(QSerialPort::NoParity);
            break;
        case 1:
            serialPort->setParity(QSerialPort::EvenParity);
            break;
        case 2:
            serialPort->setParity(QSerialPort::OddParity);
            break;
        case 3:
            serialPort->setParity(QSerialPort::SpaceParity);
            break;
        case 4:
            serialPort->setParity(QSerialPort::MarkParity);
            break;
        default: break;
        }
        /* 设置停止位 */
        switch (comboBox[4]->currentText().toInt()) {
        case 1:
            serialPort->setStopBits(QSerialPort::OneStop);
            break;
        case 2:
            serialPort->setStopBits(QSerialPort::TwoStop);
            break;
        default: break;
        }
        /* 设置流控制 */
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
        if (!serialPort->open(QIODevice::ReadWrite))
            QMessageBox::about(NULL, "错误",
                               "串口无法打开！可能串口已经被占用！");
        else {
            for (int i = 0; i < 5; i++)
                comboBox[i]->setEnabled(false);
            pushButton[1]->setText("关闭串口");
            pushButton[0]->setEnabled(true);
        }
    } else {
        serialPort->close();
        for (int i = 0; i < 5; i++)
            comboBox[i]->setEnabled(true);
        pushButton[1]->setText("打开串口");
        pushButton[0]->setEnabled(false);
    }
}

void MainWindow::serialPortReadyRead()
{
    /* 接收缓冲区中读取数据 */
    QByteArray buf = serialPort->readAll();
    textBrowser->insertPlainText(QString(buf));
}

MainWindow::~MainWindow()
{
}

