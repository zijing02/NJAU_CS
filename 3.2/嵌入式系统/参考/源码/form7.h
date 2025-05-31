#ifndef FORM7_H
#define FORM7_H

#include <content.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Form7; }
QT_END_NAMESPACE

class Form7: public QWidget
{
    Q_OBJECT

public:
    Form7(QWidget *parent = nullptr);
    ~Form7();
    Ui::Form7 *ui;

private slots:


    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_tabel_doubleClicked(const QModelIndex &index);

private:

};

#endif // FORM7_H
