#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QHBoxLayout>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明对象 */
    QWidget *widget;
    QTabWidget *tabWidget;
    QHBoxLayout  *hBoxLayout;
    QLabel *label[3];
};
#endif // MAINWINDOW_H
