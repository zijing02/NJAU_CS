#ifndef FORM6_H
#define FORM6_H

#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form6; }
QT_END_NAMESPACE

class Form6: public QWidget
{
    Q_OBJECT

public:
    Form6(QWidget *parent = nullptr);
    ~Form6();
    Ui::Form6 *ui;

private slots:


    void on_btn_pre_clicked();

    void on_btn_next_clicked();

private:

};

#endif // FORM6_H
