#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTextEdit>
#include <QPushButton>

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QTextEdit对象 */
    QTextEdit *textEdit;

    /* 声明两个QPushButton对象 */
    QPushButton *pushButtonSelectAll;
    QPushButton *pushButtonClearAll;

private slots:
    /* 声明两个槽函数，响应按钮点击响应的事件 */
    void pushButtonSelectAllClicked();
    void pushButtonClearAllClicked();

};
#endif // MAINWINDOW_H
