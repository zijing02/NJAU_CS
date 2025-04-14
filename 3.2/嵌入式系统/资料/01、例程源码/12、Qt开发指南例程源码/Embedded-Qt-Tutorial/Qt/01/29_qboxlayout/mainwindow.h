#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明按钮对象数组 */
    QPushButton *pushButton[6];

    /* 声明两个widget，用于容纳排布按钮 */
    QWidget *hWidget;
    QWidget *vWidget;

    /* QHBoxLayout与QVBoxLayout对象 */
    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;

};
#endif // MAINWINDOW_H
