#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 设置主窗体大小 */

    this->setGeometry(0, 0, 800, 480);
    /* 将窗口标题设置为图像浏览器 */
    this->setWindowTitle("图像浏览器");

    /* 实例化图形视图对象 */
    graphicsView = new QGraphicsView(this);
    /* 将图像浏览器窗口居中 */
    this->setCentralWidget(graphicsView);

    /* 实例化场景对象 */
    graphicsScene = new QGraphicsScene(this);

    /* 在QGraphicsView设置场景 */
    graphicsView->setScene(graphicsScene);

    /* 将动作添加到菜单栏 */
    openAction = new QAction("打开",this);
    ui->menubar->addAction(openAction);

    /* 信号槽连接 */
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(openActionTriggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openActionTriggered()
{
    /*调用系统打开文件窗口，设置窗口标题为“打开文件”，过滤文件名*/
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("打开文件"), "",
                tr("Files(*.png *.jpg *.bmp)")
                );
    /* 定义QPixmap对象，指向fileName */
    QPixmap image(fileName);
    /* 将image用scaled来重新设置长宽为graphicsView的长宽，
     * 保持纵横比等
     */

    /* 假若用户没选择文件，则返回 */
    if(image.isNull())
        return;
    image = image.scaled(graphicsView->width(),
                          graphicsView->height(),
                          Qt::KeepAspectRatio,
                          Qt::FastTransformation
                          );
    /* 在添加场景内容前，先清除之前的场景内容 */
    graphicsScene->clear();
    /* 添加场景内容image */
    graphicsScene->addPixmap(image);
    /* ui窗口自带有statusBar（状态栏），设置打开的文件名 */
    ui->statusbar->showMessage("文件名：" + fileName);
}

