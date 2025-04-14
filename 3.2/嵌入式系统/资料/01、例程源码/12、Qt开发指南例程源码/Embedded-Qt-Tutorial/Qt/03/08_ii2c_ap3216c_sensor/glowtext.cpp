/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   GlowText
* @brief         glowtext.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-21
*******************************************************************/
#include "glowtext.h"
#include <QDebug>
#include <QGraphicsBlurEffect>

GlowText::GlowText(QWidget *parent)
    : QWidget(parent),
      textColor("#4bf3f9"),
      fontSize(18),
      textData("100")
{
    QFont font;
    font.setPixelSize(fontSize);
    QPalette pal;
    pal.setColor(QPalette::WindowText, textColor);
    textLabelbg = new QLabel(this);
    textLabelbg->setAttribute(Qt::WA_TranslucentBackground, true);
    textLabelbg->setPalette(pal);
    textLabelbg->setFont(font);
    textLabelbg->setText(textData);
    textLabelbg->setAlignment(Qt::AlignCenter);

    /* 设置模糊特效 */
    QGraphicsBlurEffect *ef = new QGraphicsBlurEffect();
    ef->setBlurRadius(25);
    ef->setBlurHints(QGraphicsBlurEffect::QualityHint);
    textLabelbg->setGraphicsEffect(ef);

    textLabel = new QLabel(this);
    textLabel->setAttribute(Qt::WA_TranslucentBackground, true);
    textLabel->setPalette(pal);
    textLabel->setFont(font);
    textLabel->setText(textData);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabelbg->adjustSize();
    textLabel->adjustSize();

    this->resize(textLabel->size().width() + 10,
                 textLabel->size().height() + 10);
    /* 背景透明化 */
    this->setAttribute(Qt::WA_TranslucentBackground, true);
}

GlowText::~GlowText()
{
}

void GlowText::setTextColor(QColor color)
{
    QPalette pal;
    pal.setColor(QPalette::WindowText, color);
    textLabelbg->setPalette(pal);
    textLabel->setPalette(pal);
}

void GlowText::setFontSize(int size)
{
    QFont font;
    font.setPixelSize(size);

    textLabelbg->setFont(font);
    textLabel->setFont(font);

    textLabel->adjustSize();
    textLabelbg->adjustSize();
    this->resize(textLabel->size().width() + 10,
                 textLabel->size().height() + 10);
}

void GlowText::setTextData(QString text)
{
    textLabelbg->setText(text);
    textLabel->setText(text);

    textLabel->adjustSize();
    textLabelbg->adjustSize();
    this->resize(textLabel->size().width() + 10,
                 textLabel->size().height() + 10);
}
