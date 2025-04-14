/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   01_smarthome
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-26
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 界面布局 */
    layoutInit();

    /* 原子云服务器接口对象 */
    webapi = new Webapi(this);

    /* 接收设备状态改变的心跳包，这里只检查在线的心跳包，
     * 离线的需要设备方主动发送离线信息 */
    connect(webapi, SIGNAL(deviceStateChanged(QString)),
            this, SLOT(deviceState(QString)));

    for (int i = 0; i < 3; i++)
        connect(switchButton[i], SIGNAL(toggled(bool)),
                this, SLOT(onToggled(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::layoutInit()
{
    /* 主界面背景 */
    this->setGeometry(0, 0, 800, 480);
    this->setMinimumWidth(700);
    this->setMinimumHeight(450);
    this->setObjectName("smarthome");

    for (int i = 0; i < 11; i++) {
        widget[i] = new QWidget();
        widget[i]->setObjectName(tr("widget%1")
                                 .arg(QString::number(i)));
    }

    for (int i = 0; i < 2; i++)
        vBoxLayout[i] = new QVBoxLayout();

    for (int i = 0; i < 6; i++)
        hBoxLayout[i] = new QHBoxLayout();

    for (int i = 0; i < 4; i++) {
        label[i] = new QLabel();
        label[i]->setObjectName(tr("label%1")
                                .arg(QString::number(i)));
    }

    QStringList list;
    list<<"在家"<<"睡觉"<<"出远门";
    for (int i = 0; i < 3; i++) {
        pushButton[i] = new QPushButton();
        pushButton[i]->setFixedSize(100, 30);
        pushButton[i]->setText(list[i]);
        pushButton[i]->setCheckable(true);
        pushButton[i]->setFocusPolicy(Qt::NoFocus);
    }

    pushButton[0]->setChecked(true);

    widget[1]->setMaximumHeight(50);
    widget[2]->setMaximumHeight(25);
    widget[3]->setMaximumHeight(120);
    widget[4]->setMaximumHeight(30);
    widget[5]->setMaximumHeight(150);

    /*  主布局 */
    vBoxLayout[0]->addWidget(widget[1]);
    vBoxLayout[0]->addWidget(widget[2]);
    vBoxLayout[0]->addWidget(widget[3]);
    vBoxLayout[0]->addWidget(widget[4]);
    vBoxLayout[0]->addWidget(widget[5]);

    vBoxLayout[0]->setContentsMargins(80, 52, 80, 50);

    /* 主界面采用垂直布局 */
    widget[0]->setLayout(vBoxLayout[0]);
    setCentralWidget(widget[0]);

    cloudPushButton = new QPushButton(this);
    cloudPushButton->move(15, 10);
    cloudPushButton->setMinimumHeight(42);
    cloudPushButton->setMinimumWidth(150);
    cloudPushButton->setFocusPolicy(Qt::NoFocus);
    cloudPushButton->setObjectName("cloudPushButton");
    cloudPushButton->setText("原子云物联");
    QIcon icon(":/icons/cloud.png");
    cloudPushButton->setIcon(icon);
    cloudPushButton->setIconSize(QSize(42, 42));

    hBoxLayout[0]->addWidget(label[0]);
    hBoxLayout[0]->setContentsMargins(0, 0, 0, 0);
    label[0]->setText("虫虫的家");
    widget[1]->setLayout(hBoxLayout[0]);

    hBoxLayout[1]->addWidget(label[1]);
    label[1]->setText("欢迎回家，虫虫，已经为您设置在家模式");
    hBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    widget[2]->setLayout(hBoxLayout[1]);

    hBoxLayout[2]->addWidget(label[2]);
    hBoxLayout[2]->setContentsMargins(0, 0, 0, 0);
    label[2]->setText("常用控制面板");
    widget[4]->setLayout(hBoxLayout[2]);

    vBoxLayout[1]->addWidget(widget[6]);
    vBoxLayout[1]->addWidget(widget[7]);
    vBoxLayout[1]->setContentsMargins(0, 0, 0, 0);
    widget[3]->setLayout(vBoxLayout[1]);

    hBoxLayout[3]->addWidget(label[3]);
    hBoxLayout[3]->setContentsMargins(30, 0, 0, 0);
    label[3]->setText("所处场景");
    widget[6]->setLayout(hBoxLayout[3]);

    hBoxLayout[4]->addWidget(pushButton[0]);
    hBoxLayout[4]->addWidget(pushButton[1]);
    hBoxLayout[4]->addWidget(pushButton[2]);
    hBoxLayout[4]->setAlignment(Qt::AlignLeft);
    hBoxLayout[4]->setSpacing(30);
    hBoxLayout[4]->setContentsMargins(30, 0, 0, 0);
    widget[7]->setLayout(hBoxLayout[4]);

    /* 控制列表 */
    listWidget = new QListWidget();
    listWidget->setFocusPolicy(Qt::NoFocus);
    listWidget->setMaximumHeight(150);
    listWidget->setVerticalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    listWidget->setHorizontalScrollBarPolicy(
                Qt::ScrollBarAlwaysOff);
    /* 横向排布 */
    listWidget->setFlow(QListView::LeftToRight);
    hBoxLayout[5]->addWidget(listWidget);
    hBoxLayout[5]->setContentsMargins(0, 0, 0, 0);
    widget[5]->setLayout(hBoxLayout[5]);

    QStringList iconList;
    iconList<<":/icons/light.png"
           <<":/icons/tablelamp.png"
          <<":/icons/fan.png";
    QStringList localtionList;
    localtionList<<"客厅"<<"次卧"<<"主卧";
    QStringList nameList;
    nameList<<"客厅灯|离线"<<"台灯|离线"<<"空调|离线";

    /* 布局在前面章节详细说过，不再一一注释 */
    for (int i = 0; i < 3; i++) {
        gridLayout[i] =  new QGridLayout();
        gridLayout[i]->setContentsMargins(0, 0, 0, 0);
    }

    for (int i = 0; i < 3; i++) {
        switchButton[i] =  new SwitchButton();
        switchButton[i]->setMaximumSize(50, 30);
    }

    for (int i = 0; i < 9; i++) {
        itemLabel[i] = new QLabel();
        itemLabel[i]->setMaximumSize(75, 75);
        itemLabel[i]->setScaledContents(true);
        itemLabel[i]->setAlignment(Qt::AlignCenter);
        itemLabel[i]->setObjectName(tr("itemLable%1")
                                    .arg(QString::number(i)));
    }

    itemLabel[0]->setPixmap(QPixmap(iconList[0]));
    itemLabel[1]->setPixmap(QPixmap(iconList[1]));
    itemLabel[2]->setPixmap(QPixmap(iconList[2]));

    itemLabel[3]->setText(localtionList[0]);
    itemLabel[4]->setText(localtionList[1]);
    itemLabel[5]->setText(localtionList[2]);

    itemLabel[6]->setText(nameList[0]);
    itemLabel[7]->setText(nameList[1]);
    itemLabel[8]->setText(nameList[2]);

    gridLayout[0]->addWidget(itemLabel[0], 0, 0);
    gridLayout[0]->addWidget(itemLabel[3], 0, 1);
    gridLayout[0]->addWidget(itemLabel[6], 1, 0);
    gridLayout[0]->addWidget(switchButton[0], 1, 1);

    gridLayout[1]->addWidget(itemLabel[1], 0, 0);
    gridLayout[1]->addWidget(itemLabel[4], 0, 1);
    gridLayout[1]->addWidget(itemLabel[7], 1, 0);
    gridLayout[1]->addWidget(switchButton[1], 1, 1);

    gridLayout[2]->addWidget(itemLabel[2], 0, 0);
    gridLayout[2]->addWidget(itemLabel[5], 0, 1);
    gridLayout[2]->addWidget(itemLabel[8], 1, 0);
    gridLayout[2]->addWidget(switchButton[2], 1, 1);

    QListWidgetItem *listWidgetItem[3];

    for (int i = 0; i < 3; i++) {
        listWidgetItem[i] = new QListWidgetItem();
        listWidget->addItem(listWidgetItem[i]);
    }

    widget[8]->setLayout(gridLayout[0]);
    widget[9]->setLayout(gridLayout[1]);
    widget[10]->setLayout(gridLayout[2]);
    listWidget->setItemWidget(listWidget->item(0), widget[8]);
    listWidget->setItemWidget(listWidget->item(1), widget[9]);
    listWidget->setItemWidget(listWidget->item(2), widget[10]);
}

void MainWindow::onToggled(bool flag) {
    SwitchButton *bt = (SwitchButton *)sender();
    for (int i = 0; i < 3; i++) {
        if (bt == switchButton[i]) {
            QStringList list = itemLabel[i + 6]
                    ->text().split("|");
            QString message;
            if (flag)
                message = "开";
            else
                message = "关";

            /* 给原子云服务器发送指令，发送的信息格式为如："客厅灯"，"开" */
            webapi->whichDeviceNameSendCmd(list[0], message);
            break;
        }
    }
}

void MainWindow::deviceState(QString state)
{
    if (state.contains("客厅灯")) {
        itemLabel[6]->setText(state);
        itemLabel[6]->setStyleSheet("color:#f39800");
    } else if (state.contains("台灯")) {
        itemLabel[7]->setText(state);
        itemLabel[7]->setStyleSheet("color:#f39800");
    } else if (state.contains("空调")) {
        itemLabel[8]->setText(state);
        itemLabel[8]->setStyleSheet("color:#f39800");
    }
}
