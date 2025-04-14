#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QToolButton类 */
#include <QToolButton>
/* 引入QToolBar类 */
#include <QToolBar>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QToolButton对象 */
    QToolButton *toolButton;
    /* 声明一个QToolBar对象 */
    QToolBar *toolBar;
};
#endif // MAINWINDOW_H
