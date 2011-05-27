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
#include "mafExporterVTK.h"


#include "mafExternalDataCodecVTK.h"

#include "mafVTKParametricSurfaceSphere.h"
#include "mafVTKParametricSurfaceCube.h"
#include "mafVTKParametricSurfaceCone.h"
#include "mafVTKParametricSurfaceCylinder.h"
#include "mafVTKParametricSurfaceEllipsoid.h"
#include "mafDataBoundaryAlgorithmVTK.h"

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
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafExporterVTK);
    mafRegisterObject(mafPluginVTK::mafViewVTK);
    mafRegisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCube);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCone);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCylinder);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);
    mafRegisterObject(mafPluginVTK::mafDataBoundaryAlgorithmVTK);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    //mafUnregisterObject(mafPluginVTK::mafPipeDataSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeDataImageThreshold);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualSelection);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafOperationParametricSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafImporterVTK);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafExporterVTK);
    mafUnregisterObject(mafPluginVTK::mafViewVTK);
    mafUnregisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCube);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCone);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCylinder);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);
    mafUnregisterObject(mafPluginVTK::mafDataBoundaryAlgorithmVTK);
    
}

void mafPluginRegistrator::registerObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation dataPipeImageThreshold("Data pipe Image Threshold", "mafPluginVTK::mafPipeDataImageThreshold");
    mafPluggedObjectInformation visualPipeVTKSurface("Visual pipe VTK Surface", "mafPluginVTK::mafPipeVisualVTKSurface");
    mafPluggedObjectInformation visualPipeSelection("Visual pipe used to represent selection of data", "mafPluginVTK::mafPipeVisualSelection");
    mafPluggedObjectInformation viewVTK("VTK view", "mafPluginVTK::mafViewVTK");
    mafPluggedObjectInformation opParametricSurface("Create Parametric Surface", "mafPluginVTK::mafOperationParametricSurface");
    mafPluggedObjectInformation importerVTK("Import VTK data", "mafPluginVTK::mafImporterVTK");
    mafPluggedObjectInformation exporterVTK("Export VTK data", "mafPluginVTK::mafExporterVTK");
    mafPluggedObjectInformation externalDataCodecVTK("VTK codec", "mafPluginVTK::mafExternalDataCodecVTK");
    mafPluggedObjectInformation dataBoundaryAlgorithmVTK("VTK BoundaryAlgorithm", "mafPluginVTK::mafDataBoundaryAlgorithmVTK");

    pluginHash.insertMulti("mafResources::mafPipeData", dataPipeImageThreshold);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeVTKSurface);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeSelection);
    pluginHash.insertMulti("mafResources::mafView", viewVTK);
    pluginHash.insertMulti("mafResources::mafOperation", opParametricSurface);
    pluginHash.insertMulti("mafResources::mafImporter", importerVTK);
    pluginHash.insertMulti("mafResources::mafExporter", exporterVTK);
    pluginHash.insertMulti("mafCore::mafExternalDataCodec", externalDataCodecVTK);
    pluginHash.insertMulti("mafResources::mafDataBoundaryAlgorithm", dataBoundaryAlgorithmVTK);

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);
    
    //plug codec
    //Load serialization plugin
    QString plug_codec_id = "maf.local.serialization.plugCodec";
    QString encodeType = "XML";
    QString codec = "mafSerialization::mafCodecXML";
    
    argList.clear();
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);
    
    encodeType = "VTK";
    codec = "mafPluginVTK::mafExternalDataCodecVTK";
    
    argList.clear();
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent(plug_codec_id, mafEventTypeLocal, &argList);
}
