/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   media
* @brief         mediaListModel.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-06-10
*******************************************************************/
#ifndef MEDIALISTMODEL_H
#define MEDIALISTMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QUrl>

class media {
public:
    explicit media(QUrl path, QString title);
    QString getauthor() const;
    QString gettitle() const;
    QUrl getpath() const;
    void setauthor(QString author);
    void settitle(QString title);

private:
    QString  m_title;
    QUrl m_path;
};

class mediaListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit mediaListModel(QObject *parent = 0);
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
    void setCurrentIndex(const int & i);

    enum mediaRole {
        pathRole = Qt::UserRole + 1,
        titleRole,
    };

signals:
    void currentIndexChanged();

public slots:

private:
    QHash<int, QByteArray> roleNames() const;
    void addMedia(QUrl path, QString title);

    int m_currentIndex;
    QList<media> playListData;

};

#endif // MEDIALISTMODEL_H
