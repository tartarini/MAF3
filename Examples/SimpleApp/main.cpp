#include <QtGui/QApplication>

#include "mafMainWindow.h"
#include "mafOperationSimpleApp.h"

#include <mafLogic.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName("SimpleApp");
    a.setApplicationVersion("1.0");

    mafApplicationLogic::mafLogic logic;
    logic.initialize();

    // Plug into the factory the new operation. This operation has also
    // an acceptObject method defined, so call macro below.
    // If the plugged object hasn't the acceptObject defined, the registration
    // to the mafObjectFactory can be done using the following macro:
    // mafRegisterObject(myClassCustom);
    mafRegisterObjectAndAcceptBind(mafOperationSimpleApp);
    // Plug the object's information into the framework
    logic.plugObject("mafResources::mafOperation", "mafOperationSimpleApp", "Demo Operation");

    mafMainWindow w;
    logic.loadPlugins();

    w.show();
    int result = a.exec();

    return result;
}
