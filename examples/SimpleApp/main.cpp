#include <QtGui/QApplication>

#include "mafOperationSimpleApp.h"
#include "mafViewScriptInterpreterPython.h"
#include "mafApplicationSettingsPageConfigurations.h"

#include <mafMainWindow.h>
#include <mafLogic.h>


#include <mafInterpreterPreferencesWidget.h>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName("SimpleApp");
    a.setApplicationVersion("1.0");

    // Create the application's logic instance
    mafApplicationLogic::mafLogic *logic = new mafApplicationLogic::mafLogic();
    // and initialize it. This initialization will load dynamically the mafResources Library.
    bool ok = logic->initialize();
    if(!ok) {
        exit(1);
    }

    // Plug into the factory the new operation. This operation has also
    // an acceptObject method defined, so call macro below.
    // If the plugged object hasn't the acceptObject defined, the registration
    // to the mafObjectFactory can be done using the following macro:
    // mafRegisterObject(myClassCustom);
    mafRegisterObjectAndAcceptBind(mafOperationSimpleApp);
    mafRegisterObject(mafViewScriptInterpreterPython);
    // Plug the object's information into the framework
    //logic->plugObject("mafResources::mafOperation", "mafOperationSimpleApp", "Demo Operation");
    //logic->plugObject("mafResources::mafOperation", "mafResources::mafOperationTransform", "Transform");

    logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "View MIP");
    logic->customizeVisualization("View MIP", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("View MIP", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKMIPVolume");

    logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "View Iso");
    logic->customizeVisualization("View Iso", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("View Iso", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKIsoSurface");

    logic->customizeVisualization("VTK view", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");

    logic->plugObject("mafResources::mafView", "mafViewScriptInterpreterPython", "Python Console");
    // Create the instance of the main window and pass to it the application's logic.
    // In this way the mafMainWondow class will also load the plug-ins present
    // in the default 'plugins' directory.
    mafMainWindow w(logic);

    // plug custom application's setting page
    mafApplicationSettingsPageConfigurations *page = new mafApplicationSettingsPageConfigurations();
    w.plugApplicationSettingsPage(page);

    mafScriptInterpreter::mafInterpreterPreferencesWidget *interpreterPrefs = new mafScriptInterpreter::mafInterpreterPreferencesWidget();
    w.plugApplicationSettingsPage(interpreterPrefs);

    // Eventually call the loadPlugins method with custom directory to allow the application
    // load custom plugins located in custom directories.
    //logic->loadPlugins(cusom_plugin_path);

    w.setupMainWindow();
    int result = a.exec();

    mafDEL(logic);

    return result;
}
