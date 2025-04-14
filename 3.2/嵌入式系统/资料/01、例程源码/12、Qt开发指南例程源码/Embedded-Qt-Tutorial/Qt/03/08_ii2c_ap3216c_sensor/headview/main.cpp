#include "headview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    HeadView w;
    w.show();
    return a.exec();
}
