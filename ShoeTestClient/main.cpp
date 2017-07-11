#include "shoeclient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ShoeClient w;
    w.show();

    return a.exec();
}
