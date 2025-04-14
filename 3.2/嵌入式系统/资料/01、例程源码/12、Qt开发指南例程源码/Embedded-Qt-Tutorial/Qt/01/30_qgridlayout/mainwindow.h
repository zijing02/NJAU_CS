#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:

    /* 声明widget窗口部件，用于容纳下面4个pushButton按钮 */
    QWidget *gWidget;

    /* 声明QGridLayout对象 */
    QGridLayout *gridLayout;

    /* 声明pushButton按钮数组 */
    QPushButton *pushButton[4];

};
#endif // MAINWINDOW_H
