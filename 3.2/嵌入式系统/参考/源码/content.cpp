#include <content.h>


vector<int> cnt1 = {1},cnt2 = {1};

int idx = 0;
vector<QStringList> words(1,{"n.苹果","apple","An apple a day keeps the doctor away!"});

int idx1 = 0;
vector<QStringList> new_words;

QString wordset = "default";

QDateTime dateTime(QDateTime::currentDateTime());
QString date = dateTime.toString("yyyy-MM-dd");

QString rf  = "./words/1.csv";
QString wf  = "./words/2.csv";

QString wf1 = "./log/" + date + ".txt";
QString rf1 = "./log/" + date + ".txt";

QString wf2 = "./remark/" + date + ".txt";
QString rf2 = "./remark/" + date + ".txt";

int read_flag = 0;

vector<QString> wordsetlist;

QString write_word_name,cur_word_name;

bool pause_flag = false;

QString oo1 = "0",oo2 = "default";
