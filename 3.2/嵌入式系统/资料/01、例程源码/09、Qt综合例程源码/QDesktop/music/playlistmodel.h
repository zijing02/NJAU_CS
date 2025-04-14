/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   music
* @brief         playListModel.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-05
*******************************************************************/
#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QAudioDecoder>
#include <QAudioDeviceInfo>
#include <QDebug>
#include <QUrl>

#include "stdlib.h"
#include "stdio.h"
#include <QDebug>
#include <QImage>
#include <QMap>
#include <QTextCodec>
#include <QString>
#include <QFile>
#include <QUrl>

using namespace std;
#define MAX_PATH 1024
typedef struct ID3V2FrameH
{
    char frameID[4];        //存放标签格式，是否为id3v2
    unsigned char size[4];  //存放标签数据的大小
    char flags[2];
}ID3V2FH;

typedef struct MP3INFO
{
    QString Url;             //存放这首歌的详细地址
    QString Name;            //歌名  TIT2
    QString Album;           //专辑  TALB
    QString Singer;          //歌手  TPE1
    QString Picture_url;     //歌曲图片存放路径
    QString Picture_type;    //图片类型 jpg,png
    int     number;          //歌曲编号
    int     beginNum;        //图片起始位置
    int     lenth;           //图片数据长度
    bool     pic_flag;       //是否有图片


}MP3INFO;
typedef struct frameIDStruct
{
    int beginNum;
    int endNum;
    QString FrameId;

}frameIDStruct;


class song {
public:
    explicit song(QUrl path, QString title, QString author);
    QString getauthor() const;
    QString gettitle() const;
    QUrl getpath() const;
    void setauthor(QString author);
    void settitle(QString title);

private:
    QString m_author, m_title;
    QUrl m_path;
};

class playListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit playListModel(QObject *parent = 0);
    int currentIndex() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE int randomIndex();
    Q_INVOKABLE QString getcurrentTitle() const;
    Q_INVOKABLE QString getcurrentAuthor() const;
    Q_INVOKABLE QUrl getcurrentPath() const;
    Q_INVOKABLE void add(/*QList<QUrl> paths*/QString paths);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove(int first, int last);
    Q_INVOKABLE void setCurrentTitle(QString title);
    Q_INVOKABLE void setCurrentAuthor(QString author);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    void setCurrentIndex(const int & i);

    // mp3信息
    FILE *fp;
    QString m_url;
    unsigned char Header[3];
    unsigned char FrameId[4];         //存放帧标识
    unsigned char Header_size[4];
    unsigned int mp3_TagSize;
    unsigned char frameSize[4];       //存放该帧内容的大小
    unsigned int framecount;          //计算出帧内容的大小
    void GetMp3IDV2(const wchar_t *url);
    MP3INFO GetAllInfo(const wchar_t *url, int songNumber, int imageId);
    void GetPicture(MP3INFO *mp3info);
    void GetFrameId();
    QString GetInfo(QString fId);
    QMap<QString,frameIDStruct> m_IDmap;

    FILE* wfopen(const wchar_t* filename, const wchar_t* mode);

    enum songRole {
        pathRole = Qt::UserRole + 1,
        titleRole,
        authorRole,
    };

signals:
    void currentIndexChanged();

public slots:

private:
    QHash<int, QByteArray> roleNames() const;
    void addSong(QUrl path, QString title, QString author/*QString path, QString title, QString author*/);
    wchar_t *to_wchar(const char *pSrc);

    int m_currentIndex;
    QList<song> playListData;
    QString artistDir;

};

#endif // PLAYLISTMODEL_H
