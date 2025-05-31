#ifndef FORM5_H
#define FORM5_H

#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form5; }
QT_END_NAMESPACE

class Form5: public QWidget
{
    Q_OBJECT

public:
    Form5(QWidget *parent = nullptr);
    ~Form5();
    Ui::Form5 *ui;

private slots:

    void on_btn_save_clicked();

    void on_btn_show_clicked();

private:

};


#endif // FORM5_H
