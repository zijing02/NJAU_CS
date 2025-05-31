#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    total_time = 0;
    tomato_time = 15;
    flag = false;
    timer = new QTimer;
    ui->clock->setDigitCount(5);
    init_time();
    timer->setInterval(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
    timer->start();

    one = new Form1();
    ui->stackedWidget->insertWidget(0,one);

    two = new Form2();
    ui->stackedWidget->insertWidget(1,two);

    three = new Form3();
    ui->stackedWidget->insertWidget(2,three);

    four = new Form4();
    ui->stackedWidget->insertWidget(3,four);

    five = new Form5();
    ui->stackedWidget->insertWidget(4,five);

    six = new Form6();
    ui->stackedWidget->insertWidget(5,six);

    seven = new Form7();
    ui->stackedWidget->insertWidget(6,seven);

    
    DB = QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName("/home/gebeng/Desktop/Qt_learning/words.db");
    if(DB.open()) qDebug() << "Successfully open!" << '\n';
    else          qDebug() << "Error!";
}

Widget::~Widget(){delete ui;}

// init the time
void Widget::init_time()
{
    time.setHMS(0,0,0);
    ui->clock->display(time.toString("mm:ss"));
}
// update the time
void Widget::update_time()
{
    time = time.addSecs(1);
    ui->clock->display(time.toString("mm:ss"));
    oo1 = ui->lineEdit->text();
    ui->lineEdit_2->setText(oo2);
    if (time.minute() == 5 && flag){
        flag = false;
        init_time();
    }else if(time.minute() == tomato_time && !flag){
        flag = true;
        init_time();
        total_time += tomato_time;
        QFile file(wf1);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else{
            QTextStream stream(&file);
            stream.setCodec("utf-8");
            QDateTime dateTime(QDateTime::currentDateTime());
            QString tt = dateTime.toString("HH:mm:ss");
            stream << tt << ":" << "learning a tomato clock -> " << tomato_time << " minutes" << '\n';
            file.close();
        }
        QSound::play("/home/gebeng/Downloads/A.wav");
    }
}
// select mode

void Widget::init_word(){
    words.resize(1,{"n.苹果","apple","An apple a day keeps the doctor away!"});
    idx = 0;
    QFile file(rf);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) qDebug() << "Open Excel file failed!";
    QStringList list;
    list.clear();
    QTextStream in(&file);
    in.setCodec("utf-8");
    for(int i = 0; !in.atEnd(); i++)
    {
        QString fileLine = in.readLine();
        list = fileLine.split(",", QString::SkipEmptyParts);
        if(i > 0)
        {
            words.push_back(list);
        }

    }
    file.close();
}

void Widget::init_word1(){
    queryString = QString("select * from data where title=") + '"' + wordset + '"';
    QSqlQuery query(queryString);
    words.resize(1,{"n.苹果","apple","An apple a day keeps the doctor away!"});
    idx = 0;
    qDebug() << "starting query!" << '\n';
    while(query.next()){
        QString meaning = query.value("meaning").toString();
        QString word = query.value("word").toString();
        QString remark = query.value("remark").toString();
        QString cnts = query.value("error_time").toString();
        QString flags = query.value("flag").toString();
        words.push_back({meaning,word,remark,cnts,flags});
        qDebug() << cnts << ' ' << flags << '\n';
    }
}

void Widget::init_word2(){
    queryString = "select * from data where title = :a and flag = :b;";
    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(":a",wordset);
    query.bindValue(":b",1);
    if(query.exec()){
        qDebug() << "successfully init2" << '\n';
    }else{
        qDebug() << "fail init2" << '\n';
    }
    words.resize(1,{"n.苹果","apple","An apple a day keeps the doctor away!"});
    idx = 0;
    qDebug() << "starting query!" << '\n';
    while(query.next()){
        QString meaning = query.value("meaning").toString();
        QString word = query.value("word").toString();
        QString remark = query.value("remark").toString();
        QString cnts = query.value("error_time").toString();
        QString flags = query.value("flag").toString();
        words.push_back({meaning,word,remark,cnts,flags});
        qDebug() << cnts << ' ' << flags << '\n';
    }
}

// select_mod
void Widget::on_button1_clicked()
{
    if(oo1 == "0") init_word1();
    else         init_word2();
    ui->stackedWidget->setCurrentIndex(0);
}

// fill mode
void Widget::on_button2_clicked()
{
    if(oo1 == "0") init_word1();
    else         init_word2();
    ui->stackedWidget->setCurrentIndex(1);
}

// music
void Widget::on_button3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// write
void Widget::on_button4_clicked()
{
    init_word1();
    ui->stackedWidget->setCurrentIndex(5);
}

void Widget::on_cbox1_activated(const QString &arg1)
{
    if(arg1 == "番茄钟"){
        tomato_time = QInputDialog::getInt(this,"番茄钟","请输入时长");
    }else if(arg1 == "记生词"){
        write_word_name =  QInputDialog::getText(this,"文件选择","请输入生词本名称");

        ui->stackedWidget->setCurrentIndex(3);
    }else if(arg1 == "记笔记"){
        QString dir_str = "./remark";
        QDir dir;
        if (!dir.exists(dir_str)) dir.mkpath(dir_str);
        wf2 = "./remark/" + QInputDialog::getText(this,"文件选择","请输入文件名") + ".txt";
        QFile file(wf2);
        if(!file.open(QIODevice::Append)) qDebug() << "Not exists!";
        else file.close();
        read_flag = 0;
        ui->stackedWidget->setCurrentIndex(4);
    }
}

void Widget::on_cbox2_activated(const QString &arg1)
{
    if(arg1 == "看笔记"){
         rf2 = QFileDialog::getOpenFileName(this, "remark","./remark",tr("Files (*.txt)"));
         qDebug() << rf2 << '\n';
         read_flag = 1;
         ui->stackedWidget->setCurrentIndex(4);
    }else if(arg1 == "背单词"){
        wordset = QInputDialog::getText(this,"生词本选择","请输入生词本名称");
        oo2 = wordset;
    }else if(arg1 == "看日志"){
        rf1 = QFileDialog::getOpenFileName(this, "log","./log",tr("Files (*.txt)"));
        qDebug() << rf1 << '\n';
        read_flag = 2;
        ui->stackedWidget->setCurrentIndex(4);
    }else if(arg1 == "生词本"){
        init_word1();
        queryString = QString("select distinct title from data");
        QSqlQuery query(queryString);
        qDebug() << "starting query!" << '\n';
        wordsetlist.clear();
        while(query.next()){
            QString title = query.value("title").toString();
            qDebug() << title << '\n';
            wordsetlist.push_back(title);
        }
        ui->stackedWidget->setCurrentIndex(6);
    }
}
