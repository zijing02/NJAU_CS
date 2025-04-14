/******************************************************************
Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
* @projectName   QDesktop
* @brief         main.cpp
* @author        Deng Zhimao
* @email         1252699831@qq.com
* @date          2020-07-31
*******************************************************************/
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "desktop/desktop.h"

#include "weather/weatherdata.h"
#include "weather/mymodel.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "music/lyricmodel.h"
#include "music/playlistmodel.h"

#include "media/mediaListModel.h"

#include "wireless/wirelessmodel.h"

#include "tcpclient/mytcpclient.h"

#include "tcpserver/mytcpserver.h"

#include "udpchat/udpreciver.h"
#include "udpchat/udpsender.h"

#include "photoview/photolistmodel.h"

#include "iotest/beep.h"

#include "sensor/ap3216c.h"
#include "sensor/icm20608.h"

#include "radio/radio.h"

#include "fileview/fileio.h"

#include "cameramedia/cameramedia.h"

#include <QDir>
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    QDir::setCurrent(QCoreApplication::applicationDirPath());

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    qmlRegisterType<IndexData>("an.weather", 1, 0, "IndexData");
    qmlRegisterType<WeatherData>("an.weather", 1, 0, "WeatherData");
    qmlRegisterType<MyModel>("an.model", 1, 0, "MyModel");

    qmlRegisterType<lyricModel>("dataModel", 1, 0, "LyricModel");
    qmlRegisterType<playListModel>("dataModel", 1, 0, "PlayListModel");

    qmlRegisterType<mediaListModel>("mediaModel", 1, 0, "MediaListModel");

    qmlRegisterType<wirelessListModel>("wirelessModel", 1, 0, "WirelessListModel");

    qmlRegisterType<myTcpclient>("mytcpclient", 1, 0, "MyTcpclient");

    qmlRegisterType<myTcpserver>("mytcpserver", 1, 0, "MyTcpserver");

    qmlRegisterType<UdpSender>("udpsender", 1, 0, "UdpSender");
    qmlRegisterType<UdpReciver>("udpreciver", 1, 0, "UdpReciver");

    qmlRegisterType<Beep>("beep", 1, 0, "Beep");

    qmlRegisterType<Ap3216c>("ap3216c", 1, 0, "Ap3216c");
    qmlRegisterType<Icm20608>("icm20608", 1, 0, "Icm20608");


    qmlRegisterType<myRadio>("radio", 1, 0, "MyRadio");

    qmlRegisterType<MyDesktop>("myDesktop", 1, 0, "MyDesktop");

    qmlRegisterType<FileIO, 1>("fileIO", 1, 0, "FileIO");

    qmlRegisterType<CameraMedia, 1>("MyCameraMedia", 1, 0, "CameraMedia");

    QQmlApplicationEngine engine;

#if WIN32
    engine.rootContext()->setContextProperty("WINenv", true);
#else
    engine.rootContext()->setContextProperty("WINenv", false);
#endif

    engine.rootContext()->setContextProperty("WINStyle", true);

    engine.rootContext()->setContextProperty("appCurrtentDir", QCoreApplication::applicationDirPath());

    MyModel *myModel = new MyModel();
    MyDesktop *myDesktop = new MyDesktop();
    photoListModel *myPhoto = new photoListModel();
    engine.rootContext()->setContextProperty("myPhoto", myPhoto);
    engine.rootContext()->setContextProperty("myModel", myModel);
    engine.rootContext()->setContextProperty("myDesktop", myDesktop);

    myPhoto->add(QCoreApplication::applicationDirPath() + "/src/images/");

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
