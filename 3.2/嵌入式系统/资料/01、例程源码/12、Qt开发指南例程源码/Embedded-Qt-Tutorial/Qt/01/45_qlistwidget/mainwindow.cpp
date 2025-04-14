#include "mainwindow.h"
#include "QFileDialog"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗口的显示位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    listWidget = new QListWidget(this);

    /* 设置listWidget的大小 */
    listWidget->setGeometry(0, 0, 480, 480);

    listWidget->addItem("请单击右边的添加项添加内容");

    pushButton = new QPushButton(this);

    /* 设置pushButton的位置与大小 */
    pushButton->setGeometry(540, 200, 200, 100);
    pushButton->setText("添加项");

    /* 信号与槽连接 */
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(pushButtonClicked()));
}

void MainWindow::pushButtonClicked()
{
    /* 调用系统打开文件窗口，设置窗口标题为“打开文件”，过滤文件名 */
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("添加项"),"",
                tr("Files(*.mp3)")
                );

    /* 判断是否选中打开mp3文件 */
    if (fileName != NULL)
        /* 添加项到列表中 */
        listWidget->addItem(fileName);
}

MainWindow::~MainWindow()
{
}

