/*
 *  mafLogic.cpp
 *  mafApplicationLogic
 *
 *  Created by Daniele Giunchi on 28/06/10.
 *  Copyright 2009-2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafLogic.h"

#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>


using namespace mafEventBus;

#define PLUGIN_EXTENSION_FILTER "*.mafplugin"

#ifdef WIN32
    #define RESOURCES_LIBRARY_NAME "mafResources.dll"
#else
    #ifdef __APPLE__
        #define RESOURCES_LIBRARY_NAME "libmafResources.dylib"
    #else
        #define RESOURCES_LIBRARY_NAME "libmafResources.so"
    #endif
#endif


using namespace mafCore;
using namespace mafApplicationLogic;

mafLogic::mafLogic(const QString code_location) : mafLogicLight(code_location), m_WorkingDirectory(""), m_ApplicationDirectory(QDir::currentPath()) {
}

mafLogic::~mafLogic() {
    // Unload the mafResourcesLibrary.
    bool result(false);
    result = mafShutdownModule(m_LibraryHandlersHash.value(RESOURCES_LIBRARY_NAME));
    if(result) {
        m_LibraryHandlersHash.remove(RESOURCES_LIBRARY_NAME);
    }
}

bool mafLogic::initialize() {
    // Call the initialization of the superclass.
    Superclass::initialize();

    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.logic.openFile");
    provider->createNewId("maf.local.logic.status.viewmanager.store");
    provider->createNewId("maf.local.logic.status.viewmanager.restore");
    provider->createNewId("maf.local.logic.status.vmemanager.store");
    provider->createNewId("maf.local.logic.status.vmemanager.restore");
    provider->createNewId("maf.local.logic.settings.store");
    provider->createNewId("maf.local.logic.settings.restore");

    // Signal registration.
    mafRegisterLocalSignal("maf.local.logic.openFile", this, "openFile(const QString)");
    mafRegisterLocalSignal("maf.local.logic.status.viewmanager.store", this, "statusViewManagerStore()");
    mafRegisterLocalSignal("maf.local.logic.status.viewmanager.restore", this, "statusViewManagerRestore(mafCore::mafMemento *, bool)");
    mafRegisterLocalSignal("maf.local.logic.status.vmemanager.store", this, "statusVmeManagerStore()");
    mafRegisterLocalSignal("maf.local.logic.status.vmemanager.restore", this, "statusVmeManagerRestore(mafCore::mafMemento *, bool)");
    mafRegisterLocalSignal("maf.local.logic.settings.store", this, "writeSettings()");
    mafRegisterLocalSignal("maf.local.logic.settings.restore", this, "readSettings()");

    // Slot registration.
    mafRegisterLocalCallback("maf.local.logic.settings.store", this, "storeSettings()");
    mafRegisterLocalCallback("maf.local.logic.settings.restore", this, "restoreSettings()");

    m_CustomPluggedObjectsHash.clear();

    // Load the module related to the resources and managers and initialize it.
    QLibrary *handler(NULL);
    handler = mafInitializeModule(RESOURCES_LIBRARY_NAME);
    if(handler) {
        m_LibraryHandlersHash.insert(RESOURCES_LIBRARY_NAME, handler);
    }

    requestNewHierarchy();

    // Perform design by contract check.
    ENSURE(handler);
    return handler != NULL;
}

mafCore::mafHierarchy *mafLogic::requestNewHierarchy() {
    // Initialize data hierarchy
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, m_Hierarchy);
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.new", mafEventTypeLocal, NULL, &ret_val);
    
    //clean the scenegraphs of all the views
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.clearViews");
    
    return m_Hierarchy;
}

void mafLogic::plugObject(const QString base_class, const QString class_type, const QString object_label) {
    // Add information to the
    mafPluggedObjectInformation objectInformation(object_label, class_type);
    // add the plugged object to the hash
    m_CustomPluggedObjectsHash.insertMulti(base_class, objectInformation);
}

void mafLogic::loadPlugins(QString plugin_dir) {
    // Compose the plugin absolute directory.
    QString pluginDir = plugin_dir.isEmpty() ? (m_ApplicationDirectory + QDir::toNativeSeparators("/plugins/")) : plugin_dir;
    pluginDir = QDir::cleanPath(pluginDir);
    
    // Check for plugins to load
    QStringList filters;
    filters << PLUGIN_EXTENSION_FILTER;
    QDir dir(pluginDir);
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files);
    QStringList plugin_list = dir.entryList();

    mafEventArgumentsList argList;

    // For each plugin file ask the plugin manager to load it through the event bus.
    foreach(QString file, plugin_list) {
        argList.clear();
        file = dir.absoluteFilePath(file);
        argList.append(mafEventArgument(QString, file));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.loadLibrary", mafEventTypeLocal, &argList);
    }

    // Plug also the custom objects plugged from the vertical application.
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, m_CustomPluggedObjectsHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

void mafLogic::storeSettings() {
    qDebug() << "Writing mafLogic settings...";
    QSettings s;
    s.setValue("workingDir", m_WorkingDirectory);
}

void mafLogic::restoreSettings() {
    qDebug() << "Reading mafLogic settings...";
    QSettings s;
    m_WorkingDirectory = s.value("workingDir").toString();
}
