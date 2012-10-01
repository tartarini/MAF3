/*
 *  mafPluginRegistrator.cpp
 *  mafPluginQt
 *
 *  Created by Paolo Quadrani on 19/03/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"
#include "mafPipeDataSurfaceQtPluginTest.h"


using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginQtTest;


mafPluginRegistrator::~mafPluginRegistrator() {
    mafUnregisterObject(mafPluginQtTest::mafPipeDataSurfaceQtPluginTest);
}

void mafPluginRegistrator::registerObjects() {
    /// register in maf factory
    mafRegisterObject(mafPluginQtTest::mafPipeDataSurfaceQtPluginTest);
    
    
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation dataPipeSurfacePluginQt("Data pipe Surface Qt Plugin Test", "mafPluginQtTest::mafPipeDataSurfaceQtPluginTest");

    pluginHash.insertMulti("mafResources::mafPipeData", dataPipeSurfacePluginQt);


    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}

mafResources::mafPluginInfo mafPluginRegistrator::pluginInfo() {
    mafResources::mafPluginInfo info;
    info.m_Version = 1.0;
    info.m_PluginName = "Plugin Qt";
    info.m_Author = "Paolo Quadrani & Daniele Giunchi";
    info.m_Vendor = "SCS-B3C";
    info.m_VendorHomepage = "http:///www.openmaf.org/";
    info.m_Description = "Qt plugin: created only for be compliant to Qt plugins";

    return info;
}

Q_EXPORT_PLUGIN2(mafPluginQtTest, mafPluginQtTest::mafPluginRegistrator)
