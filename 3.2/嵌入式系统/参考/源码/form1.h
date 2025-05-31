#ifndef FORM1_H
#define FORM1_H

#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form1; }
QT_END_NAMESPACE

class Form1: public QWidget
{
    Q_OBJECT

public:
    Form1(QWidget *parent = nullptr);
    ~Form1();
    Ui::Form1 *ui;

private slots:

    void on_btA_clicked();

    void on_btB_clicked();

    void on_btC_clicked();

    void on_btD_clicked();

    void Rand();

    void on_btn_next_clicked();

private:

};
#endif // FORM1_H
