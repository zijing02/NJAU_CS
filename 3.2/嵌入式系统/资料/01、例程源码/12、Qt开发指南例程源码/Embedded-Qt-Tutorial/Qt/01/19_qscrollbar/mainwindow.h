#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollBar>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明水平滚动条对象 */
    QScrollBar *horizontalScrollBar;

    /* 声明垂直滚动条对象 */
    QScrollBar *verticalScrollBar;

    /* 声明标签文本 */
    QLabel *label;
};
#endif // MAINWINDOW_H
