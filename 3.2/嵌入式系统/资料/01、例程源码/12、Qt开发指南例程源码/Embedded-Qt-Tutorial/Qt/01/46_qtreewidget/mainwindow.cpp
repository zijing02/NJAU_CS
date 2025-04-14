#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化 */
    treeWidget = new QTreeWidget(this);

    /* 居中 */
    setCentralWidget(treeWidget);

    /* 清空列表 */
    treeWidget->clear();

    /* 实例化顶层树节点 */
    parentItem = new QTreeWidgetItem(treeWidget);
    parentItem->setText(0, "同事");

    parentItem->setFlags(
                Qt::ItemIsUserCheckable
                | Qt::ItemIsEnabled
                | Qt::ItemIsSelectable
                );
    /* 树节点设置为未选中 */
    parentItem->setCheckState(0, Qt::Unchecked);

    /* 字符串链表 */
    QList <QString> strList;
    strList<<"关羽"<<"刘备"<<"张飞";

    for (int i = 0; i < 3; i++){
        /* 实例化子节点 */
        subItem[i] = new QTreeWidgetItem(parentItem);
        /* 设置子节点的文本，参数0代表第0列 */
        subItem[i]->setText(0, strList[i]);
        /* 设置子节点的属性为用户可选、项开启、项可选 */
        subItem[i]->setFlags(
                    Qt::ItemIsUserCheckable
                    | Qt::ItemIsEnabled
                    | Qt::ItemIsSelectable
                    );
        /* 设置子节点的状态为未选中 */
        subItem[i]->setCheckState(0,Qt::Unchecked);
    }
    /* 信号槽连接 */
    connect(treeWidget,SIGNAL(itemChanged(QTreeWidgetItem* , int)),
            this, SLOT(treeItemChanged(QTreeWidgetItem* , int)));

}

/* 更新树节点函数 */
void MainWindow::updateParentItem(QTreeWidgetItem *item)
{
    /* 获取子节点的父节点（树节点） */
    QTreeWidgetItem* parent = item->parent();
    if(parent == NULL){
        return;
    }
    /* 初始化选中的数目为0，下面根据selectCount来判断树节点的状态 */
    int selectCount = 0;
    /* 获取树节点的子节点总数 */
    int childCount = parent->childCount();
    /* 循环判断子节点的状态 */
    for(int i = 0; i < childCount; i ++){
        QTreeWidgetItem* childItem =parent->child(i);
        /* 判断当前子节点的状是否为选中状态，如果是，则加一 */
        if(childItem->checkState(0) == Qt::Checked) {
            selectCount ++;
        }
    }
    /* 根据selectCount来判断树节点的状态 */
    /* 当选中的子节点小于或等于0时，则为设置树节点为未选中状态 */
    if (selectCount <= 0) {
        /* 设置树节点为未选中状态 */
        parent->setCheckState(0, Qt::Unchecked);
        /* 部分选中时，树节点为半选状态 */
    } else if (selectCount > 0 && selectCount < childCount) {
        /* 设置为半选状态 */
        parent->setCheckState(0, Qt::PartiallyChecked);
        /* 子节点全选时 */
    } else if (selectCount == childCount){
        /* 设置为树节点为选中状态 */
        parent->setCheckState(0, Qt::Checked);
    }
}

void MainWindow::treeItemChanged(QTreeWidgetItem *item, int)
{
    /* 获取子节点总数 */
    int count = item->childCount();

    /* 若顶层树节点选中 */
    if(Qt::Checked == item->checkState(0) ) {
        /* 若选中的项是树节点，count会大于0，否则选中的项是子节点 */
        if (count > 0) {
            for (int i = 0; i < count; i++) {
                /* 子节点全选 */
                item->child(i)->setCheckState(0, Qt::Checked);
            }
        } else {
            /* 子节点处理 */
            updateParentItem(item);
        }
        /* 若顶层树节点取消选中时 */
    } else if (Qt::Unchecked == item->checkState(0)) {
        if (count > 0){
            /* 若选中的项是树节点，count会大于0，否则选中的项是子节点 */
            for (int i = 0; i < count; i++) {
                /* 子节点全不选 */
                item->child(i)->setCheckState(0, Qt::Unchecked);
            }
        } else {
            /* 子节点处理 */
            updateParentItem(item);
        }
    }
}

MainWindow::~MainWindow()
{
}

