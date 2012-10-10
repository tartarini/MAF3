/*
 *  mafPluginRegistrator.cpp
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"
#include "mafPipeDataSurfacePluginTest.h"


using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginTest;
using namespace mafResources;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    mafRegisterObject(mafPluginTest::mafPipeDataSurfacePluginTest);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    mafUnregisterObject(mafPluginTest::mafPipeDataSurfacePluginTest);
}

void mafPluginRegistrator::registerObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation dataPipeSurfacePluginTest("Data pipe Surface Plugin Test", "mafPluginTest::mafPipeDataSurfacePluginTest");

    pluginHash.insertMulti("mafResources::mafPipeData", dataPipeSurfacePluginTest);


    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}
