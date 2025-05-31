QT       += core gui
QT       += testlib
QT       += multimedia
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    content.cpp \
    form1.cpp \
    form2.cpp \
    form3.cpp \
    form4.cpp \
    form5.cpp \
    form6.cpp \
    form7.cpp \
    main.cpp \
    widget.cpp

HEADERS += \
    content.h \
    form1.h \
    form2.h \
    form3.h \
    form4.h \
    form5.h \
    form6.h \
    form7.h \
    widget.h

FORMS += \
    form1.ui \
    form2.ui \
    form3.ui \
    form4.ui \
    form5.ui \
    form6.ui \
    form7.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
