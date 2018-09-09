#include "Promerel.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Promerel w;
    w.show();
    return a.exec();
}
