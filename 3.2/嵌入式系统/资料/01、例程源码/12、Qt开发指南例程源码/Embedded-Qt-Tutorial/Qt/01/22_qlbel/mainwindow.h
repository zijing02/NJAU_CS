#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 用一个QLabel对象用于显示字符串 */
    QLabel *labelString;

    /* 用一个QLabel对象用于显示图像 */
    QLabel *labelImage;
};
#endif // MAINWINDOW_H
