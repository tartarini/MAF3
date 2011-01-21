/*
 *  mafPluginRegistrator.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPluginRegistrator.h"
//#include "mafDataPipeSurface.h"
#include "mafDataPipeImageThreshold.h"
#include "mafVisualPipeVTKSurface.h"
#include "mafVisualPipeSelection.h"
#include "mafViewVTK.h"
#include "mafExternalDataCodecVTK.h"
#include "mafVTKParametricSurfaceSphere.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;
using namespace mafResources;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    //mafRegisterObject(mafPluginVTK::mafDataPipeSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafDataPipeImageThreshold);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeSelection);
    mafRegisterObject(mafPluginVTK::mafViewVTK);
    mafRegisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    //mafUnregisterObject(mafPluginVTK::mafDataPipeSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafDataPipeImageThreshold);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafVisualPipeVTKSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafVisualPipeSelection);
    mafUnregisterObject(mafPluginVTK::mafViewVTK);
    mafUnregisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
}

void mafPluginRegistrator::registerObjects() {
    mafPluggedObjectsHash pluginHash;

    //mafPluggedObjectInformation dataPipeSurface("Data pipe Surface", "mafPluginVTK::mafDataPipeSurface");
    mafPluggedObjectInformation dataPipeImageThreshold("Data pipe Image Threshold", "mafPluginVTK::mafDataPipeImageThreshold");
    mafPluggedObjectInformation visualPipeVTKSurface("Visual pipe VTK Surface", "mafPluginVTK::mafVisualPipeVTKSurface");
    mafPluggedObjectInformation visualPipeSelection("Visual pipe used to represent selection of data", "mafPluginVTK::mafVisualPipeSelection");
    mafPluggedObjectInformation viewVTK("VTK view", "mafPluginVTK::mafViewVTK");
    mafPluggedObjectInformation externalDataCodecVTK("VTK codec", "mafPluginVTK::mafExternalDataCodecVTK");

    //pluginHash.insertMulti("mafResources::mafDataPipe", dataPipeSurface);
    pluginHash.insertMulti("mafResources::mafDataPipe", dataPipeImageThreshold);
    pluginHash.insertMulti("mafResources::mafVisualPipe", visualPipeVTKSurface);
    pluginHash.insertMulti("mafResources::mafVisualPipe", visualPipeSelection);
    pluginHash.insertMulti("mafResources::mafView", viewVTK);
    pluginHash.insertMulti("mafCore::mafExternalDataCodec", externalDataCodecVTK);

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}
