/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   appdemo
* @brief         appdemo.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-06-10
*******************************************************************/
#include "appdemo.h"
#include <QFile>

AppDemo::AppDemo(QWidget *parent)
    : QWidget(parent)
{
    this->resize(800, 480);
    this->setMinimumSize(800, 480);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    mainWidget = new QWidget(this);
    topWidget = new QWidget();
    middleWidget = new QWidget();
    bottomWidget = new QWidget();

    mid_left_Widget = new QWidget();
    mid_right_Widget = new QWidget();

    mid_right_top_Widget = new QWidget();
    mid_right_bottom_Widget = new QWidget();


    for (int i = 0; i < 6; i++) {
        mid_pushButton[i] = new QPushButton();
        mid_pushButton[i]->setObjectName(tr("mid_pushButton%1").arg(QString::number(i)));
        mid_pushButton[i]->setFocusPolicy(Qt::NoFocus);

        if (i != 5) {
            top_pushButton[i] = new QPushButton();
            top_pushButton[i]->setFixedSize(120, 60);
            top_pushButton[i]->setObjectName(tr("top_pushButton%1").arg(QString::number(i)));
            top_pushButton[i]->setFocusPolicy(Qt::NoFocus);
        }
    }

    for (int i = 0; i < 7; i++) {
        bottom_pushButton[i] = new QPushButton();
        bottom_pushButton[i]->setFixedSize(60, 60);
        bottom_pushButton[i]->setObjectName(tr("bottom_pushButton%1").arg(QString::number(i)));
        bottom_pushButton[i]->setFocusPolicy(Qt::NoFocus);
    }


    mainVLayout = new QVBoxLayout();
    topHLayout = new QHBoxLayout();
    bottomHLayout = new QHBoxLayout();
    middleHLayout = new QHBoxLayout();
    mid_left_VLayout = new QVBoxLayout();
    mid_right_VLayout = new QVBoxLayout();
    mid_right_top_HLayout = new QHBoxLayout();
    mid_right_bottom_HLayout = new QHBoxLayout();

    /* 主布局 */
    mainVLayout->addWidget(topWidget);
    mainVLayout->addWidget(middleWidget);
    middleWidget->setFixedHeight(305);
    mainVLayout->addWidget(bottomWidget);
    mainWidget->setLayout(mainVLayout);
    mainVLayout->setAlignment(Qt::AlignCenter);
    mainVLayout->setContentsMargins(0, 0, 0, 0);

    /* 中间布局 */
    middleHLayout->addWidget(mid_left_Widget);
    middleHLayout->setAlignment(Qt::AlignCenter);
    mid_left_Widget->setFixedSize(305, 305);
    middleHLayout->addWidget(mid_right_Widget);
    middleHLayout->setContentsMargins(5, 0, 0, 0);
    middleWidget->setLayout(middleHLayout);
    middleWidget->setFixedSize(795, 300);

    /* 中间左边布局 */
    mid_left_VLayout->addWidget(mid_pushButton[0]);
    mid_pushButton[0]->setFixedSize(305, 305);
    mid_left_VLayout->setContentsMargins(0, 0, 0, 0);
    mid_left_Widget->setLayout(mid_left_VLayout);

    /* 中间右边布局 */
    mid_right_VLayout->addWidget(mid_right_top_Widget);
    mid_right_VLayout->addWidget(mid_right_bottom_Widget);
    mid_right_VLayout->setContentsMargins(0, 0, 0, 0);
    mid_right_Widget->setLayout(mid_right_VLayout);

    /* 中间右边顶部布局 */
    mid_right_top_HLayout->addWidget(mid_pushButton[1]);
    mid_pushButton[1]->setFixedSize(150, 150);
    mid_pushButton[1]->setText("Name of song");
    mid_right_top_HLayout->addWidget(mid_pushButton[2]);
    mid_pushButton[2]->setFixedSize(150, 150);
    mid_pushButton[2]->setText("Categories");
    mid_right_top_HLayout->addWidget(mid_pushButton[3]);
    mid_pushButton[3]->setFixedSize(150, 150);
    mid_pushButton[3]->setText("Top List");
    mid_right_top_HLayout->setContentsMargins(0, 0, 0, 0);
    mid_right_top_Widget->setLayout(mid_right_top_HLayout);

    /* 中间右边底部布局 */
    mid_right_bottom_HLayout->addWidget(mid_pushButton[4]);
    mid_pushButton[4]->setFixedSize(150, 150);
    mid_pushButton[4]->setText("New");
    mid_right_bottom_HLayout->addWidget(mid_pushButton[5]);
    mid_pushButton[5]->setFixedHeight(150);
    mid_pushButton[5]->setText("Stars");
    mid_right_bottom_HLayout->setContentsMargins(0, 0, 0, 0);
    mid_right_bottom_Widget->setLayout(mid_right_bottom_HLayout);

    /* 顶部布局 */
    for (int i = 0; i < 5; i++) {
        if (i == 4) {
            QSpacerItem *hSpacer0 = new QSpacerItem(150, 10, QSizePolicy::Fixed, QSizePolicy::Fixed);
            topHLayout->addSpacerItem(hSpacer0);
        }
        topHLayout->addWidget(top_pushButton[i]);
        topHLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter | Qt::AlignLeft);
        topHLayout->setContentsMargins(0, 0, 0, 20);
    }
    topHLayout->setSpacing(20);
    top_pushButton[1]->setText("Select songs");
    top_pushButton[2]->setText("Interation");
    top_pushButton[3]->setText("More         ");
    top_pushButton[4]->setText("Search       ");
    topWidget->setFixedHeight(80);
    topWidget->setFixedWidth(800);
    topWidget->setLayout(topHLayout);

    /* 底部布局 */
    for (int i = 0; i < 7; i++) {
        bottomHLayout->addWidget(bottom_pushButton[i]);
    }

    bottomHLayout->setContentsMargins(5, 0, 5, 0);
    bottomHLayout->setAlignment(Qt::AlignLeft);
    bottomHLayout->setSpacing(62);
    bottomWidget->setFixedHeight(100);
    bottomWidget->setLayout(bottomHLayout);
    bottom_pushButton[0]->setText("Service");
    bottom_pushButton[1]->setText("Score");
    bottom_pushButton[2]->setText("Volume");
    bottom_pushButton[4]->setText("Original");
    bottom_pushButton[5]->setText("Next");
    bottom_pushButton[6]->setText("List");

    /* 加载qss样式文件 */
    loadStyle();
}

AppDemo::~AppDemo()
{
}

void AppDemo::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    mainWidget->resize(this->size());
}

void AppDemo::loadStyle()
{
    /* 指定文件 */
    QFile file(":/style.qss");

    /* 判断文件是否存在 */
    if (file.exists() ) {
        /* 以只读的方式打开 */
        file.open(QFile::ReadOnly);
        /* 以字符串的方式保存读出的结果 */
        QString styleSheet = QLatin1String(file.readAll());
        /* 设置全局样式 */
        this->setStyleSheet(styleSheet);
        /* 关闭文件 */
        file.close();
    }
}

