/*
 *  mafResourcesRegistration.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#include <mafCoreRegistration.h>
#include "mafResourcesRegistration.h"

#include "mafVME.h"
#include "mafInterpolatorBefore.h"
#include "mafInterpolatorNearest.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafView.h"
#include "mafViewCompound.h"
#include "mafSceneNode.h"
#include "mafImporter.h"
#include "mafMementoResource.h"
#include "mafMementoDataSet.h"
#include "mafMementoDataSetCollection.h"
#include "mafMementoVME.h"
#include "mafInteractor.h"
#include "mafVisitorFindSceneNodeByVMEHash.h"
#include "mafOperationTransform.h"
#include "mafPipeVisualCompound.h"
#include "mafOperationWorker.h"
#include "mafTool.h"
#include "mafToolHandler.h"
#include "mafPipeDataSlice.h"
#include "mafVisitorBounds.h"

using namespace mafCore;
using namespace mafResources;

void mafResourcesRegistration::registerResourcesObjects() {
    mafRegisterObject(mafResources::mafResource);
    mafRegisterObject(mafResources::mafVME);
    mafRegisterObject(mafResources::mafDataSet);
    mafRegisterObject(mafResources::mafInterpolatorBefore);
    mafRegisterObject(mafResources::mafInterpolatorNearest);
    mafRegisterObject(mafResources::mafDataSet);
    mafRegisterObject(mafResources::mafDataSetCollection);
    mafRegisterObject(mafResources::mafView);
    mafRegisterObject(mafResources::mafViewCompound);
    mafRegisterObject(mafResources::mafMementoResource);
    mafRegisterObject(mafResources::mafMementoDataSet);
    mafRegisterObject(mafResources::mafMementoDataSetCollection);
    mafRegisterObject(mafResources::mafMementoVME);
    mafRegisterObject(mafResources::mafSceneNode);
    mafRegisterObject(mafResources::mafInteractor);
    mafRegisterObject(mafResources::mafVisitorFindSceneNodeByVMEHash);
    mafRegisterObject(mafResources::mafPipeVisualCompound);
    mafRegisterObject(mafResources::mafTool);
    mafRegisterObject(mafResources::mafToolHandler);
    mafRegisterObject(mafResources::mafPipeDataSlice);
    mafRegisterObject(mafResources::mafVisitorBounds);
    mafRegisterObjectAndAcceptBind(mafResources::mafOperationTransform);
    mafRegisterObjectAndAcceptBind(mafResources::mafImporter);


    qRegisterMetaType<mafResources::mafMatrixPointer>("mafResources::mafMatrixPointer");
    qRegisterMetaType<mafResources::mafMatrix>("mafResources::mafMatrix");
    qRegisterMetaType<mafResources::mafResourceWorkerPointer>("mafResources::mafResourceWorkerPointer");
    qRegisterMetaType<mafExecutionPoolPointer>("mafExecutionPoolPointer");
}

void mafResourcesRegistration::unregisterResourcesObjects() {
    mafUnregisterObject(mafResources::mafResource);
    mafUnregisterObject(mafResources::mafVME);
    mafUnregisterObject(mafResources::mafDataSet);
    mafUnregisterObject(mafResources::mafInterpolatorBefore);
    mafUnregisterObject(mafResources::mafInterpolatorNearest);
    mafUnregisterObject(mafResources::mafDataSet);
    mafUnregisterObject(mafResources::mafDataSetCollection);
    mafUnregisterObject(mafResources::mafView);
    mafUnregisterObject(mafResources::mafViewCompound);
    mafUnregisterObject(mafResources::mafMementoResource);
    mafUnregisterObject(mafResources::mafMementoDataSet);
    mafUnregisterObject(mafResources::mafMementoDataSetCollection);
    mafUnregisterObject(mafResources::mafMementoVME);
    mafUnregisterObject(mafResources::mafSceneNode);
    mafUnregisterObject(mafResources::mafInteractor);
    mafUnregisterObject(mafResources::mafVisitorFindSceneNodeByVMEHash);
    mafUnregisterObject(mafResources::mafPipeVisualCompound);
    mafUnregisterObject(mafResources::mafTool);
    mafUnregisterObject(mafResources::mafToolHandler);
    mafUnregisterObject(mafResources::mafPipeDataSlice);
    mafUnregisterObject(mafResources::mafVisitorBounds);
    mafUnregisterObjectAndAcceptUnbind(mafResources::mafOperationTransform);
    mafUnregisterObjectAndAcceptUnbind(mafResources::mafImporter);
}
