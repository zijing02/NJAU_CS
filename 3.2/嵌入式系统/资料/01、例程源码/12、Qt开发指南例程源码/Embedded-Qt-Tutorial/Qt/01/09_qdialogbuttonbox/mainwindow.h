#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QDialogButtonBox */
#include <QDialogButtonBox>
/* 引入QPuhsButton */
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QDialogButtonBox对象 */
    QDialogButtonBox *dialogButtonBox;

    /* 声明一个QPushButton对象 */
    QPushButton *pushButton;

private slots:
    /* 声明信号槽，带QAbstractButton *参数，用于判断点击了哪个按钮 */
    void dialogButtonBoxClicked(QAbstractButton *);

};
#endif // MAINWINDOW_H
