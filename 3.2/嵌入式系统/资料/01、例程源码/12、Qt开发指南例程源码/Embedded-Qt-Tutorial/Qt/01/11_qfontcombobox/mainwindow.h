#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QFontComboBox */
#include <QFontComboBox>
/* 引入QLable */
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QFontComboBox对象 */
    QFontComboBox *fontComboBox;
    /* 声明一个Label对象，用于显示当前字体变化 */
    QLabel *label;

private slots:
    /* 声明QFontComboBox对象使用的槽函数 */
    void fontComboBoxFontChanged(QFont);

};
#endif // MAINWINDOW_H
