/*
 *  mafPluginRegistrator.cpp
 *  mafPluginEventBusMonitor
 *
 *  Created by Daniele Giunchi on 14/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"
#include "mafViewEventBusMonitor.h"

#include <mafPluginConfigurator.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginEventBusMonitor;
using namespace mafResources;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    mafRegisterObject(mafPluginEventBusMonitor::mafViewEventBusMonitor);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    mafUnregisterObject(mafPluginEventBusMonitor::mafViewEventBusMonitor);
}

void mafPluginRegistrator::registerAllObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation view("EventBusView", "mafPluginEventBusMonitor::mafViewEventBusMonitor");
    
    pluginHash.insertMulti("mafResources::mafView", view);
    
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

void mafPluginRegistrator::registerObjects() {
    mafPluginConfigurator configurator;
    if (!configurator.parseConfigurationFile("mafPluginEventBusMonitor.xml")) {
        registerAllObjects();
        return;
    }
}
