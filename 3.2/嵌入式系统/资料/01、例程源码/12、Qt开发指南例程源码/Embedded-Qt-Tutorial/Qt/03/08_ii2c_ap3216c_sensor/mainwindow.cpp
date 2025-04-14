/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   08_spi_sensor
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-21
*******************************************************************/
#include "mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800, 480);
    this->setStyleSheet("background:#011753");

    for (int i = 0; i < 6; i++)
        widget[i] = new QWidget();

    for (int i = 0; i < 3; i++)
        arcGraph[i] = new ArcGraph();

    for (int i = 0; i < 5; i++)
        hBoxLayout[i] = new QHBoxLayout();

    headView = new HeadView();

    QFont font;
    font.setPixelSize(18);

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::white);

    QStringList list;
    list<<"环境光强度："<<"接近距离："<<"红外强度：";
    for (int i = 0; i < 3; i++) {
        label[i] = new QLabel();
        glowText[i] = new GlowText();
        glowText[i]->setMinimumWidth(30);
        label[i]->setText(list[i]);
        label[i]->setFont(font);
        label[i]->setPalette(pal);
        label[i]->adjustSize();
    }

    vBoxLayout = new QVBoxLayout();

    /* 垂直布局，将主窗体为上下两部分，方便布局 */
    vBoxLayout->addWidget(headView);
    vBoxLayout->addWidget(widget[1]);
    vBoxLayout->addWidget(widget[2]);
    widget[0]->setLayout(vBoxLayout);

    /* 主布局设置为widget[0] */
    setCentralWidget(widget[0]);

    /* 设置widget[1]的高度，不会随界面的大小而变化 */
    widget[2]->setFixedHeight(150);

    /* 三个蓝色科技感弧形图布局，采用水平布局 */
    hBoxLayout[0]->addWidget(arcGraph[0]);
    hBoxLayout[0]->addWidget(arcGraph[1]);
    hBoxLayout[0]->addWidget(arcGraph[2]);
    widget[1]->setLayout(hBoxLayout[0]);

    /* 数据文字容器水平布局, */
    hBoxLayout[1]->addWidget(widget[3]);
    hBoxLayout[1]->addWidget(widget[4]);
    hBoxLayout[1]->addWidget(widget[5]);
    hBoxLayout[1]->setContentsMargins(0, 40, 0, 0);

    widget[2]->setLayout(hBoxLayout[1]);

    /* als布局 */
    hBoxLayout[2]->addWidget(label[0]);
    hBoxLayout[2]->addWidget(glowText[0]);
    hBoxLayout[2]->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    widget[3]->setLayout(hBoxLayout[2]);

    /* ps布局 */
    hBoxLayout[3]->addWidget(label[1]);
    hBoxLayout[3]->addWidget(glowText[1]);
    hBoxLayout[3]->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    widget[4]->setLayout(hBoxLayout[3]);

    /* ir布局 */
    hBoxLayout[4]->addWidget(label[2]);
    hBoxLayout[4]->addWidget(glowText[2]);
    hBoxLayout[4]->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    widget[5]->setLayout(hBoxLayout[4]);

    ap3216c = new Ap3216c(this);
    /* 只能在开发板上开启获取数据，Ubuntu上是没有ap3216c传感器的 */
#if __arm__
    ap3216c->setCapture(true);
#endif

    connect(ap3216c, SIGNAL(ap3216cDataChanged()),
            this, SLOT(getAp3216cData()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::getAp3216cData()
{
    static QString als = ap3216c->alsData();
    if (als != ap3216c->alsData()) {
        als = ap3216c->alsData();
        arcGraph[0]->setangleLength(als.toUInt() * 360 / 65535);
    }

    static QString ps = ap3216c->psData();
    if (ps != ap3216c->psData()) {
        ps = ap3216c->psData();
        arcGraph[1]->setangleLength(ps.toUInt() * 360 / 1023);
    }

    static QString ir = ap3216c->irData();
    if (ir != ap3216c->irData()) {
        ir = ap3216c->irData();
        arcGraph[2]->setangleLength(ir.toUInt() * 360 / 1023);
    }

    glowText[0]->setTextData(als);
    glowText[1]->setTextData(ps);
    glowText[2]->setTextData(ir);
}
