#include "esp8266.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Esp82266 w;
    Q_UNUSED(w);

    return a.exec();
}
