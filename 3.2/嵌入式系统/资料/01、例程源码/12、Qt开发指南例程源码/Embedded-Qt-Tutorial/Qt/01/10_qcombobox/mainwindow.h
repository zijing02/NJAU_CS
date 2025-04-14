#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QComboBox */
#include <QComboBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QComboBox对象 */
    QComboBox *comboBox;

private slots:
    /* 声明QComboBox对象的槽函数 */
     void comboBoxIndexChanged(int);

};
#endif // MAINWINDOW_H
