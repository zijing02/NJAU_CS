#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFormLayout>
#include <QLineEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    /* widget对象 */
    QWidget *fWidget;

    /* 用于输入用户名 */
    QLineEdit *userLineEdit;

    /* 用于输入密码 */
    QLineEdit *passwordLineEdit;

    /* 声明QFormLayout对象 */
    QFormLayout *formLayout;
};
#endif // MAINWINDOW_H
