#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDial>
#include <QLabel>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明对象 */
    QDial *dial;
    QLabel *label;

private slots:
    /* 槽函数 */
    void dialValueChanged(int);

};
#endif // MAINWINDOW_H
