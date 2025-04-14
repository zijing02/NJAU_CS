#include "lyricmodel.h"
#include "QDebug"
lyricLine::lyricLine() {
    milliseconds = INT_MAX;
    text = " ";
}

lyricLine::lyricLine(int m, QString t) {
    milliseconds = m;
    text = t;
}

int lyricLine::getmilliseconds() const {
    return milliseconds;
}

QString lyricLine::gettext() const {
    return text;
}

lyricModel::lyricModel(QObject *parent) : QAbstractListModel(parent) {
    m_currentIndex = 0;
}

int lyricModel::currentIndex() const {
    return m_currentIndex;
}

int lyricModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return lyricData.count();
}

QVariant lyricModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= lyricData.count())
        return QVariant();
    const lyricLine &ll = lyricData.at(index.row());
    switch (role) {
    case timeRole:
        return ll.getmilliseconds();
    case textRole:
        return ll.gettext();
    default:
        return QVariant();
    }
}

bool lyricModel::setPathofSong(QString path,QString appDir) {
    clearData();
    setcurrentIndex(0);
    QFileInfo fi(path);
    //qDebug()<<fi.path()<<endl;
    path = appDir + "/src/lrc/" + fi.completeBaseName() + ".lrc";
    fi.setFile(path);
    if (fi.exists() && fi.isReadable()) {
        QFile flyric(path);
        if (! flyric.open(QFile::ReadOnly | QFile::Text)) {
            return false;
        }
        bool ok;
        QString textLine;
        QTextStream sin(& flyric);
        int milliseconds;
        while (! sin.atEnd()) {
            textLine = sin.readLine();
            textLine.mid(1,1).toInt(& ok, 10);
            if (! ok) {
                continue;
            }
            milliseconds = (textLine.mid(1,2).toInt(& ok) * 60 +
                            textLine.mid(4,2).toInt(& ok)) * 1000 +
                    textLine.mid(7,2).toInt(& ok) * 10;
            textLine = textLine.mid(10);
            addSingleLine(lyricLine(milliseconds, textLine));
        }
        flyric.close();
        return true;
    } else {
        qDebug()<<"src/lrc Dir is not exist"<<endl;
        addSingleLine(lyricLine(0, tr("未找到歌词")));
        return false;
    }
}

int lyricModel::findIndex(int position) {
    // bug fix
    if (position == 0) {
        setcurrentIndex(0);
        return 0;
    }
    // bug fix end
    int countless = lyricData.count() - 1, mid = lyricData.count() / 2, diff = mid / 2;
    while (1) {
        if (lyricData.at(mid).getmilliseconds() <= position) {
            if (mid < countless && lyricData.at(mid + 1).getmilliseconds() > position) {
                break;
            } else {
                mid += diff;
            }
        } else {
            mid -= diff;
        }
        diff /= 2;
        if (diff == 0) {
            break;
        }
    }
    setcurrentIndex(mid);
    return mid;
}

int lyricModel::getIndex(int position) {
    if (m_currentIndex + 1 < lyricData.count() &&
            lyricData.at(m_currentIndex + 1).getmilliseconds() <= position) {
        m_currentIndex ++;
        emit currentIndexChanged();
    }
    return m_currentIndex;
}

void lyricModel::addSingleLine(lyricLine l) {
    beginInsertRows(QModelIndex(), lyricData.count(), lyricData.count());
    lyricData << l;
    endInsertRows();
}

void lyricModel::removeTopLine() {
    beginRemoveRows(QModelIndex(), 0, 0);
    lyricData.removeFirst();
    endRemoveRows();
}

void lyricModel::setcurrentIndex(const int & i) {
    if ((i == 0 || (i < lyricData.count())) && m_currentIndex != i) {
        m_currentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> lyricModel::roleNames() const {
    QHash<int, QByteArray> r;
    r[timeRole] = "time";
    r[textRole] = "textLine";
    return r;
}

void lyricModel::clearData() {
    beginResetModel();
    lyricData.clear();
    endResetModel();
}
