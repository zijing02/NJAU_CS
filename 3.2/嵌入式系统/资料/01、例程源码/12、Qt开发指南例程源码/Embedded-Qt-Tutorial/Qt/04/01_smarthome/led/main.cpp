#include "led.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Led w;
    Q_UNUSED(w);
    return a.exec();
}
