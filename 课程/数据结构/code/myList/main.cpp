#include "listdemo.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ListDemo w;
    w.show();

    return a.exec();
}
