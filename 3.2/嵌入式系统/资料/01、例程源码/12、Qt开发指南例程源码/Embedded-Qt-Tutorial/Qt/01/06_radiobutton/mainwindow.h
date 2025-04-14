#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QRadioButton */
#include <QRadioButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明两个QRadioButton对象 */
    QRadioButton *radioButton1;
    QRadioButton *radioButton2;
};
#endif // MAINWINDOW_H
