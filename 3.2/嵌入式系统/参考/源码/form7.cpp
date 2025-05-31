#include "form7.h"
#include "ui_form7.h"
Form7::Form7(QWidget *parent): QWidget(parent), ui(new Ui::Form7)
{
    ui->setupUi(this);
}
Form7::~Form7(){delete ui;}

void Form7::on_pushButton_clicked()
{
    int n = words.size();
    ui->tabel->setRowCount(n);
    ui->tabel->setColumnCount(3);
    for(int col = 0;col < ui->tabel->colorCount();++col){
        if     (col == 0) ui->tabel->setColumnWidth(col,150);
        else if(col == 1) ui->tabel->setColumnWidth(col,150);
        else              ui->tabel->setColumnWidth(col,200);
    }
    ui->tabel->setHorizontalHeaderLabels({"meaning", "word", "remark"});
    for (int row = 0; row < ui->tabel->rowCount(); ++row) {
        for (int col = 0; col < ui->tabel->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString(words[row][col]));
            ui->tabel->setItem(row, col, item);
        }
    }
}

void Form7::on_pushButton_2_clicked()
{
    int n = wordsetlist.size();
    ui->tabel->setRowCount(n);
    ui->tabel->setColumnCount(1);
    ui->tabel->setColumnWidth(0,300);

    ui->tabel->setHorizontalHeaderLabels({"wordset"});
    for (int row = 0; row < ui->tabel->rowCount(); ++row) {
        QTableWidgetItem *item = new QTableWidgetItem(QString(wordsetlist[row]));
        ui->tabel->setItem(row, 0, item);
    }
}

void Form7::on_tabel_doubleClicked(const QModelIndex &index)
{
    int row = index.row(),col = index.column();
    if(col == 0){
        qDebug() << row << '\n';
        wordset = ui->tabel->item(row,0)->text();
        QString queryString = QString("select * from data where title=") + '"' + wordset + '"';
        oo2 = wordset;
        QSqlQuery query(queryString);
        words.resize(1,{"n.苹果","apple","An apple a day keeps the doctor away!"});
        idx = 0;
        qDebug() << "starting query!" << '\n';
        while(query.next()){
            QString meaning = query.value("meaning").toString();
            QString word = query.value("word").toString();
            QString remark = query.value("remark").toString();
            words.push_back({meaning,word,remark});
        }
    }
}
