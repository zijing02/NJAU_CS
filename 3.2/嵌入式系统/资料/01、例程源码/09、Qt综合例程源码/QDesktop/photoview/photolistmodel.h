/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   photoview
* @brief         photoListModel.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-16
*******************************************************************/
#ifndef PHOTOLISTMODEL_H
#define PHOTOLISTMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QUrl>

class photo {
public:
    explicit photo(QUrl path, QString title);
    QString gettitle() const;
    QUrl getpath() const;
    void settitle(QString title);

private:
    QString  m_title;
    QUrl m_path;
};

class photoListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit photoListModel(QObject *parent = 0);
    int currentIndex() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE int randomIndex();
    Q_INVOKABLE QString getcurrentTitle() const;
    Q_INVOKABLE QUrl getcurrentPath() const;
    Q_INVOKABLE void add(QString paths);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove(int first, int last);
    Q_INVOKABLE void setCurrentTitle(QString title);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_INVOKABLE void setCurrentIndex(const int & i);

    enum photoRole {
        pathRole = Qt::UserRole + 1,
        titleRole,
    };

signals:
    void currentIndexChanged();

public slots:

private:
    QHash<int, QByteArray> roleNames() const;
    void addPhoto(QUrl path, QString title);

    int m_currentIndex;
    QList<photo> phtoListData;

};

#endif // PHOTOLISTMODEL_H
