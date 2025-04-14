#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUndoView>
#include <QUndoStack>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <command.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* 水平布局 */
    QHBoxLayout *hLayout;
    /* 水平布局 */
    QVBoxLayout *vLayout;
    /* 用于容纳hLayout布局 */
    QWidget *mainWidget;
    /* 容器作用QWidget,用于容纳标签与按钮 */
    QWidget *widget;
    /* 存放 QUndoCommand命令的栈 */
    QUndoStack *undoStack;
    /* 历史记录面板 */
    QUndoView *undoView;
    /* 用于显示计算结果 */
    QLabel *label;
    /* 按钮 */
    QPushButton *pushButton;
    /* 计算结果 */
    int count;

private slots:
    void pushButtonClieked();
    void showCountValue(int);
};
#endif // MAINWINDOW_H
