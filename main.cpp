#include "qcode.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qcode w;
    w.show();
    return a.exec();
}
