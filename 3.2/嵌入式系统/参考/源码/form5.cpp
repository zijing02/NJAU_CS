#include "form5.h"
#include "ui_form5.h"


Form5::Form5(QWidget *parent): QWidget(parent), ui(new Ui::Form5)
{
    ui->setupUi(this);
}
Form5::~Form5(){delete ui;}

void Form5::on_btn_save_clicked()
{
    QFile file;
    if(read_flag == 1){
        file.setFileName(rf2);
    }else if(read_flag == 0){
        file.setFileName(wf2);
    }else if(read_flag == 2){
        file.setFileName(rf1);
    }else{
        file.setFileName(rf);
    }
    if(!file.open(QIODevice::ReadWrite | QIODevice::WriteOnly | QIODevice::ReadOnly))qDebug() << ("打开文件失败");
    QTextStream stream(&file);
    stream.setCodec("utf-8");
    stream << ui->textEdit->toPlainText() << '\n';
    file.close();
}

void Form5::on_btn_show_clicked()
{
    if(read_flag == 1){
        QFile file1(rf2);
        if(!file1.open(QIODevice::ReadOnly | QIODevice::Text)) qDebug() << ("打开文件失败");
        QTextStream stream(&file1);
        QString line;
        QString s;
        while(!stream.atEnd())
        {
            line = stream.readLine() + "\n";
            s += line;
        }
        ui->textEdit->setText(s);
        file1.close();
    }else if(read_flag == 2){
        QFile file1(rf1);
        if(!file1.open(QIODevice::ReadOnly | QIODevice::Text)) qDebug() << ("打开文件失败");
        QTextStream stream(&file1);
        QString line;
        QString s;
        while(!stream.atEnd())
        {
            line = stream.readLine() + "\n";
            s += line;
        }
        ui->textEdit->setText(s);
        file1.close();
    }else if(read_flag == 3){
        QFile file1(rf);
        if(!file1.open(QIODevice::ReadOnly | QIODevice::Text)) qDebug() << ("打开文件失败");
        QTextStream stream(&file1);
        QString line;
        QString s;
        while(!stream.atEnd())
        {
            line = stream.readLine() + "\n";
            s += line;
        }
        ui->textEdit->setText(s);
        file1.close();
    }else{
        ui->textEdit->setText("");
    }
}
