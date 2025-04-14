#include "ocr.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Ocr w;
    Q_UNUSED(w)
    return a.exec();
}
