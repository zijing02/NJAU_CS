#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QSpacerItem>
#include <QBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 按钮对象数组 */
    QPushButton *bt[4];
    /* 垂直间隔 */
    QSpacerItem *vSpacer;
    /* 水平间隔 */
    QSpacerItem *hSpacer;
    /* 声明一个widget用来存放布局的内容 */
    QWidget *widget;
    /* 主布局对象 */
    QHBoxLayout *mainLayout;
    /* 垂直布局对象 */
    QVBoxLayout *vBoxLayout;
    /* 水平布局对象 */
    QHBoxLayout *hBoxLayout;

};
#endif // MAINWINDOW_H
