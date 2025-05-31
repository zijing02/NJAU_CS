#include "form4.h"
#include "ui_form4.h"

extern vector<QStringList> new_words;
extern int idx1;
extern QString wf,rf;
Form4::Form4(QWidget *parent): QWidget(parent), ui(new Ui::Form4)
{
    ui->setupUi(this);
}
Form4::~Form4(){delete ui;}


void Form4::on_btn_save_clicked()
{
    for (int i = 0; i < new_words.size(); i++)
    {
        QStringList row = new_words.at(i);
        QString queryString = QString("insert into data (") + "'word'" + "," +  "'meaning'" + "," + "'remark'" + "," + "'title'" + ")" + \
      " values (" +  "'" + row.at(0) + "'" + "," + "'" + row.at(1) + "'" + "," + "'" + row.at(2) + "'" + "," + "'" + write_word_name + "'" + ");";
        qDebug() << queryString << '\n';
        QSqlQuery query;
        query.prepare(queryString);
        if(query.exec()){
            qDebug() << "successfully insert" << '\n';
        }else{
            qDebug() << "fail insert" << '\n';
        }
    }
}

void Form4::on_btn_pre_clicked()
{

    idx1 = (idx1 - 1 + new_words.size()) % new_words.size();
    ui->input  ->setText(new_words[idx1][1]);
    ui->input_2->setText(new_words[idx1][0]);
    ui->input_3->setText(new_words[idx1][2]);
}

void Form4::on_btn_next_clicked()
{
    QString a = ui->input->text(),b = ui->input_2->text(),c = ui->input_3->toPlainText();
    if(idx1 + 1 == new_words.size()){
        idx1 += 1;
        ui->input  ->setText("");
        ui->input_2->setText("");
        ui->input_3->setText(" ");
    }else if(idx1 == new_words.size()){
        new_words.push_back({b,a,c});
        idx1 += 1;
        ui->input  ->setText("");
        ui->input_2->setText("");
        ui->input_3->setText(" ");
    }
    else{
        idx1 += 1;
        ui->input  ->setText(new_words[idx1][1]);
        ui->input_2->setText(new_words[idx1][0]);
        ui->input_3->setText(new_words[idx1][2]);
    }
}
