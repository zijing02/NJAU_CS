#include "appdemo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AppDemo w;
    w.show();
    return a.exec();
}
