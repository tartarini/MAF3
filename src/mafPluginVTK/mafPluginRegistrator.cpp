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
#include "mafPipeDataSliceSurface.h"
#include "mafPipeVisualVTKSurface.h"
#include "mafPipeVisualVTKIsoSurface.h"
#include "mafPipeVisualVTKMIPVolume.h"
#include "mafPipeVisualVTKSelection.h"
#include "mafPipeVisualVTKBox.h"
#include "mafPipeVisualVTKSliceSurface.h"

#include "mafToolVTKAxes.h"

#include "mafViewVTK.h"
#include "mafSceneNodeVTK.h"

#include "mafOperationParametricSurface.h"
#include "mafOperationTransformVTK.h"
#include "mafOperationVTKAddLandmark.h"
#include "mafImporterVTK.h"
#include "mafExporterVTK.h"


#include "mafExternalDataCodecVTK.h"

#include "mafVTKParametricSurfaceSphere.h"
#include "mafVTKParametricSurfaceCube.h"
#include "mafVTKParametricSurfaceCone.h"
#include "mafVTKParametricSurfaceCylinder.h"
#include "mafVTKParametricSurfaceEllipsoid.h"
#include "mafDataBoundaryAlgorithmVTK.h"

#include <mafPluginConfigurator.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;
using namespace mafResources;

mafPluginRegistrator::mafPluginRegistrator() {
    // Register to the mafObjectFactory the plug-in object's types.
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeDataImageThreshold);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeDataSliceSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKMIPVolume);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKIsoSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSelection);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKBox);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSliceSurface);

    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafOperationParametricSurface);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafOperationTransformVTK);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafOperationVTKAddLandmark);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafImporterVTK);
    mafRegisterObjectAndAcceptBind(mafPluginVTK::mafExporterVTK);

    mafRegisterObject(mafPluginVTK::mafViewVTK);
    mafRegisterObject(mafPluginVTK::mafSceneNodeVTK);
    mafRegisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCube);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCone);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceCylinder);
    mafRegisterObject(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);
    mafRegisterObject(mafPluginVTK::mafDataBoundaryAlgorithmVTK);

    mafRegisterObject(mafPluginVTK::mafToolVTKAxes);
}

mafPluginRegistrator::~mafPluginRegistrator() {
    // When the library is Un-Loaded it has to remove from the mafObjectFactory its object's types.
    //mafUnregisterObject(mafPluginVTK::mafPipeDataSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeDataImageThreshold);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeDataSliceSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKIsoSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKMIPVolume);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKSelection);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKBox);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafPipeVisualVTKSliceSurface);

    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafOperationParametricSurface);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafOperationTransformVTK);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafOperationVTKAddLandmark);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafImporterVTK);
    mafUnregisterObjectAndAcceptUnbind(mafPluginVTK::mafExporterVTK);

    mafUnregisterObject(mafPluginVTK::mafViewVTK);
    mafUnregisterObject(mafPluginVTK::mafSceneNodeVTK);
    mafUnregisterObject(mafPluginVTK::mafExternalDataCodecVTK);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceSphere);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCube);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCone);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceCylinder);
    mafUnregisterObject(mafPluginVTK::mafVTKParametricSurfaceEllipsoid);
    mafUnregisterObject(mafPluginVTK::mafDataBoundaryAlgorithmVTK);
    
    mafUnregisterObject(mafPluginVTK::mafToolVTKAxes);
}

