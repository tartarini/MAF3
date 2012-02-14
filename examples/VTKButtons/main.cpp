/*
 *  main.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#define BUNDLE_SUFFIX ""

#ifdef __APPLE__
#define BUNDLE_SUFFIX ".app"
#endif

#include <QtGui/QApplication>

#include <mafMainWindow.h>
#include <mafLogic.h>

#include "mafViewOrthoSlice.h"
#include "mafViewVTKButtons.h"
#include "mafOrthoSlice.h"


//#include <mafViewScriptInterpreterPython.h>
//#include <mafInterpreterPreferencesWidget.h>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName(QString("VTKButtons").append(BUNDLE_SUFFIX));
    a.setApplicationVersion("1.0");

    // Create the application's logic instance
    mafApplicationLogic::mafLogic *logic = new mafApplicationLogic::mafLogic();
    logic->setApplicationName(a.applicationName());
    
    
    /// push libraries to load during initialization.
    logic->pushLibraryToLoad("mafResources");
    logic->pushLibraryToLoad("mafSerialization");
    
    // and initialize it. This initialization will load dynamically the mafResources Library.
    bool ok = logic->initialize();
    if(!ok) {
        exit(1);
    }
    
    mafEventBus::mafEventBusManager::instance()->enableEventLogging(true);

    // Plug into the factory the custom objects (Operations, Views, ...).
    // If the object has also an acceptObject method defined, call macro:
    // mafRegisterObjectAndAcceptBind(myNamespace::myClassCustom);
    //
    // If the plugged object hasn't the acceptObject defined, the registration
    // to the mafObjectFactory can be done using the following macro:
    // mafRegisterObject(myNamespace::myClassCustom);
//    mafRegisterObject(mafScriptInterpreter::mafViewScriptInterpreterPython);
    mafRegisterObject(mafViewOrthoSlice);
    mafRegisterObject(mafViewVTKButtons);

    // Plug the object's information into the framework
//    logic->plugObject("mafResources::mafOperation", "mafResources::mafOperationTransform", "Transform");

    // Plug a View into the application
    logic->plugObject("mafResources::mafView", "mafViewOrthoSlice", "OrthoSlice");
    logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "Slice");
    logic->plugObject("mafResources::mafView", "mafViewVTKButtons", "Buttons view");
    // ... and customize it telling to use mafPipeVisualVTKSurface visual pipe to render vtkPolyData data type.
    logic->customizeVisualization("VTK view", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("Buttons view", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("Slice", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKSliceVolume");
    logic->customizeVisualization("Slice", "vtkRectilinearGrid", "mafPluginVTK::mafPipeVisualVTKSliceVolume");

    // Example of View customization with a particular widget
    //mafRegisterQtObject(mafOrthoSlice);
    //logic->customizeViewRootWidget("OrthoSlice", "mafOrthoSlice");

//    logic->plugObject("mafResources::mafView", "mafScriptInterpreter::mafViewScriptInterpreterPython", "Python Console");

    // Create the instance of the main window and pass to it the application's logic.
    // In this way the mafMainWondow class will also load the plug-ins present
    // in the default 'plugins' directory.
    mafMainWindow w(logic);

    // plug custom application's setting page
//    mafScriptInterpreter::mafInterpreterPreferencesWidget *interpreterPrefs = new mafScriptInterpreter::mafInterpreterPreferencesWidget();
//    w.plugApplicationSettingsPage(interpreterPrefs);

    // Eventually call the loadPlugins method with custom directory to allow the application
    // load custom plugins located in custom directories.
    logic->loadPlugins();

    w.setupMainWindow();
    int result = a.exec();

    mafDEL(logic);
    return result;
}
