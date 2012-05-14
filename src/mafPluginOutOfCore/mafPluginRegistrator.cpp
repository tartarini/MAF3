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
#include "mafImporterCTKDICOM.h"

#include <mafPluginConfigurator.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginCTK;
using namespace mafResources;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    mafRegisterObjectAndAcceptBind(mafPluginCTK::mafImporterCTKDICOM);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    mafUnregisterObjectAndAcceptUnbind(mafPluginCTK::mafImporterCTKDICOM);
}

void mafPluginRegistrator::registerAllObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation importerCTKDICOM("CTK DICOM Importer", "mafPluginCTK::mafImporterCTKDICOM");
    
    pluginHash.insertMulti("mafResources::mafImporter", importerCTKDICOM);
    
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

void mafPluginRegistrator::registerObjects() {
    mafPluginConfigurator configurator;
    if (!configurator.parseConfigurationFile("mafPluginCTK.xml")) {
        registerAllObjects();
        return;
    }
}
