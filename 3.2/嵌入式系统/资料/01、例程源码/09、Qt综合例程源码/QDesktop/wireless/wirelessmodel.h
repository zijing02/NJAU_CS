/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   wireless
* @brief         wirelessListModel.h
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-01
*******************************************************************/
#ifndef WIRELESSMODEL_H
#define WIRELESSMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QUrl>
#include <QProcess>
#include <QRegularExpression>

class wireless {
public:
    explicit wireless(QString title,  int level);
    int getlevel() const;
    QString gettitle() const;
    QUrl getpath() const;
    void setlevel(QString level);
    void settitle(QString title);

private:
    QString  m_title;
    QUrl m_path;
    int m_level;
};

class wirelessListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit wirelessListModel(QObject *parent = 0);
    int currentIndex() const;
    int currentSate() const;
    bool readyFlag() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    Q_INVOKABLE int randomIndex();
    Q_INVOKABLE QString getcurrentTitle() const;
    Q_INVOKABLE QUrl getcurrentPath() const;
    Q_INVOKABLE void wirelessInit();
    Q_INVOKABLE void wirelessClose();
    Q_INVOKABLE void getMyWifiSSIDPSK(QString ssid, QString psk);
    Q_INVOKABLE void passwordLessConnection(QString ssid);
    Q_INVOKABLE void move(int from, int to);
    Q_INVOKABLE void remove(int first, int last);
    Q_INVOKABLE void setCurrentTitle(QString title);
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int state READ currentSate NOTIFY returnMainWinWifiResult)
    Q_PROPERTY(bool readyFlag  READ readyFlag  NOTIFY wirelessReadyChanged)
    void setCurrentIndex(const int & i);

    enum wirelessRole {
        pathRole = Qt::UserRole + 1,
        titleRole,
        levelRole,
    };

signals:
    void currentIndexChanged();
    void returnMainWinWifiResult();
    void wirelessReadyChanged();

public slots:
    void standardOutput_WifiConnectSta();
    void standardError_WifiConnectSta();

private:
    QHash<int, QByteArray> roleNames() const;
    void addwireless(QString title, int level);

    int m_currentIndex;
    int m_sate;
    bool m_readyFlag;
    QList<wireless> playListData;

    QProcess *ConnetWifi = nullptr;

    // Acquisition of signal strength
    QString get_Signallevel(QString data) {
        QString pattern1 = "Signal level=(\\S*)/100";
        QRegularExpression regularExpression(pattern1);
        QRegularExpressionMatch  match = regularExpression.match(data, 0);
        if(match.hasMatch())
            return match.captured(1);

        return nullptr;
    }

    // Get SSID name
    QString get_ssid(QString data) {
        QString pattern1 = "ESSID:\\s*\"(\\S*)\"";
        QRegularExpression regularExpression(pattern1);
        QRegularExpressionMatch  match = regularExpression.match(data, 0);
        if(match.hasMatch())
            return match.captured(1);
        return nullptr;
    }
    QProcess * Mprocess;

};

#endif // WIRELESSMODEL_H
