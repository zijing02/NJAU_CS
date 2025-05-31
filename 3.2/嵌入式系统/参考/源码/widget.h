#ifndef WIDGET_H
#define WIDGET_H


#include <form1.h>
#include <form2.h>
#include <form3.h>
#include <form4.h>
#include <form5.h>
#include <form6.h>
#include <form7.h>
#include <content.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    Form1 *one;

    Form2 *two;

    Form3 *three;

    Form4 *four;

    Form5 *five;

    Form6 *six;

    Form7 *seven;

    QString queryString;

private slots:
    void on_button1_clicked();

    void on_button2_clicked();

    void on_button3_clicked();


    void init_time();

    void update_time();

    void on_button4_clicked();

    void on_cbox1_activated(const QString &arg1);

    void on_cbox2_activated(const QString &arg1);

private:

    bool flag;
    QString color;
    QPoint click;
    QTimer *timer;
    QTime time;
    int tomato_time;
    int total_time;

    void init_word();
    void init_word1();
    void init_word2();

    Ui::Widget *ui;
    QSqlDatabase DB;
};
#endif // WIDGET_H
