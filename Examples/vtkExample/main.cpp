#include <QtGui/QApplication>
#include "mafMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mafMainWindow w;
    w.show();
    return a.exec();
}
