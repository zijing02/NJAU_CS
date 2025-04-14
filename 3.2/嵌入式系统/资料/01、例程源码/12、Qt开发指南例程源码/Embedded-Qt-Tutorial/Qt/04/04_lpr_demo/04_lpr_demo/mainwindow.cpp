/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 2021-2030. All rights reserved.
* @projectName   04_lpr_demo
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         dengzhimao@alientek.com
* @link          www.openedv.com
* @date          2021-11-17
*******************************************************************/
#include "mainwindow.h"

#include <QDebug>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 车牌识别类初始化 */
    ocr = new Ocr(this);

    /* 设置界面位置大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 用于显示车牌图片 */
    QLabel *label = new QLabel(this);

    /* 用于显示车牌识别结果 */
    resultLabel = new QLabel(this);

    /* 居中 */
    label->setAlignment(Qt::AlignCenter);

    /* 按钮，用于点击开始识别车牌 */
    pushButton = new QPushButton(this);

    pushButton->setText("开始识别");

    resultLabel->move(pushButton->width(), 0);
    resultLabel->resize(300, 30);

    QPixmap pixmap;

    /* 加载图片，显示于Label上 */
    if (pixmap.load("image/carlpr.jpg")) {
        label->setPixmap(pixmap);
        qDebug() << "识别的图片路径为：" + QCoreApplication::applicationDirPath() + "/images/carlpr.jpg" << endl;
    } else {
        qDebug() << "未找要识别的车牌图片！请检查可执行程序下是否有image/carlpr.jpg" << endl;
        qDebug() << QCoreApplication::applicationDirPath() + "/images/carlpr.jpg" + "不存在！" << endl;
    }

    /* 居中显示 */
    this->setCentralWidget(label);

    /* 信号槽连接 */
    connect(pushButton, SIGNAL(clicked(bool)), this, SLOT(pushButtonClicked()));
    connect(ocr, SIGNAL(ocrReadyData(QString)), this, SLOT(disPlaylprResult(QString)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::pushButtonClicked()
{
    /* 识别的图片为可执行程序路径下的image文件夹下的carlpr.jpg */
    ocr->readyToDetection("image/carlpr.jpg");
}

void MainWindow::disPlaylprResult(QString result)
{
    if (result.isEmpty())
        resultLabel->setText("    识别结果为：未识别到车牌！");
    else
        resultLabel->setText("    识别结果为：" + result);
}

