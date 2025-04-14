#include "mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置主窗体显示的位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例一个水平布局，用于左侧按钮区域与右侧历史记录面板 */
    hLayout = new QHBoxLayout();

    /* 实例一个水平布局，用于左侧标签与按钮 */
    vLayout = new QVBoxLayout();

    /* 主Widget, 因为MainWindow自带一个布局，
     * 我们要新建一个Widget容纳新布局
     */
    mainWidget = new QWidget();

    /* 用于存放命令行栈 */
    undoStack = new QUndoStack(this);

    /* 用于容纳左侧标签与按钮布局 */
    widget = new QWidget();

    /* 历史记录面板实例化 */
    undoView = new QUndoView(undoStack);

    /* 实例化一个按钮，用于加一操作 */
    pushButton = new QPushButton();

    /* 标签，用于显示计算结果 */
    label = new QLabel();

    /* 设置widget的大小 */
    widget->setMinimumSize(400, 480);

    /* 将两个widget添加到水平布局 */
    hLayout->addWidget(widget);
    hLayout->addWidget(undoView);

    /* 初始化count的值 */
    count = 0;

    /* 显示初始化计算结果 */
    label->setText("计算结果：" + QString::number(count));
    label->setAlignment(Qt::AlignCenter);

    /* 左侧布局 */
    vLayout->addWidget(label);
    vLayout->addWidget(pushButton);

    /* 左侧布局控件的高度设置 */
    label->setMaximumHeight(this->height() / 5);
    pushButton->setMaximumHeight(this->height() / 5);

    /* 按钮文件设置 */
    pushButton->setText("加1");

    /* 设置widget的布局为vLayout */
    widget->setLayout(vLayout);

    /* 将主窗体的布局设置为hLayout */
    mainWidget->setLayout(hLayout);

    /* 设置mainWidget为主窗体的居中widget */
    this->setCentralWidget(mainWidget);

    /* 信号槽连接，按钮点击，执行加一操作 */
    connect(pushButton, SIGNAL(clicked()), this,
            SLOT(pushButtonClieked()));

    /* 信号槽连接，历史记录项index发生变化，显示count大小 */
    connect(undoStack, SIGNAL(indexChanged(int) ),
            this, SLOT(showCountValue(int)));
}

/* 入栈操作会自动调用addCommand的redo */
void MainWindow::pushButtonClieked()
{
    /* 变量值加一 */
    count++;

    /* value指向count的地址 */
    int *value = &count;

    /* 用重写的addCommand类实例化 */
    QUndoCommand *add = new addCommand(value);

    /* 入栈 */
    undoStack->push(add);
}

void MainWindow::showCountValue(int)
{
    /* 标签用于显示计算结果 */
    label->setText("计算结果：" + QString::number(count));
}

MainWindow::~MainWindow()
{

}

