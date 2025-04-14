#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMdiArea>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* Mdi Area区域对象 */
    QMdiArea *mdiArea;
    /* MdiSubWindow子窗口对象 */
    QMdiSubWindow *newMdiSubWindow;
    /* 用作点击创建新的窗口 */
    QPushButton *pushButton;

private slots:
    /* 按钮槽函数 */
    void creat_newMdiSubWindow();

};
#endif // MAINWINDOW_H
