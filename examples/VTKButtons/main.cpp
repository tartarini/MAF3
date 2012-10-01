/*
 *  main.cpp
 *  VTKButtons
 *
 *  Created by Paolo Quadrani on 27/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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

#include <fvupdater.h>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName(QString("VTKButtons").append(BUNDLE_SUFFIX));
    a.setApplicationVersion("1.0");
    
    a.setWindowIcon(QIcon(":/images/icon_vtkButtons.png"));

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

    mafRegisterObject(mafViewOrthoSlice);
    mafRegisterObject(mafViewVTKButtons);

    // Plug the object's information into the framework
//    logic->plugObject("mafResources::mafOperation", "mafResources::mafOperationTransform", "Transform");

    // Plug a View into the application
    logic->plugObject("mafResources::mafView", "mafViewOrthoSlice", "OrthoSlice");
    logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "View Iso");
    logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "Slice");
    logic->plugObject("mafResources::mafView", "mafViewVTKButtons", "Buttons Slice");
    logic->plugObject("mafResources::mafView", "mafViewVTKButtons", "Buttons Iso");
    // ... and customize it telling to use mafPipeVisualVTKSurface visual pipe to render vtkPolyData data type.
    logic->customizeVisualization("View Iso", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("View Iso", "vtkRectilinearGrid", "mafPluginVTK::mafPipeVisualVTKIsoSurface");
    logic->customizeVisualization("View Iso", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKIsoSurface");
    logic->customizeVisualization("VTK view", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("VTK view", "vtkImageData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("VTK view", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKBox");
    logic->customizeVisualization("Buttons Slice", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("Buttons Slice", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKSliceVolume");
    logic->customizeVisualization("Buttons Slice", "vtkRectilinearGrid", "mafPluginVTK::mafPipeVisualVTKSliceVolume");
    logic->customizeVisualization("Buttons Iso", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    logic->customizeVisualization("Buttons Iso", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKIsoSurface");
    logic->customizeVisualization("Buttons Iso", "vtkRectilinearGrid", "mafPluginVTK::mafPipeVisualVTKIsoSurface");
    logic->customizeVisualization("Slice", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKSliceVolume");
    logic->customizeVisualization("Slice", "vtkRectilinearGrid", "mafPluginVTK::mafPipeVisualVTKSliceVolume");

    // Example of View customization with a particular widget
    //mafRegisterQtObject(mafOrthoSlice);
    //logic->customizeViewWidget("OrthoSlice", "mafOrthoSlice");

    // Create the instance of the main window and pass to it the application's logic.
    // In this way the mafMainWondow class will also load the plug-ins present
    // in the default 'plugins' directory.
    mafMainWindow w(logic);

    // Eventually call the loadPlugins method with custom directory to allow the application
    // load custom plugins located in custom directories.
    logic->loadPlugins();

    w.setupMainWindow();

    //--- Updater
    FvUpdater::sharedUpdater()->SetFeedURL("http://pypt.github.com/fervor/Appcast.xml");
    FvUpdater::sharedUpdater()->CheckForUpdatesSilent();
    //-----------------------

    int result = a.exec();

    mafDEL(logic);
    return result;
}
