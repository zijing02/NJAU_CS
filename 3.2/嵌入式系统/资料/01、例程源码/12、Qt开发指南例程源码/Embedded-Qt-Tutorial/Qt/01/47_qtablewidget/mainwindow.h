#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* QTabelWidget表格 */
    QTableWidget *tableWidget;

    /* QTabelWidgetItem表格数据（项） */
    QTableWidgetItem *tableWidgetItem[4];

};
#endif // MAINWINDOW_H
