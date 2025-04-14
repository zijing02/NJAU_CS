#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include <QLCDNumber>
#include <QTimer>
#include <QTime>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明QLCDNumber对象 */
    QLCDNumber *lcdNumber;

    /* 声明QTimer对象 */
    QTimer *timer;

private slots:
    /* 槽函数 */
    void timerTimeOut();

};
#endif // MAINWINDOW_H
