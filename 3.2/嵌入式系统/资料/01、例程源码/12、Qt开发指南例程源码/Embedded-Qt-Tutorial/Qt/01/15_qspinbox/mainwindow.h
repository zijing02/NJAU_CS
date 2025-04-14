#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QSpinBox对象 */
    QSpinBox *spinBox;
private slots:
    /* 槽函数 */
    void spinBoxValueChanged(int);
};
#endif // MAINWINDOW_H
