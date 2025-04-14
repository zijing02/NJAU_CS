/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   04_qtchart
* @brief         mainwindow.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-03-28
*******************************************************************/
#include "mainwindow.h"
#include <QDateTime>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置最显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);
    /* 最大储存maxSize - 1个数据 */
    maxSize = 51;
    /* x轴上的最大值 */
    maxX = 5000;
    /* y轴最大值 */
    maxY = 40;

    /* splineSeries曲线实例化（折线用QLineSeries） */
    splineSeries = new QSplineSeries();
    /* 图表实例化 */
    chart = new QChart();
    /* 图表视图实例化 */
    chartView = new QChartView();

    /* 坐标轴 */
    axisY = new QValueAxis();
    axisX = new QValueAxis();
    /* 定时器 */
    timer = new QTimer(this);

    /* legend译图例类型，以绘图的颜色区分，本例设置为隐藏 */
    chart->legend()->hide();
    /* chart设置标题 */
    chart->setTitle("实时动态曲线示例");
    /* 添加一条曲线splineSeries */
    chart->addSeries(splineSeries);

    /* 设置显示格式 */
    axisY->setLabelFormat("%i");
    /* y轴标题 */
    axisY->setTitleText("温度/℃");
    /* y轴标题位置（设置坐标轴的方向） */
    chart->addAxis(axisY, Qt::AlignLeft);
    /* 设置y轴范围 */
    axisY->setRange(0, maxY);
    /* 将splineSeries附加于y轴上 */
    splineSeries->attachAxis(axisY);

    /* 设置显示格式 */
    axisX->setLabelFormat("%i");
    /* x轴标题 */
    axisX->setTitleText("时间/ms");
    /* x轴标题位置（设置坐标轴的方向） */
    chart->addAxis(axisX, Qt::AlignBottom);
    /* 设置x轴范围 */
    axisX->setRange(0, maxX);
    /* 将splineSeries附加于x轴上 */
    splineSeries->attachAxis(axisX);

    /* 将图表的内容设置在图表视图上 */
    chartView->setChart(chart);
    /* 设置抗锯齿 */
    chartView->setRenderHint(QPainter::Antialiasing);

    /* 设置为图表视图为中心部件 */
    setCentralWidget(chartView);

    /* 定时200ms */
    timer->start(200);
    /* 信号槽连接 */
    connect(timer, SIGNAL(timeout()), this, SLOT(timerTimeOut()));

    /* 设置随机种子，随机数初始化 */
    qsrand(time(NULL));
}

MainWindow::~MainWindow()
{
}

void MainWindow::timerTimeOut()
{
    /* 产生随机0~maxY之间的数据 */
    receivedData(qrand() % maxY );
}

void MainWindow::receivedData(int value)
{
    /* 将数据添加到data中 */
    data.append(value);

    /* 当储存数据的个数大于最大值时，把第一个数据删除 */
    while (data.size() > maxSize) {
        /* 移除data中第一个数据 */
        data.removeFirst();
    }

    /* 先清空 */
    splineSeries->clear();

    /* 计算x轴上的点与点之间显示的间距 */
    int xSpace = maxX / (maxSize - 1);

    /* 添加点，xSpace * i 表示第i个点的x轴的位置 */
    for (int i = 0; i < data.size(); ++i) {
        splineSeries->append(xSpace * i, data.at(i));
    }
}

