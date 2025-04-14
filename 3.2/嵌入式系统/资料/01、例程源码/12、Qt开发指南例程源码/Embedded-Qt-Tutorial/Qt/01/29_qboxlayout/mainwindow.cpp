#include "mainwindow.h"
#include <QList>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗口的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化与设置位置大小 */
    hWidget = new QWidget(this);
    hWidget->setGeometry(0, 0, 800, 240);

    vWidget = new QWidget(this);
    vWidget->setGeometry(0, 240, 800, 240);

    hLayout = new QHBoxLayout();
    vLayout = new QVBoxLayout();

    /* QList<T>是Qt的一种泛型容器类。
     * 它以链表方式存储一组值，
     * 并能对这组数据进行快速索引
     */
    QList <QString>list;
    /* 将字符串值插入list */
    list<<"one"<<"two"<<"three"<<"four"<<"five"<<"six";

    /* 用一个循环实例化6个按钮 */
    for(int i = 0; i < 6; i++){
        pushButton[i] = new QPushButton();
        pushButton[i]->setText(list[i]);
        if(i < 3) {
            /* 将按钮添加至hLayout中 */
            hLayout->addWidget(pushButton[i]);
        } else {
            /* 将按钮添加至vLayout中 */
            vLayout->addWidget(pushButton[i]);
        }
    }
    /* 设置间隔为50 */
    hLayout->setSpacing(50);

    /* hWidget与vWidget的布局设置为hLayout/vLayout */
    hWidget->setLayout(hLayout);
    vWidget->setLayout(vLayout);
}

MainWindow::~MainWindow()
{
}

