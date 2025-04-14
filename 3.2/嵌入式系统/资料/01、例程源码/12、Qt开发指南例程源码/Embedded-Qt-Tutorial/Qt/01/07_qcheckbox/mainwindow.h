#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QCheckBox */
#include <QCheckBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QCheckBox对象 */
    QCheckBox *checkBox;
private slots:
    /* 声明QCheckBox的槽函数，并带参数传递，用这个参数接收信号的参数 */
    void checkBoxStateChanged(int);

};
#endif // MAINWINDOW_H
