/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2021. All rights reserved.
* @projectName   GlowText
* @brief         glowtext.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @net           www.openedv.com
* @date          2021-05-21
*******************************************************************/
#ifndef GLOWTEXT_H
#define GLOWTEXT_H

#include <QWidget>
#include <QLabel>

class GlowText : public QWidget
{
    Q_OBJECT

public:
    GlowText(QWidget *parent = nullptr);
    ~GlowText();

    void setTextColor(QColor);
    void setFontSize(int);
    void setTextData(QString);

private:
    /* 文本背景 */
    QLabel *textLabelbg;

    /* 文本标签 */
    QLabel *textLabel;

    /* 字体颜色 */
    QColor textColor;

    /* 文本字体大小 */
    int fontSize;

    /* 文本内容 */
    QString textData;
};
#endif // GLOWTEXT_H
