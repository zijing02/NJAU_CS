#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* QListView对象 */
    QListView *listView;
    /* 字符串模型对象 */
    QStringListModel *stringListModel;

};
#endif // MAINWINDOW_H
