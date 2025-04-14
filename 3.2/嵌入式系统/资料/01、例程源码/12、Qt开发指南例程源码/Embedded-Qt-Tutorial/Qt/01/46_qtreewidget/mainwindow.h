#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    /* QTreeWidget对象 */
    QTreeWidget *treeWidget;
    /* 顶层树节点 */
    QTreeWidgetItem *parentItem;
    /* 声明三个子节点 */
    QTreeWidgetItem *subItem[3];

    /* 子节点处理函数 */
    void updateParentItem(QTreeWidgetItem*);

private slots:
    /* 槽函数 */
    void treeItemChanged(QTreeWidgetItem*, int);

};
#endif // MAINWINDOW_H
