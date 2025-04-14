/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   wireless
* @brief         wirelessmodel.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-01
*******************************************************************/
#include "wirelessmodel.h"
#include <QDir>
#include <QDebug>
#include <QDirIterator>
wireless::wireless(QString title, int level) {
    m_level = level;
    m_title = title;
}

QUrl wireless::getpath() const {
    return m_path;
}

int wireless::getlevel() const {
    return m_level;
}

QString wireless::gettitle() const {
    return m_title;
}

void wireless::settitle(QString title) {
    m_title = title;
}

wirelessListModel::wirelessListModel(QObject *parent) : QAbstractListModel(parent) {
    m_currentIndex = -1;
    m_readyFlag = false;
#if __arm__
    system("killall wpa_supplicant");
#endif
}

int wirelessListModel::currentIndex() const {
    return m_currentIndex;
}

int wirelessListModel::currentSate() const {
    return m_sate;
}

bool wirelessListModel::readyFlag() const {
    return  m_readyFlag;
}

int wirelessListModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return playListData.count();
}

QVariant wirelessListModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= playListData.count())
        return QVariant();
    const wireless &s = playListData.at(index.row());
    switch (role) {
    case pathRole:
        return s.getpath();
    case titleRole:
        return s.gettitle();
    case levelRole:
        return s.getlevel();
    default:
        return QVariant();
    }
}

int wirelessListModel::randomIndex() {
    int tmp;
    srand(time(NULL));
    do {
        tmp = qrand() % playListData.count();
    } while (tmp == m_currentIndex);
    setCurrentIndex(tmp);
    return tmp;
}

QString wirelessListModel::getcurrentTitle() const {
    return playListData.at(m_currentIndex).gettitle();
}

QUrl wirelessListModel::getcurrentPath() const {
    return playListData.at(m_currentIndex).getpath();
}

void wirelessListModel::wirelessInit() {
    m_readyFlag = false;
#ifdef __arm__
    int count;
    QList<QString> pathList;
    count = pathList.count();
    if(count > 0){

    }else{

    }
    system("ifconfig wlan0 up");
    system("iwlist wlan0 scan > ./wifiinfo");
    QFile wifiinfo("./wifiinfo");
    if(!wifiinfo.open(QFile::ReadOnly)){
        return;
    }

    QString wifiinfo_buf = QString::fromUtf8(wifiinfo.readAll());
    wifiinfo.close();

    QString pattern_1 = "Address:\\s(\\S*)\\s+ESSID:\\s*(\\S*)";
    QString pattern_2 = "Quality=(\\S*)\\s*Signal level=(\\S*)";

    QRegularExpression reguE_1(pattern_1);
    QRegularExpression reguE_2(pattern_2);
    QRegularExpressionMatch match_1;
    QRegularExpressionMatch match_2;
    int index_1 = 0, index_2 = 0, dataLen = 1;

    //QStringList iconNameList ;
    //iconNameList<< "wifi_0"<< "wifi_1" <<"wifi_2" <<"wifi_3"<< "wifi_4"<< "wifi_5";


    do{
        match_1 = reguE_1.match(wifiinfo_buf, index_1);
        match_2 = reguE_2.match(wifiinfo_buf, index_2);
        if(match_1.hasMatch()&&match_2.hasMatch()){
            index_1 = match_1.capturedEnd();
            index_2 = match_2.capturedEnd();

            QString mySSD = get_ssid(match_1.captured(0));
            if(mySSD.size()){
                int SignalValue = get_Signallevel(match_2.captured(0)).toInt();
                int level =0;
                if (SignalValue > 60)
                     level = 5;
                else if (SignalValue <= 59 && SignalValue >= 50)
                     level = 4;
                else if (SignalValue <= 49 && SignalValue >= 40)
                     level = 3;
                else if (SignalValue <= 39 && SignalValue >= 30)
                     level = 2;
                else if (SignalValue <= 29  && SignalValue >= 10)
                     level = 1;
                else if (SignalValue <= 9)
                     level = 0;

                addwireless(mySSD, level);
            }

            dataLen++;
        }else{
            break;
        }
    }while(index_1 < wifiinfo_buf.size());

    system("rm ./wifiinfo");
#else
    for (int i = 0; i< 10; i++) {
        QString str("正点原子WIFI模型" + QString::number(i));
        addwireless(str, i);
    }
#endif
    if (m_currentIndex < 0 && playListData.count()) {
        setCurrentIndex(0);
    }

    m_readyFlag = true;
    emit wirelessReadyChanged();
}

void wirelessListModel::wirelessClose()
{
    system("ifconfig wlan0 down");
}

