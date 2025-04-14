#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPlainTextEdit>
#include <QRadioButton>

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明对象 */
    QPlainTextEdit *plainTextEdit;
    QRadioButton *radioButton;

private slots:
    /* 槽函数 */
    void radioButtonClicked();

};
#endif // MAINWINDOW_H
