#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
/* 引入QCommandLinkButton */
#include <QCommandLinkButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QCommandLinkButton对象 */
    QCommandLinkButton *commandLinkButton;

private slots:
    /* 声明槽函数，用于点击commandLinkButton后触发 */
    void commandLinkButtonClicked();


};
#endif // MAINWINDOW_H
