#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBox>
#include <QGroupBox>
#include <QToolButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明对象 */
    QToolBox *toolBox;
    QGroupBox *groupBox[2];
    QVBoxLayout  *vBoxLayout[2];
    QToolButton *toolButton[6];

};
#endif // MAINWINDOW_H
