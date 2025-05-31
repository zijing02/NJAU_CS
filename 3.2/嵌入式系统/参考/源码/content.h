#ifndef CONTENT_H
#define CONTENT_H
#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QWidget>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QDebug>
#include <vector>
#include <cstring>
#include <QStringList>
#include <QTest>
#include <cstdio>
#include <ctime>
#include <bits/stdc++.h>
#include <QSound>
#include <QTimer>
#include <QTime>
#include <QInputDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMenu>
#include <QTableWidget>
using namespace  std;

extern Q_DECL_EXPORT vector<QStringList> words,new_words;
extern Q_DECL_EXPORT vector<int> cnt1,cnt2;
extern Q_DECL_EXPORT int idx,idx1;
extern Q_DECL_EXPORT QString rf,wf,rf1,wf1,date,rf2,wf2;

extern Q_DECL_EXPORT int read_flag;

extern Q_DECL_EXPORT QString wordset;

extern Q_DECL_EXPORT vector<QString> wordsetlist;

extern Q_DECL_EXPORT QString write_word_name,cur_word_name;

extern Q_DECL_EXPORT bool pause_flag;

extern Q_DECL_EXPORT QString oo1,oo2;
#endif // CONTENT_H