void mafPluginRegistrator::registerAllObjects() {
    mafPluggedObjectsHash pluginHash;

    mafPluggedObjectInformation dataPipeImageThreshold("Data pipe Image Threshold", "mafPluginVTK::mafPipeDataImageThreshold");
    mafPluggedObjectInformation dataPipeSliceSurface("Data pipe slice surface", "mafPluginVTK::mafPipeDataSliceSurface");
    
    mafPluggedObjectInformation visualPipeVTKSurface("Visual pipe VTK Surface", "mafPluginVTK::mafPipeVisualVTKSurface");
    mafPluggedObjectInformation visualPipeVTKIsoSurface("Visual pipe VTK Iso Surface", "mafPluginVTK::mafPipeVisualVTKIsoSurface");
    mafPluggedObjectInformation visualPipeVTKMIPVolume("Visual pipe VTK MIP Volume", "mafPluginVTK::mafPipeVisualVTKMIPVolume");
    mafPluggedObjectInformation visualPipeSelection("Visual pipe used to represent selection of data", "mafPluginVTK::mafPipeVisualVTKSelection");
    mafPluggedObjectInformation visualPipeBox("Visual pipe used to represent data bounding box", "mafPluginVTK::mafPipeVisualVTKBox");
    mafPluggedObjectInformation visualPipeSliceSurface("Visual pipe used to slice a vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSliceSurface");
    
    mafPluggedObjectInformation toolAxes("Tool used to represent reference system", "mafPluginVTK::mafToolVTKAxes");

    mafPluggedObjectInformation viewVTK("VTK view", "mafPluginVTK::mafViewVTK");
    
    mafPluggedObjectInformation opParametricSurface("Create Parametric Surface", "mafPluginVTK::mafOperationParametricSurface");
    mafPluggedObjectInformation opTransformVTK("Transform VTK Data", "mafPluginVTK::mafOperationTransformVTK");
    mafPluggedObjectInformation opVTKAddLandmark("Add VTK Landmark", "mafPluginVTK::mafOperationVTKAddLandmark");
    mafPluggedObjectInformation importerVTK("Import VTK data", "mafPluginVTK::mafImporterVTK");
    mafPluggedObjectInformation exporterVTK("Export VTK data", "mafPluginVTK::mafExporterVTK");
    
    mafPluggedObjectInformation externalDataCodecVTK("VTK codec", "mafPluginVTK::mafExternalDataCodecVTK");
    
    mafPluggedObjectInformation dataBoundaryAlgorithmVTK("VTK BoundaryAlgorithm", "mafPluginVTK::mafDataBoundaryAlgorithmVTK");

    pluginHash.insertMulti("mafResources::mafPipeData", dataPipeImageThreshold);
    pluginHash.insertMulti("mafResources::mafPipeData", dataPipeSliceSurface);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeVTKSurface);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeVTKIsoSurface);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeVTKMIPVolume);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeSelection);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeBox);
    pluginHash.insertMulti("mafResources::mafPipeVisual", visualPipeSliceSurface);
    pluginHash.insertMulti("mafResources::mafTool", toolAxes);
    pluginHash.insertMulti("mafResources::mafView", viewVTK);
    pluginHash.insertMulti("mafResources::mafOperation", opParametricSurface);
    pluginHash.insertMulti("mafResources::mafOperation", opTransformVTK);
    pluginHash.insertMulti("mafResources::mafOperation", opVTKAddLandmark);
    pluginHash.insertMulti("mafResources::mafImporter", importerVTK);
    pluginHash.insertMulti("mafResources::mafExporter", exporterVTK);
    pluginHash.insertMulti("mafCore::mafExternalDataCodec", externalDataCodecVTK);
    pluginHash.insertMulti("mafResources::mafDataBoundaryAlgorithm", dataBoundaryAlgorithmVTK);

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafPluggedObjectsHash, pluginHash));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.registerLibrary", mafEventTypeLocal, &argList);

    //plug codec
    //Load serialization plug-in
    QString encodeType = "XML";
    QString codec = "mafSerialization::mafCodecXML";

    argList.clear();
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);

    encodeType = "VTK";
    codec = "mafPluginVTK::mafExternalDataCodecVTK";

    argList.clear();
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);
}

void mafPluginRegistrator::registerObjects() {
    mafPluginConfigurator configurator;
    if (!configurator.parseConfigurationFile("mafPluginVTK.xml")) {
        registerAllObjects();
        return;
    }
}
