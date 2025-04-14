#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setGeometry(0, 0, 800, 480);

    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(280, 200, 200, 20);

    pushButton = new QPushButton(this);
    pushButton->setGeometry(500, 200, 50, 20);
    pushButton->setText("确认");

    label = new QLabel(this);
    label->setGeometry(280, 250, 400, 20);
    label->setText("你输入的内容是：");

    /* 信号槽连接 */
    connect(pushButton,SIGNAL(clicked()), this,
            SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::pushButtonClicked()
{
    /* 字符串变量str */
    QString str;

    str = "你输入的内容是：";
    str += lineEdit->text();

    /* 设置label里的文本显示内容 */
    label->setText(str);
    /* 在点击了确认键之后清空lineEdit单行输入框 */
    lineEdit->clear();
}

