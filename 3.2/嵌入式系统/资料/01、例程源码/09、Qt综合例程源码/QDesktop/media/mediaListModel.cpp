/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   media
* @brief         mediaListModel.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-10
*******************************************************************/
#include "mediaListModel.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
media::media(QUrl path, QString title) {
    m_path = path;
    m_title = title;
}

QUrl media::getpath() const {
    return m_path;
}

QString media::gettitle() const {
    return m_title;
}

void media::settitle(QString title) {
    m_title = title;
}

mediaListModel::mediaListModel(QObject *parent) : QAbstractListModel(parent) {
    m_currentIndex = -1;
}

int mediaListModel::currentIndex() const {
    return m_currentIndex;
}

int mediaListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return playListData.count();
}

QVariant mediaListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= playListData.count())
        return QVariant();
    const media &s = playListData.at(index.row());
    switch (role) {
    case pathRole:
        return s.getpath();
    case titleRole:
        return s.gettitle();
    default:
        return QVariant();
    }
}

int mediaListModel::randomIndex() {
    int tmp;
    srand(time(NULL));
    do {
        tmp = qrand() % playListData.count();
    } while (tmp == m_currentIndex);
    setCurrentIndex(tmp);
    return tmp;
}

QString mediaListModel::getcurrentTitle() const {
    return playListData.at(m_currentIndex).gettitle();
}

QUrl mediaListModel::getcurrentPath() const {
    return playListData.at(m_currentIndex).getpath();
}

void mediaListModel::add(/*QList<QUrl> paths*/ QString paths) {

    int count, index;
    QString  title;
    QDir dir(paths);

    if(!dir.exists())
    {
        qDebug()<<"Dir not exist"<<endl;
        return ;
    }

    QStringList filter;
    filter<<"*.avi"<<"*.mp4"<<"*.mkv"<<"*.rm";

    /*QDirIterator: : Subdirectories the fourth parameter can set up a file
     * the iterator marks accords with a condition to access a folder in the file,
     * but will be slowly
     */
    QDirIterator it(paths, filter, QDir::Files|QDir::NoSymLinks);
    QList<QUrl> pathList;

    while (it.hasNext()){
        it.next();
        QFileInfo fileif = it.fileInfo();
#ifdef WIN32
        QUrl mediasPath = QString::fromUtf8((fileif.filePath().toUtf8().data()));
#else
        QUrl mediasPath = QString::fromUtf8((QString("file:///" + fileif.filePath()).toUtf8().data()));
#endif
        //addMedia(MediasPath, title, author);
        pathList.append(mediasPath);
    }

    count = pathList.count();
    for (index = 0; index < count; index ++) {

           title = pathList.at(index).fileName(QUrl::FullyDecoded).remove(-4, 4);
        addMedia(pathList.at(index), title);
    }
    if (m_currentIndex < 0 && playListData.count()) {
        setCurrentIndex(0);
    }
}

void mediaListModel::move(int from, int to) {
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
    playListData.move(from, to);
    endMoveRows();
}

void mediaListModel::remove(int first, int last) {
    if ((first < 0) && (first >= playListData.count()))
        return;
    if ((last < 0) && (last >= playListData.count()))
        return;
    if (first > last) {
        first ^= last;
        last ^= first;
        first ^= last;
    }
    beginRemoveRows(QModelIndex(), first, last);
    while (first <= last) {
        playListData.removeAt(first);
        last --;
    }
    endRemoveRows();
    if (m_currentIndex >= playListData.count()) {
        setCurrentIndex(playListData.count() - 1);
    }
}

void mediaListModel::setCurrentTitle(QString title) {
    media s = playListData.at(m_currentIndex);
    s.settitle(title);
    playListData.replace(m_currentIndex, s);
}

void mediaListModel::setCurrentIndex(const int & i) {
    if (i >= playListData.count() && m_currentIndex != 0) {
        m_currentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < playListData.count()) && (m_currentIndex != i)) {
        m_currentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> mediaListModel::roleNames() const {
    QHash<int, QByteArray> role;
    role[pathRole] = "path";
    role[titleRole] = "title";
    return role;
}

void mediaListModel::addMedia(QUrl path, QString title) {
    beginInsertRows(QModelIndex(), playListData.count(), playListData.count());
    playListData.append(media(path, title));
    endInsertRows();
}

