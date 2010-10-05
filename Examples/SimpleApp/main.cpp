#include <QtGui/QApplication>
#include <mafLogic.h>

#include "mafMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName("SimpleApp");
    a.setApplicationVersion("1.0");

    mafApplicationLogic::mafLogic l;
    l.initialize();

    mafMainWindow w;
    w.setSettingsFilename("./SimpleAppSettings");
    w.show();

    return a.exec();
}
