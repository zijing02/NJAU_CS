#include "audiorecorder.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AudioRecorder w;
    Q_UNUSED(w);
    return a.exec();
}
