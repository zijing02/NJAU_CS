#include "webapi.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Webapi w;
    Q_UNUSED(w);
    return a.exec();
}
