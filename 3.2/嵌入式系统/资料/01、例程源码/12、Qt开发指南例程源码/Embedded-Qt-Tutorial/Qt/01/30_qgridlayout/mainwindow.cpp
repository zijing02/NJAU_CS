#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置位置与大小 */
    this->setGeometry(0, 0, 800, 480);

    /* 实例化 */
    gWidget = new QWidget(this);
    /* 设置gWidget居中央 */
    this->setCentralWidget(gWidget);

    gridLayout = new QGridLayout();
    /* QList链表，字符串类型 */
    QList <QString> list;
    list<<"按钮1"<<"按钮2"<<"按钮3"<<"按钮4";
    for (int i = 0; i < 4; i++){
        pushButton[i] = new QPushButton();
        pushButton[i]->setText(list[i]);
        /* 设置最小宽度与高度 */
        pushButton[i]->setMinimumSize(100, 30);
        /* 自动调整按钮的大小 */
        pushButton[i]->setSizePolicy(
                    QSizePolicy::Expanding,
                    QSizePolicy::Expanding
                    );
        switch (i) {
        case 0:
            /* 将pushButton[0]添加至网格的坐标(0,0),下同 */
            gridLayout->addWidget(pushButton[i], 0, 0);
            break;
        case 1:
            gridLayout->addWidget(pushButton[i], 0, 1);
            break;
        case 2:
            gridLayout->addWidget(pushButton[i], 1, 0);
            break;
        case 3:
            gridLayout->addWidget(pushButton[i], 1, 1);
            break;
        default:
            break;
        }
    }
    /* 设置第0行与第1行的行比例系数 */
    gridLayout->setRowStretch(0, 2);
    gridLayout->setRowStretch(1, 3);

    /* 设置第0列与第1列的列比例系数 */
    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 3);

    /* 将gridLayout设置到gWidget */
    gWidget->setLayout(gridLayout);
}

MainWindow::~MainWindow()
{
}

