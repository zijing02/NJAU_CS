#ifndef FORM4_H
#define FORM4_H

#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form4; }
QT_END_NAMESPACE

class Form4: public QWidget
{
    Q_OBJECT

public:
    Form4(QWidget *parent = nullptr);
    ~Form4();

private slots:


    void on_btn_save_clicked();

    void on_btn_pre_clicked();

    void on_btn_next_clicked();

private:
    Ui::Form4 *ui;
};


#endif // FORM4_H