void wirelessListModel::getMyWifiSSIDPSK(QString ssid, QString psk)
{
#ifdef __arm__
    system("sed -i /ssid/d /etc/wpa_supplicant.conf");
    system("sed -i /psk/d /etc/wpa_supplicant.conf");
    system("sed -i /key_mgmt/d /etc/wpa_supplicant.conf");
    system("killall wpa_supplicant");
    system("killall udhcpc");
    /*QString addUserAccount = tr("sed -i 6i\\ssid=\\\"%1\\\" /etc/wpa_supplicant.conf\n").arg(ssid);
    QString addUserCodeKey = tr("sed -i 6i\\psk=\\\"%1\\\" /etc/wpa_supplicant.conf\n").arg(psk);
    system(addUserAccount.toStdString().c_str());
    system(addUserCodeKey.toStdString().c_str());*/
    QString wifiMessage = tr("echo \"ctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0"
                             "\nupdate_config=1\n\nnetwork={\npsk=\\\"%1\\\"\nssid=\\\"%2\\\"\n}\" "
                             "> /etc/wpa_supplicant.conf").arg(psk).arg(ssid);
    system(wifiMessage.toStdString().c_str());

    ConnetWifi = new QProcess;
    connect(ConnetWifi,&QProcess::readyReadStandardOutput,this,&wirelessListModel::standardOutput_WifiConnectSta);
    connect(ConnetWifi,&QProcess::readyReadStandardError,this,&wirelessListModel::standardError_WifiConnectSta);
    ConnetWifi->waitForStarted();
    ConnetWifi->start("wpa_supplicant -D wext -c /etc/wpa_supplicant.conf -i wlan0 &\n"); // usb wifi
    //ConnetWifi->start("wpa_supplicant -Dnl80211 -c /etc/wpa_supplicant.conf -i wlan0 &\n"); // sdio wifi
    ConnetWifi->waitForStarted();
#endif
}

void wirelessListModel::passwordLessConnection(QString ssid)
{
#ifdef __arm__
    system("sed -i /ssid/d /etc/wpa_supplicant.conf");
    system("sed -i /psk/d /etc/wpa_supplicant.conf");
    system("sed -i /key_mgmt/d /etc/wpa_supplicant.conf");
    system("killall wpa_supplicant");
    system("killall udhcpc");
    QString wifiMessage = tr("echo \"ctrl_interface=/var/run/wpa_supplicant\nctrl_interface_group=0"
                             "\nupdate_config=1\n\nnetwork={\n\nssid=\\\"%1\\\"\nkey_mgmt=NONE\n}\" "
                             "> /etc/wpa_supplicant.conf").arg(ssid);
    system(wifiMessage.toStdString().c_str());

    ConnetWifi = new QProcess;
    connect(ConnetWifi,&QProcess::readyReadStandardOutput,this,&wirelessListModel::standardOutput_WifiConnectSta);
    connect(ConnetWifi,&QProcess::readyReadStandardError,this,&wirelessListModel::standardError_WifiConnectSta);
    ConnetWifi->waitForStarted();
    ConnetWifi->write("wpa_supplicant -D wext -c /etc/wpa_supplicant.conf -i wlan0 &\n"); // usb wifi
    //ConnetWifi->start("wpa_supplicant -Dnl80211 -c /etc/wpa_supplicant.conf -i wlan0 &\n"); // sdio wifi
    ConnetWifi->waitForStarted();
#endif
}

void wirelessListModel::standardOutput_WifiConnectSta()
{
    QString pattern = "wlan0: CTRL-(\\S*)";

    QString str = "";
    QString str1 = str.append(ConnetWifi->readAllStandardOutput());
    QString str2 = "wlan0: CTRL-EVENT-CONNECTED";
    QString str3 = "wlan0: CTRL-EVENT-DISCONNECTED";
    QString str4 = "wlan0: CTRL-EVENT-SUBNET-STATUS-UPDATE";

    QRegularExpression regularExpression(pattern);
    QRegularExpressionMatch  match = regularExpression.match(str1, 0);
    if(match.hasMatch()){
        if(match.captured(0) == str2){ // suceess
            system("udhcpc -b -i wlan0");
            m_sate = 1;

        }else if(match.captured(0) == str3) { // fail
            m_sate = 2;
            system("killall wpa_supplicant");
        }else if(match.captured(0) == str4) { // connectting
            m_sate = 3;
        }
        emit returnMainWinWifiResult();
    }
}

void wirelessListModel::standardError_WifiConnectSta()
{

}

void wirelessListModel::move(int from, int to) {
    beginMoveRows(QModelIndex(), from, from, QModelIndex(), to);
    playListData.move(from, to);
    endMoveRows();
}

void wirelessListModel::remove(int first, int last) {
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

void wirelessListModel::setCurrentTitle(QString title) {
    wireless s = playListData.at(m_currentIndex);
    s.settitle(title);
    playListData.replace(m_currentIndex, s);
}

void wirelessListModel::setCurrentIndex(const int & i) {
    if (i >= playListData.count() && m_currentIndex != 0) {
        m_currentIndex = 0;
        emit currentIndexChanged();
    } else if ((i >= 0) && (i < playListData.count()) && (m_currentIndex != i)) {
        m_currentIndex = i;
        emit currentIndexChanged();
    }
}

QHash<int, QByteArray> wirelessListModel::roleNames() const {
    QHash<int, QByteArray> role;
    role[pathRole] = "path";
    role[titleRole] = "title";
    role[levelRole] = "level";
    return role;
}

void wirelessListModel::addwireless(QString title, int level) {
    beginInsertRows(QModelIndex(), playListData.count(), playListData.count());
    playListData.append(wireless(title, level));
    endInsertRows();
}
