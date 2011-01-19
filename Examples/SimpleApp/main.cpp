#include <QtGui/QApplication>

#include "mafMainWindow.h"
#include "mafOperationSimpleApp.h"

#include <mafLogic.h>

#ifdef WIN32
    #define VTK_PLUGIN_NAME "mafPluginVTK_d.mafplugin"
#else
    #define VTK_PLUGIN_NAME "libmafPluginVTK_debug.mafplugin"
#endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName("SimpleApp");
    a.setApplicationVersion("1.0");

    mafString pluginVTK;
    pluginVTK.append(VTK_PLUGIN_NAME);

    mafString p = QDir::currentPath();
    p.append(QDir::separator());
    p.append("plugins");

    QDir pluginDir(p);
    if(!pluginDir.exists(p)) {
        pluginDir.mkdir(p);
    }
    p.append(QDir::separator());
    p.append(VTK_PLUGIN_NAME);

    // Remove a previous version of the plug-in just to be updated in a
    // development phase.
    bool res = QFile::remove(p);
    // Copy the new one into the plugin directory.
    res = QFile::copy(pluginVTK, p);

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
    logic.plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "View 3D");

    mafMainWindow w(&logic);
    logic.loadPlugins();

    w.show();
    int result = a.exec();

    return result;
}
