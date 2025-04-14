#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置窗口的显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);
    pushButton = new QPushButton("新建窗口", this);
    pushButton->setGeometry(0, 30, 100, 30);

    mdiArea = new QMdiArea(this);
    /* 设置MDI Area区域大小 */
    mdiArea->setGeometry(100, 30, 700, 430);
    /* 连接信号槽 */
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(creat_newMdiSubWindow()));
}

void MainWindow::creat_newMdiSubWindow()
{
    newMdiSubWindow = new QMdiSubWindow();
    newMdiSubWindow->setWindowTitle("新建窗口");

    /* 如果窗口设置了Qt::WA_DeleteOnClose 这个属性，
     * 在窗口接受了关闭事件后，Qt会释放这个窗口所占用的资源
     */
    newMdiSubWindow->setAttribute(Qt::WA_DeleteOnClose);

    /* 添加子窗口 */
    mdiArea->addSubWindow(newMdiSubWindow);
    /* 显示窗口，不设置时为不显示 */
    newMdiSubWindow->show();
    /* 自适应窗口 */
    newMdiSubWindow->sizePolicy();
    /* 以级联的方式排列所有窗口 */
    // mdiArea->cascadeSubWindows();
    /* 以平铺方式排列所有窗口 */
    mdiArea->tileSubWindows();
}

MainWindow::~MainWindow()
{
}

