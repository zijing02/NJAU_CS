/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   08_ii2_ap3216c_sensor
* @brief         arcgraph.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-22
*******************************************************************/
#include "arcgraph.h"

ArcGraph::ArcGraph(QWidget *parent)
    : QWidget(parent),
      startAngle(90),
      angleLength(100)
{
    this->setMinimumSize(100, 100);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

ArcGraph::~ArcGraph()
{
}

void ArcGraph::setstartAngle(int angle)
{
    startAngle = angle;
    this->repaint();
}

void ArcGraph::setangleLength(int length)
{
    angleLength = length;
    this->repaint();
}

void ArcGraph::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    /* 保存状态 */
    painter.save();

    /* 设置抗锯齿 */
    painter.setRenderHints(QPainter::Antialiasing, true);

    /* 最外层的圆 */
    QRect drawRect = event->rect();
    QRadialGradient gradient1(drawRect.center(),
                              drawRect.width() / 2,
                              drawRect.center());
    gradient1.setColorAt(0, Qt::transparent);
    gradient1.setColorAt(0.5, Qt::transparent);
    gradient1.setColorAt(0.51, QColor("#00237f"));
    gradient1.setColorAt(0.58, QColor("#00237f"));
    gradient1.setColorAt(0.59, Qt::transparent);
    gradient1.setColorAt(1, Qt::transparent);
    painter.setBrush(gradient1);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(drawRect);

    /* 里层的圆 */
    QRadialGradient gradient2(drawRect.center(),
                              drawRect.width() / 2,
                              drawRect.center());
    gradient2.setColorAt(0, Qt::transparent);
    gradient2.setColorAt(0.420, Qt::transparent);
    gradient2.setColorAt(0.421, QColor("#885881e3"));
    gradient2.setColorAt(0.430, QColor("#5881e3"));
    gradient2.setColorAt(0.440, QColor("#885881e3"));
    gradient2.setColorAt(0.441, Qt::transparent);
    gradient2.setColorAt(1, Qt::transparent);
    painter.setBrush(gradient2);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(drawRect);

    /* 数字 */
    QFont font;
    font.setPixelSize(drawRect.width() / 10);
    painter.setPen(Qt::white);
    painter.setFont(font);
    painter.drawText(drawRect, Qt::AlignCenter,
                     QString::number(angleLength * 100 / 360) + "%");

    /* 发光背景圆 */
    painter.translate(drawRect.width() >> 1, drawRect.height() >> 1);
    int radius = drawRect.width() / 2;
    /* radius<< 1（左移1位）相当于radius*2 */
    QRectF rect(-radius, -radius, radius << 1, radius << 1);

    QRadialGradient gradient3(0, 0, radius);
    gradient3.setColorAt(0, Qt::transparent);
    gradient3.setColorAt(0.42, Qt::transparent);
    gradient3.setColorAt(0.51, QColor("#500194d3"));
    gradient3.setColorAt(0.55, QColor("#22c1f3f9"));
    gradient3.setColorAt(0.58, QColor("#500194d3"));
    gradient3.setColorAt(0.68, Qt::transparent);
    gradient3.setColorAt(1.0, Qt::transparent);
    painter.setBrush(gradient3);
    QPainterPath path1;
    path1.arcTo(rect, startAngle, -angleLength);
    painter.setPen(Qt::NoPen);
    painter.drawPath(path1);

    /* 发光圆/弧 */
    QRadialGradient gradient4(0, 0, radius);
    gradient4.setColorAt(0, Qt::transparent);
    gradient4.setColorAt(0.49, Qt::transparent);
    gradient4.setColorAt(0.50, QColor("#4bf3f9"));
    gradient4.setColorAt(0.59, QColor("#4bf3f9"));
    gradient4.setColorAt(0.60, Qt::transparent);
    gradient4.setColorAt(1.0, Qt::transparent);
    painter.setBrush(gradient4);
    QPainterPath path2;
    path2.arcTo(rect, startAngle, -angleLength);
    painter.setPen(Qt::NoPen);
    painter.drawPath(path2);

    /* 恢复状态 */
    painter.restore();

    /* 设置事件对象的accept标志 */
    event->accept();
}
