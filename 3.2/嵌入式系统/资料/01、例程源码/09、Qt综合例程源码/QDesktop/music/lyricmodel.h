/*本程序使用GPLv2协议发布*/
#ifndef LYRICMODEL_H
#define LYRICMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

class lyricLine {
public:
    explicit lyricLine();
    explicit lyricLine(int m,QString t);
    int getmilliseconds() const;
    QString gettext() const;

private:
    int milliseconds;
    QString text;
};

class lyricModel : public QAbstractListModel {
    Q_OBJECT
public:
    explicit lyricModel(QObject *parent = 0);
    int currentIndex() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE bool setPathofSong(QString path, QString appDir);
    Q_INVOKABLE int findIndex(int position);
    Q_INVOKABLE int getIndex(int position);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setcurrentIndex NOTIFY currentIndexChanged)
    void addSingleLine(lyricLine l);
    void removeTopLine();
    void setcurrentIndex(const int & i);

    enum lyricRoles {
        timeRole = Qt::UserRole + 1,
        textRole,
    };

signals:
    void currentIndexChanged();

public slots:

private:
    QHash<int, QByteArray> roleNames() const;
    void clearData();

    int m_currentIndex;
    QList<lyricLine> lyricData, tempData;
};

#endif // LYRICMODEL_H
