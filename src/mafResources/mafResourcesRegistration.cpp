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
#include "mafSceneNode.h"
#include "mafHierarchy.h"
#include "mafMementoResource.h"
#include "mafMementoDataSet.h"
#include "mafMementoDataSetCollection.h"
#include "mafMementoVME.h"
#include "mafInteractor.h"
#include "mafVisitorFindSceneNodeByVMEHash.h"
#include "mafOperationTransform.h"

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
    mafRegisterObject(mafResources::mafMementoResource);
    mafRegisterObject(mafResources::mafMementoDataSet);
    mafRegisterObject(mafResources::mafMementoDataSetCollection);
    mafRegisterObject(mafResources::mafMementoVME);
    mafRegisterObject(mafResources::mafSceneNode);
    mafRegisterObject(mafResources::mafInteractor);
    mafRegisterObject(mafResources::mafVisitorFindSceneNodeByVMEHash);
    mafRegisterObjectAndAcceptBind(mafResources::mafOperationTransform);
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
    mafUnregisterObject(mafResources::mafMementoResource);
    mafUnregisterObject(mafResources::mafMementoDataSet);
    mafUnregisterObject(mafResources::mafMementoDataSetCollection);
    mafUnregisterObject(mafResources::mafMementoVME);
    mafUnregisterObject(mafResources::mafSceneNode);
    mafUnregisterObject(mafResources::mafInteractor);
    mafUnregisterObject(mafResources::mafVisitorFindSceneNodeByVMEHash);
    mafUnregisterObjectAndAcceptUnbind(mafResources::mafOperationTransform);
}
