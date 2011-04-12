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

#include "mafPipeDataImageThreshold.h"
#include "mafPipeVisualVTKSurface.h"
#include "mafPipeVisualSelection.h"

#include "mafViewVTK.h"

#include "mafOperationParametricSurface.h"
#include "mafImporterVTK.h"

#include "mafExternalDataCodecVTK.h"

#include "mafVTKParametricSurfaceSphere.h"
#include "mafVTKParametricSurfaceCube.h"
#include "mafVTKParametricSurfaceCone.h"
#include "mafVTKParametricSurfaceCylinder.h"
#include "mafVTKParametricSurfaceEllipsoid.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;
using namespace mafResources;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeDataImageThreshold);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualSelection);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafOperationParametricSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafImporterVTK);
    mafRegisterObject(mafPluginVTK::mafViewVTK);
    mafRegisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCube);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCone);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCylinder);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    //mafUnregisterObject(mafPluginVTK::mafPipeDataSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeDataImageThreshold);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualSelection);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafOperationParametricSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafImporterVTK);
    mafUnregisterObject(mafPluginVTK::mafViewVTK);
    mafUnregisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCube);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCone);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCylinder);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);
}

void mafPluginRegistrator::registerObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation dataPipeImageThreshold("Data pipe Image Threshold", "mafPluginVTK::mafPipeDataImageThreshold");
    mafPluggedObjectInformation visualPipeVTKSurface("Visual pipe VTK Surface", "mafPluginVTK::mafPipeVisualVTKSurface");
    mafPluggedObjectInformation visualPipeSelection("Visual pipe used to represent selection of data", "mafPluginVTK::mafPipeVisualSelection");
    mafPluggedObjectInformation viewVTK("VTK view", "mafPluginVTK::mafViewVTK");
    mafPluggedObjectInformation opParametricSurface("Create Parametric Surface", "mafPluginVTK::mafOperationParametricSurface");
    mafPluggedObjectInformation importerVTK("Import VTK data", "mafPluginVTK::mafImporterVTK");
    mafPluggedObjectInformation externalDataCodecVTK("VTK codec", "mafPluginVTK::mafExternalDataCodecVTK");

    pluginHash.insertMulti("mafResources::mafPipeData", dataPipeImageThreshold);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeVTKSurface);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeSelection);
    pluginHash.insertMulti("mafResources::mafView", viewVTK);
    pluginHash.insertMulti("mafResources::mafOperation", opParametricSurface);
    pluginHash.insertMulti("mafResources::mafImporter", importerVTK);
    pluginHash.insertMulti("mafCore::mafExternalDataCodec", externalDataCodecVTK);

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
}
