#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeySequenceEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 声明QKeySequenceEdit对象 */
    QKeySequenceEdit  *keySequenceEdit;

private slots:
    /* 声明槽 */
    void KSEKeySequenceChanged(const QKeySequence &);

};
#endif // MAINWINDOW_H
