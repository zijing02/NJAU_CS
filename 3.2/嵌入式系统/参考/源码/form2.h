#ifndef FORM2_H
#define FORM2_H

#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form2; }
QT_END_NAMESPACE

class Form2: public QWidget
{
    Q_OBJECT

public:
    Form2(QWidget *parent = nullptr);
    ~Form2();
    Ui::Form2 *ui;

private slots:


    void on_btnA_clicked();

    void on_btnB_clicked();

private:

};

#endif // FORM2_H
