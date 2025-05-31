#include "form6.h"
#include "ui_form6.h"
Form6::Form6(QWidget *parent): QWidget(parent), ui(new Ui::Form6)
{
    ui->setupUi(this);
}
Form6::~Form6(){delete ui;}

void Form6::on_btn_pre_clicked()
{
    idx = (idx - 1 + words.size()) % words.size();
    ui->input  ->setText(words[idx][1]);
    ui->input_2->setText(words[idx][0]);
    ui->input_3->setText(words[idx][2]);
}

void Form6::on_btn_next_clicked()
{
    idx = (idx + 1) % words.size();
    ui->input  ->setText(words[idx][1]);
    ui->input_2->setText(words[idx][0]);
    ui->input_3->setText(words[idx][2]);
}
