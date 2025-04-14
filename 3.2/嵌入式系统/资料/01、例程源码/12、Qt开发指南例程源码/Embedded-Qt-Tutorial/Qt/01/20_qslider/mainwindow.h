#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明对象 */
    QSlider *horizontalSlider;
    QSlider *verticalSlider;
    QLabel *label;
private slots:
    /* 槽函数 */
    void horizontalSliderValueChanged(int);
    void verticalSliderValueChanged(int);

};
#endif // MAINWINDOW_H
