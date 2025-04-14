#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明一个QDoubleSpinBox对象 */
    QDoubleSpinBox *doubleSpinBox;

private slots:
    /* 槽函数 */
    void doubleSpinBoxValueChanged(double);

};
#endif // MAINWINDOW_H
