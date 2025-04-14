/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   08_spi_sensor
* @brief         arcgraph.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-21
*******************************************************************/
#ifndef ARCGRAPH_H
#define ARCGRAPH_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

/* 蓝色科技感弧形视图 */
class ArcGraph : public QWidget
{
    Q_OBJECT

public:
    ArcGraph(QWidget *parent = nullptr);
    ~ArcGraph();

    void setstartAngle(int);
    void setangleLength(int);

private:
    void paintEvent(QPaintEvent *event);
    int startAngle;
    int angleLength;
};
#endif // ARCGRAPH_H
