/*
 *  mafPluginRegistrator.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi && Roberto Mucci on 14/12/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"
#include "mafMementoVolume.h"

#include <mafPluginConfigurator.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginOutOfCore;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    mafRegisterObject(mafPluginOutOfCore::mafMementoVolume);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    mafUnregisterObject(mafPluginOutOfCore::mafMementoVolume);
}

void mafPluginRegistrator::registerAllObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation mementoVolume("Memento Volume", "mafPluginOutOfCore::mafMementoVolume");
    
    pluginHash.insertMulti("mafCore::mafMemento", mementoVolume);
    
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

void mafPluginRegistrator::registerObjects() {
    mafPluginConfigurator configurator;
    if (!configurator.parseConfigurationFile("mafPluginOutOfCore.xml")) {
        registerAllObjects();
        return;
    }
}
