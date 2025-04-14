/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   08_spi_sensor
* @brief         mainwindow.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-21
*******************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "arcgraph.h"
#include "glowtext.h"
#include "ap3216c.h"
#include "headview/headview.h"
class ArcGraph;
class GlowText;
class Ap3216c;
class HeadView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    ArcGraph *arcGraph[3];
    GlowText *glowText[3];

    QVBoxLayout *vBoxLayout;
    QHBoxLayout *hBoxLayout[5];

    GlowText *test;

    /* 容器作用，用于布局 */
    QWidget *widget[6];

    /* 标签文本 */
    QLabel *label[3];

    /* ii2传感器类 */
    Ap3216c *ap3216c;

    /* 视图表头 */
    HeadView *headView;

private slots:
    /* 获取ap3216传感器数据 */
    void getAp3216cData();
};
#endif // MAINWINDOW_H
