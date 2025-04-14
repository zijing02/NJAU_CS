/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   photoview
* @brief         photoListModel.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-18
*******************************************************************/
#include "photolistmodel.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
photo::photo(QUrl path, QString title) {
    m_path = path;
    m_title = title;
}

QUrl photo::getpath() const {
    return m_path;
}

QString photo::gettitle() const {
    return m_title;
}

void photo::settitle(QString title) {
    m_title = title;
}

photoListModel::photoListModel(QObject *parent) : QAbstractListModel(parent) {
    m_currentIndex = -1;
}

int photoListModel::currentIndex() const {
    return m_currentIndex;
}

int photoListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return phtoListData.count();
}

QVariant photoListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= phtoListData.count())
        return QVariant();
    const photo &s = phtoListData.at(index.row());
    switch (role) {
    case pathRole:
        return s.getpath();
    case titleRole:
        return s.gettitle();
    default:
        return QVariant();
    }
}

int photoListModel::randomIndex() {
    int tmp;
    srand(time(NULL));
    do {
        tmp = qrand() % phtoListData.count();
    } while (tmp == m_currentIndex);
    setCurrentIndex(tmp);
    return tmp;
}

QString photoListModel::getcurrentTitle() const {
    return phtoListData.at(m_currentIndex).gettitle();
}

QUrl photoListModel::getcurrentPath() const {
    return phtoListData.at(m_currentIndex).getpath();
}

void photoListModel::add( QString paths) {
    int count, index;
    QString  title;
    QDir dir(paths);
    if(!dir.exists()){
        qDebug()<<"src/images Dir not exist"<<endl;
        return ;
    }
    QStringList filter;
    filter<<"*.jpg"<<"*.png";
    QDirIterator it(paths, filter, QDir::Files|QDir::NoSymLinks);
    QList<QUrl> pathList;
    while (it.hasNext()){
        it.next();
        QFileInfo fileif = it.fileInfo();
        QString photosPath = QString::fromUtf8((QString("file:///" + fileif.filePath()).toUtf8().data()));

        //qDebug()<<photosPath<<endl;
        pathList.append(photosPath);
    }
    count = pathList.count();
    for (index = 0; index < count; index ++) {

           title = pathList.at(index).fileName(QUrl::FullyDecoded).remove(-4, 4);
        addPhoto(pathList.at(index), title);
    }
    if (m_currentIndex < 0 && phtoListData.count()) {
        setCurrentIndex(0);
    }
}

void photoListModel::move(int from, int to) {
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
    phtoListData.move(from, to);
    endMoveRows();
}

void photoListModel::remove(int first, int last) {
    if ((first < 0) && (first >= phtoListData.count()))
        return;
    if ((last < 0) && (last >= phtoListData.count()))
        return;
    if (first > last) {
        first ^= last;
        last ^= first;
        first ^= last;
    }
    beginRemoveRows(QModelIndex(), first, last);
    while (first <= last) {
        phtoListData.removeAt(first);
        last --;
    }
    endRemoveRows();
    if (m_currentIndex >= phtoListData.count()) {
        setCurrentIndex(phtoListData.count() - 1);
    }
}

void photoListModel::setCurrentTitle(QString title) {
    photo s = phtoListData.at(m_currentIndex);
    s.settitle(title);
    phtoListData.replace(m_currentIndex, s);
}

void photoListModel::setCurrentIndex(const int & i) {
    if (i >= phtoListData.count() && m_currentIndex != 0) {
        m_currentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < phtoListData.count()) && (m_currentIndex != i)) {
        m_currentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> photoListModel::roleNames() const {
    QHash<int, QByteArray> role;
    role[pathRole] = "path";
    role[titleRole] = "title";
    return role;
}

void photoListModel::addPhoto(QUrl path, QString title) {
    beginInsertRows(QModelIndex(), phtoListData.count(), phtoListData.count());
    phtoListData.append(photo(path, title));
    endInsertRows();
}
