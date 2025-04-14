#include "asr.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Asr w;
    w.show();
    return a.exec();
}
