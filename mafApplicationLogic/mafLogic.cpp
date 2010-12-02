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
#include <mafEventDefinitions.h>

using namespace mafEventBus;

#ifdef WIN32
    #define PLUGIN_EXTENSION_FILTER "*.dll"
    #ifdef QT_DEBUG
        #define RESOURCES_LIBRARY_NAME "mafResources_d.dll"
    #else
        #define RESOURCES_LIBRARY_NAME "mafResources.dll"
    #endif
#else
    #ifdef __APPLE__
        #define PLUGIN_EXTENSION_FILTER "*.dylib"
        #ifdef QT_DEBUG
            #define RESOURCES_LIBRARY_NAME "libmafResources_debug.dylib"
        #else
            #define RESOURCES_LIBRARY_NAME "libmafResources.dylib"
        #endif
    #else
        #define PLUGIN_EXTENSION_FILTER "*.so"
        #ifdef QT_DEBUG
            #define RESOURCES_LIBRARY_NAME "libmafResources_debug.so"
        #else
            #define RESOURCES_LIBRARY_NAME "libmafResources.so"
        #endif
    #endif
#endif


using namespace mafCore;
using namespace mafApplicationLogic;

mafLogic::mafLogic(const mafString code_location) : mafLogicLight(code_location), m_WorkingDirectory(""), m_ApplicationDirectory(QDir::currentPath()) {
}

mafLogic::~mafLogic() {
}

bool mafLogic::initialize() {
    // Call the ionitialization of the superclass.
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
    mafRegisterLocalSignal("maf.local.logic.openFile", this, "openFile(const mafString)");
    mafRegisterLocalSignal("maf.local.logic.status.viewmanager.store", this, "statusViewManagerStore()");
    mafRegisterLocalSignal("maf.local.logic.status.viewmanager.restore", this, "statusViewManagerRestore(mafCore::mafMemento *, bool)");
    mafRegisterLocalSignal("maf.local.logic.status.vmemanager.store", this, "statusVmeManagerStore()");
    mafRegisterLocalSignal("maf.local.logic.status.vmemanager.restore", this, "statusVmeManagerRestore(mafCore::mafMemento *, bool)");

    // Slot registration.
    mafRegisterLocalCallback("maf.local.logic.settings.store", this, "storeSettings()");
    mafRegisterLocalCallback("maf.local.logic.settings.restore", this, "restoreSettings()");

    // Load the module related to the resources and managers and initialize it.
    bool module_initialized(false);
    module_initialized = mafInitializeModule(RESOURCES_LIBRARY_NAME);

    // load the plugins (if any)
//    loadPlugins();

    // Perform design by contract check.
    ENSURE(module_initialized);
    return module_initialized;
}

void mafLogic::loadPlugins(mafString plugin_dir) {
    // Build the plugin absolute directory.
    mafString pluginDir = plugin_dir.isEmpty() ? (m_ApplicationDirectory + QDir::toNativeSeparators("/plugins")) : plugin_dir;
    pluginDir = QDir::cleanPath(pluginDir);

    // Check for plugins to load
    QDir dir(pluginDir);
    mafStringList filters;
    filters << PLUGIN_EXTENSION_FILTER;
    mafStringList plugin_list = dir.entryList(filters);
    mafEventArgumentsList argList;

    // For each plugin file ask the plugin manager to load it through the event bus.
    foreach(mafString file, plugin_list) {
        argList.clear();
        file = dir.absoluteFilePath(file);
        argList.append(mafEventArgument(mafString, file));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.loadLibrary", mafEventTypeLocal, &argList);
    }
}

void mafLogic::storeSettings() {

}

void mafLogic::restoreSettings() {

}
