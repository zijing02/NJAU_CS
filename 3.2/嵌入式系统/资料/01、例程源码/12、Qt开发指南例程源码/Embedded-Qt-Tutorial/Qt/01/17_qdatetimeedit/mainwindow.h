#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTimeEdit>
#include <QTimeEdit>
#include <QDateEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明对象 */
    QDateTimeEdit *dateTimeEdit;
    QTimeEdit *timeEdit;
    QDateEdit *dateEdit;
};
#endif // MAINWINDOW_H
