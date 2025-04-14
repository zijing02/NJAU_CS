#include "slidepage.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SlidePage w;
    w.show();
    return a.exec();
}
