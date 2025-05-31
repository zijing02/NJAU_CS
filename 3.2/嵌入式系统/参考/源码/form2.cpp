#include "form2.h"
#include "ui_form2.h"

extern vector<QStringList> words;
extern int idx;
Form2::Form2(QWidget *parent): QWidget(parent), ui(new Ui::Form2)
{
    ui->setupUi(this);
}
Form2::~Form2(){delete ui;}

void Form2::on_btnA_clicked()
{
    QString cur = ui->input->text(),ans = words[idx][1];
    int x = words[idx][3].toInt(),y = words[idx][4].toInt();
    qDebug() << x << '\n';
    if(cur == ans){
        ui->input->setStyleSheet("background-color: rgb(85, 87, 83);\
                                 color: green;\
                                 qproperty-alignment:AlignHCenter;\
                                 font: 20pt Ubuntu;\
                                 border: 1px solid gray;\
                                 border-radius: 10px;\
                                 padding: 0 8px;\
                                 selection-background-color: darkgray;");
     if(x != 0){
         x -= 1;
         words[idx][3] = QString(x);
         qDebug() << words[idx][3] << '\n';
         QString queryString = "update data set error_time = :a where word = :b;";
         QSqlQuery query;
         query.prepare(queryString);
         query.bindValue(":a",x);
         query.bindValue(":b",cur);
         if(query.exec()){
             qDebug() << "successfully update" << '\n';
         }else{
             qDebug() << "fail update" << '\n';
         }
         if(x == 0 && y == 1){
             QString queryString = "update data set flag = :a where word = :b;";
             QSqlQuery query;
             query.prepare(queryString);
             query.bindValue(":a",0);
             query.bindValue(":b",cur);
             if(query.exec()){
                 qDebug() << "successfully update" << '\n';
             }else{
                 qDebug() << "fail update" << '\n';
             }
             words[idx][4] = QString(y - 1);
         }
     }else{
         qDebug() << "Nothing!!" << '\n';
     }

    }else{
        ui->input->setStyleSheet("background-color: rgb(85, 87, 83);\
                                 color: red;\
                                 qproperty-alignment:AlignHCenter;\
                                 font: 20pt Ubuntu;\
                                 border: 1px solid gray;\
                                 border-radius: 10px;\
                                 padding: 0 8px;\
                                 selection-background-color: darkgray;");

        ui->input->setText(ans);
        x += 1;
        qDebug() << x << '\n';
        words[idx][3] = QString(x);
        QString queryString = "update data set error_time = :a where word = :b;";
        QSqlQuery query;
        query.prepare(queryString);
        query.bindValue(":a",x);
        query.bindValue(":b",ans);
        if(query.exec()){
            qDebug() << "successfully update" << '\n';
        }else{
            qDebug() << "fail update" << '\n';
        }
        if(x >= 2 && y == 0){
            QString queryString = "update data set flag = :a where word = :b;";
            QSqlQuery query;
            query.prepare(queryString);
            query.bindValue(":a",1);
            query.bindValue(":b",ans);
            if(query.exec()){
                qDebug() << "successfully update" << '\n';
            }else{
                qDebug() << "fail update" << '\n';
            }
            words[idx][4] = QString(y + 1);
        }
        QFile file(wf1);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else{
            QTextStream stream(&file);
            stream.setCodec("utf-8");
            QDateTime dateTime(QDateTime::currentDateTime());
            QString tt = dateTime.toString("HH:mm:ss");
            stream << tt << ":" << "(" << ans << "," << words[idx][0] << ")" << "->" << "(" << cur << ")" << "\n";
            file.close();
        }
    }
    ui->remark->setText(words[idx][2]);
}

void Form2::on_btnB_clicked()
{
    ui->remark->setText("");
    idx = (idx + 1) % words.size();
    ui->label->setText(words[idx][0]);
    ui->input->setText("");
    ui->input->setStyleSheet("background-color: rgb(85, 87, 83);\
                             color: white;\
                             qproperty-alignment:AlignHCenter;\
                             font: 20pt Ubuntu;\
                             border: 1px solid gray;\
                             border-radius: 10px;\
                             padding: 0 8px;\
                             selection-background-color: darkgray;");
}
