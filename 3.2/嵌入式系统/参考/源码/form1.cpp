#include "form1.h"
#include "ui_form1.h"

extern vector<QStringList> words;
extern int idx;

Form1::Form1(QWidget *parent): QWidget(parent), ui(new Ui::Form1)
{
    ui->setupUi(this);

}
Form1::~Form1(){delete ui;}


void Form1::Rand(){
    vector<int> a(1,idx);
    vector<int> b(words.size());
    iota(b.begin(),b.end(),0);
    b.erase(b.begin() + idx);
    while(b.size() < 3){
        b.push_back(idx);
    }
    random_shuffle(b.begin(),b.end());
    for(int i = 0;i < 3;++i) a.push_back(b[i]);
    random_shuffle(a.begin(),a.end());
    ui->btA->setStyleSheet("color: rgb(238, 238, 236);text-align: left;");
    ui->btB->setStyleSheet("color: rgb(238, 238, 236);text-align: left;");
    ui->btC->setStyleSheet("color: rgb(238, 238, 236);text-align: left;");
    ui->btD->setStyleSheet("color: rgb(238, 238, 236);text-align: left;");
    ui->btA->setText(words[a[0]][0]);
    ui->btB->setText(words[a[1]][0]);
    ui->btC->setText(words[a[2]][0]);
    ui->btD->setText(words[a[3]][0]);
}
void Form1::on_btA_clicked()
{
    QString answer = words[idx][0],cur = ui->btA->text(),ans = words[idx][1];
    int x = words[idx][3].toInt(),y = words[idx][4].toInt();
    qDebug() << answer << ' ' << cur << '\n';
    if(answer == cur){
        idx = (idx + 1) % words.size();
        Rand();
        ui->label->setText(words[idx][1]);
        ui->remark->setText("");
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
        }
    }else{
        QFile file(wf1);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else{
            QTextStream stream(&file);
            stream.setCodec("utf-8");
            QDateTime dateTime(QDateTime::currentDateTime());
            QString tt = dateTime.toString("HH:mm:ss");
            stream << tt << ":" << "(" << words[idx][1] << "," << answer << ")" << "->" << "(" << cur << ")" << "\n";
            file.close();
        }
        QString st = "";
        for(int i = 0;i < words.size();++i){
            if(words[i][0] == cur){
                st = words[i][1];
            }
        }
        ui->btA->setStyleSheet("color:red");
        ui->btA->setText(cur + "(" +  st + ")");
        ui->remark->setText(words[idx][2]);
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
    }
}

void Form1::on_btB_clicked()
{
    QString answer = words[idx][0],cur = ui->btB->text(),ans = words[idx][1];
    int x = words[idx][3].toInt(),y = words[idx][4].toInt();
    if(answer == cur){
        idx = (idx + 1) % words.size();
        Rand();
        ui->label->setText(words[idx][1]);
        ui->remark->setText("");
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
        }
    }else{
        QFile file(wf1);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else{
            QTextStream stream(&file);
            stream.setCodec("utf-8");
            QDateTime dateTime(QDateTime::currentDateTime());
            QString tt = dateTime.toString("HH:mm:ss");
            stream << tt << ":" << "(" << words[idx][1] << "," << answer << ")" << "->" << "(" << cur << ")" << "\n";
            file.close();
        }
        QString st = "";
        for(int i = 0;i < words.size();++i){
            if(words[i][0] == cur){
                st = words[i][1];
            }
        }
        ui->btB->setStyleSheet("color:red");
        ui->btB->setText(cur + "(" +  st + ")");
        ui->remark->setText(words[idx][2]);
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
    }
}



void Form1::on_btC_clicked()
{
    QString answer = words[idx][0],cur = ui->btC->text(),ans = words[idx][1];
    int x = words[idx][3].toInt(),y = words[idx][4].toInt();
    if(answer == cur){
        idx = (idx + 1) % words.size();
        Rand();
        update();
        ui->label->setText(words[idx][1]);
        ui->remark->setText("");
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
        }
    }else{
        QFile file(wf1);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else{
            QTextStream stream(&file);
            stream.setCodec("utf-8");
            QDateTime dateTime(QDateTime::currentDateTime());
            QString tt = dateTime.toString("HH:mm:ss");
            stream << tt << ":" << "(" << words[idx][1] << "," << answer << ")" << "->" << "(" << cur << ")" << "\n";
            file.close();
        }
        QString st = "";
        for(int i = 0;i < words.size();++i){
            if(words[i][0] == cur){
                st = words[i][1];
            }
        }
        ui->btC->setStyleSheet("color:red");
        ui->btC->setText(cur + "(" +  st + ")");
        ui->remark->setText(words[idx][2]);
        ui->remark->setText(words[idx][2]);
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
    }
}


void Form1::on_btD_clicked()
{
    QString answer = words[idx][0],cur = ui->btD->text(),ans = words[idx][1];
    int x = words[idx][3].toInt(),y = words[idx][4].toInt();
    if(answer == cur){
        idx = (idx + 1) % words.size();
        Rand();
        ui->label->setText(words[idx][1]);
        ui->remark->setText("");
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
        }
    }else{
        QFile file(wf1);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else{
            QTextStream stream(&file);
            stream.setCodec("utf-8");
            QDateTime dateTime(QDateTime::currentDateTime());
            QString tt = dateTime.toString("HH:mm:ss");
            stream << tt << ":" << "(" << words[idx][1] << "," << answer << ")" << "->" << "(" << cur << ")" << "\n";
            file.close();
        }
        QString st = "";
        for(int i = 0;i < words.size();++i){
            if(words[i][0] == cur){
                st = words[i][1];
            }
        }
        ui->btD->setStyleSheet("color:red");
        ui->btD->setText(cur + "(" +  st + ")");
        ui->remark->setText(words[idx][2]);
        ui->remark->setText(words[idx][2]);
        ui->remark->setText(words[idx][2]);
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
    }
}

void Form1::on_btn_next_clicked()
{
    idx = (idx + 1) % words.size();
    Rand();
    ui->label->setText(words[idx][1]);
}
