/*
 *  mafResourcesRegistration.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafResourcesRegistration.h"

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
    mafRegisterObject(mafResources::mafOperation);
    mafRegisterObject(mafResources::mafView);
    mafRegisterObject(mafResources::mafMementoResource);
    mafRegisterObject(mafResources::mafMementoDataSet);
    mafRegisterObject(mafResources::mafMementoVME);
    mafRegisterObject(mafResources::mafSceneNode);
    mafRegisterObject(mafResources::mafInteractor);
}

