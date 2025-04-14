#/******************************************************************
#Copyright © Deng Zhimao Co., Ltd. 1990-2030. All rights reserved.
#* @projectName   QDesktop
#* @brief         摘要
#* @author        Deng Zhimao
#* @email         1252699831@qq.com
#* @date          2020-06-01
#*******************************************************************/
QT += quick core qml network positioning widgets quickcontrols2 widgets multimedia
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cameramedia/cameramedia.cpp \
    fileview/fileio.cpp \
        main.cpp \
    weather/myjsonparse.cpp \
    weather/mymodel.cpp \
    weather/weatherdata.cpp \
    music/lyricmodel.cpp \
    music/playlistmodel.cpp \
    media/mediaListModel.cpp \
    wireless/wirelessmodel.cpp \
    tcpclient/mytcpclient.cpp \
    tcpserver/mytcpserver.cpp \
    desktop/desktop.cpp \
    udpchat/udpreciver.cpp \
    udpchat/udpsend.cpp \
    photoview/photolistmodel.cpp \
    iotest/beep.cpp \
    sensor/ap3216c.cpp \
    sensor/icm20608.cpp \
    iotest/keysthread.cpp \
    radio/radio.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cameramedia/cameramedia.h \
    fileview/fileio.h \
    weather/myjsonparse.h \
    weather/mymodel.h \
    weather/weatherdata.h \
    music/lyricmodel.h \
    music/playlistmodel.h \
    media/mediaListModel.h \
    wireless/wirelessmodel.h \
    tcpclient/mytcpclient.h \
    tcpserver/mytcpserver.h \
    desktop/desktop.h \
    udpchat/udpreciver.h \
    udpchat/udpsender.h \
    photoview/photolistmodel.h \
    iotest/beep.h \
    sensor/ap3216c.h \
    sensor/icm20608.h \
    iotest/keysthread.h \
    radio/radio.h

DISTFILES +=
