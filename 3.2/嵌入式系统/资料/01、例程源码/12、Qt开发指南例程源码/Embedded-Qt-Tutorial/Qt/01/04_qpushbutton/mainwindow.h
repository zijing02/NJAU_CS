#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QPushButton类 */
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QPushButton对象pushButton1 */
    QPushButton *pushButton1;
    /* 声明一个QPushButton对象pushButton2 */
    QPushButton *pushButton2;

private slots:
    /* 声明对象pushButton1的槽函数 */
    void pushButton1_Clicked();
    /* 声明对象pushButton2的槽函数 */
    void pushButton2_Clicked();
};
#endif // MAINWINDOW_H
