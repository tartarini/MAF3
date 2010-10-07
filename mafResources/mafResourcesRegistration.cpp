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
#include "mafViewManager.h"
#include "mafVMEManager.h"
#include "mafOperationManager.h"
#include "mafPluginManager.h"
#include "mafInteracionManager.h"

using namespace mafCore;
using namespace mafResources;

mafMap<mafString, mafResourcesRegistration::acceptObjectPointer> mafResourcesRegistration::m_BindingMap = mafBindingMap();

mafStringList mafResourcesRegistration::acceptObject(mafCore::mafObject *obj) {
    mafBindingMap::iterator iter = m_BindingMap.begin();
    mafBindingMap::iterator iter_end = m_BindingMap.end();
    mafStringList res_list;
    while(iter != iter_end) {
        if(iter.value()(obj)) {
            res_list.append(iter.key());
        }
        iter++;
    }
    return res_list;
}

void mafResourcesRegistration::unregisterUnbind( const mafString& className ) {
    m_BindingMap.remove(className);
}

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

void mafResources::initializeModule() {
    mafResources::mafResourcesRegistration::registerResourcesObjects();
    mafResources::mafVMEManager::instance();
    mafResources::mafViewManager::instance();
    mafResources::mafOperationManager::instance();
    mafResources::mafPluginManager::instance();
    mafResources::mafInteracionManager::instance();
}

