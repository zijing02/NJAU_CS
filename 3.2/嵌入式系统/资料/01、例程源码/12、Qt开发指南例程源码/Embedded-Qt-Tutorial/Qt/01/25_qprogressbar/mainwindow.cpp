#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    progressBar = new QProgressBar(this);
    progressBar->setGeometry(300, 200, 200, 60);

    /*样式表设置，常用使用setStyleSheet来设置样式（实现界面美化的功能），
     * 具体可参考seTyleSheet */
    progressBar->setStyleSheet (
                "QProgressBar{border:8px solid #FFFFFF;"
                "height:30;"
                "border-image:url(:/images/battery.png);" //背景图片
                "text-align:center;"    // 文字居中
                "color:rgb(255,0,255);"
                "font:20px;"      // 字体大小为20px
                "border-radius:10px;}"
                "QProgressBar::chunk{"
                "border-radius:5px;" // 斑马线圆角
                "border:1px solid black;" // 黑边，默认无边
                "background-color:skyblue;"
                "width:10px;margin:1px;}" // 宽度和间距
                );

    /* 设置progressBar的范围值 */
    progressBar->setRange(0, 100);
    /* 初始化value为0 */
    value = 0;
    /* 给progressBar设置当前值 */
    progressBar->setValue(value);
    /* 设置当前文本字符串的显示格式 */
    progressBar->setFormat("充电中%p%");

    /* 定时器实例化设置每100ms发送一个timeout信号 */
    timer = new QTimer(this);
    timer->start(100);

    /* 信号槽连接 */
    connect(timer, SIGNAL(timeout()),
            this, SLOT(timerTimeOut()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::timerTimeOut()
{
    /* 定显示器时间到，value值自加一 */
    value ++;
    progressBar->setValue(value);
    /* 若value值大于100，令value再回到0 */
    if(value>100)
        value = 0;
}

