#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* 设置位置与大小，下同 */
    this->setGeometry(0, 0, 800, 480);

    /* 对象实例化设置显示的位置与大小 */
    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setGeometry(200, 20, 400, 300);

    QFont font;
    /* 设置日历里字体的大小为10像素 */
    font.setPixelSize(10);
    calendarWidget->setFont(font);

    /* 对象实例化设置显示的位置与大小 */
    pushButton = new QPushButton("回到当前日期",this);
    pushButton->setGeometry(200, 350, 100, 30);

    /* 对象实例化设置显示的位置与大小 */
    label = new QLabel(this);
    label->setGeometry(400, 350, 400, 30);
    QString str = "当前选择的日期:"
            + calendarWidget->selectedDate().toString();
    label->setText(str);

    /* 信号槽连接 */
    connect(calendarWidget, SIGNAL(selectionChanged()),
            this, SLOT(calendarWidgetSelectionChanged()));
    connect(pushButton, SIGNAL(clicked()),
            this, SLOT(pushButtonClicked()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::calendarWidgetSelectionChanged()
{
    /* 当日历点击改变当前选择的期时，更新Label的显示内容 */
    QString str = "当前选择的日期:"
            + calendarWidget->selectedDate().toString();
    label->setText(str);
}

void MainWindow::pushButtonClicked()
{
    /* 设置当前选定的日期为系统的QDate */
    calendarWidget->setSelectedDate(QDate::currentDate());
}

