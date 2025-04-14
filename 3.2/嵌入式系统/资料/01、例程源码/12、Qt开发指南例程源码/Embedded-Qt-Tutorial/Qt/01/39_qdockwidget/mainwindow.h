#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDockWidget>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 停靠窗口部件 */
    QDockWidget *dockWidget;
    /* 文本编辑框 */
    QTextEdit *textEdit;

};
#endif // MAINWINDOW_H
