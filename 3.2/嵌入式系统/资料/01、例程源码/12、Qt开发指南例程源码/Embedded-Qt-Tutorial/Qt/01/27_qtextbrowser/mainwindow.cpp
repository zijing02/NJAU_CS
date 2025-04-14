#include "mainwindow.h"
#include "ui_mainwindow.h"
/* 窗口对话框与文本流 */
#include <QFileDialog>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    /* 设置主窗体位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 将窗口标题设置为文本浏览器 */
    this->setWindowTitle("文本浏览器");

    /* 实例化 */
    textBrowser = new QTextBrowser(this);
    /* 将文本浏览器窗口居中 */
    this->setCentralWidget(textBrowser);

    /* 实例化 */
    openAction = new QAction("打开",this);
    /* ui窗口自带有menubar(菜单栏)、mainToolbar（工具栏）与
     * statusbar（状态栏）
     * menuBar是ui生成工程就有的，所以可以在menubar里添加
     * 我们的QActiont等，如果不需要menubar，可以在ui设计
     * 窗口里，在右则对象里把menubar删除，再自己重新定义自己的
     * 菜单栏
     */
    /* 将动作添加到菜单栏 */
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
    /* 调用系统打开文件窗口，过滤文件名 */
    QString fileName = QFileDialog::getOpenFileName(
                this,tr("打开文件"),"",
                tr("Files(*.txt *.cpp *.h *.html *.htm)")
                );
    QFile myFile(fileName);
    /* 以只读、文本方式打开，若打开失败，则返回 */
    if(!myFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    /* 用QTextStream对象接收 */
    QTextStream in (&myFile);

    /* 读取全部数据 */
    QString myText = in.readAll();

    /* 判断打开文件的后缀，如果是html格式的则设置文本浏览器为html格式 */
    if(fileName.endsWith("html") || fileName.endsWith("htm")){
        textBrowser->setHtml(myText);
    } else {
        textBrowser->setPlainText(myText);
    }

    /* ui窗口自带有statusbar（状态栏），设置打开的文件名 */
    ui->statusbar->showMessage("文件名：" + fileName);
}

