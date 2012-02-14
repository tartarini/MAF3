/*
 *  mafCoreRegistration.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreRegistration.h"
#include "mafObjectFactory.h"
#include "mafObject.h"
#include "mafSQLITE.h"
#include "mafMementoObject.h"
#include "mafLoggerBuffer.h"
#include "mafLoggerFile.h"
#include "mafLoggerConsole.h"
#include "mafLoggerSQLITE.h"
#include "mafMonitorRAM.h"
#include "mafMonitorHardDisk.h"
#include "mafHierarchy.h"
#include "mafMementoHierarchy.h"
#include "mafProxyInterface.h"
// #include "mafVisitorFindObjectsByHashCode.h"
#include "mafVisitorBounds.h"
#include "mafBounds.h"
#include "mafPoint.h"

using namespace mafCore;

QMap<QString, mafCoreRegistration::acceptObjectPointer> mafCoreRegistration::m_BindingMap = mafBindingMap();

QStringList mafCoreRegistration::acceptObject(mafCore::mafObjectBase *obj) {
    mafBindingMap::iterator iter = m_BindingMap.begin();
    mafBindingMap::iterator iter_end = m_BindingMap.end();
    QStringList res_list;
    while(iter != iter_end) {
        if(iter.value()(obj)) {
            res_list.append(iter.key());
        }
        ++iter;
    }
    return res_list;
}

void mafCoreRegistration::unregisterUnbind( const QString& className ) {
    m_BindingMap.remove(className);
}

void mafCoreRegistration::registerCoreObjects() {
    mafRegisterObject(mafCore::mafObjectBase);
    mafRegisterObject(mafCore::mafObject);
    mafRegisterObject(mafCore::mafSQLITE);
    mafRegisterObject(mafCore::mafMemento);
    mafRegisterObject(mafCore::mafMementoObject);
    mafRegisterObject(mafCore::mafLoggerBuffer);
    mafRegisterObject(mafCore::mafLoggerFile);
    mafRegisterObject(mafCore::mafLoggerConsole);
    mafRegisterObject(mafCore::mafLoggerSQLITE);
    mafRegisterObject(mafCore::mafMonitorRAM);
    mafRegisterObject(mafCore::mafMonitorHardDisk);
    mafRegisterObject(mafCore::mafHierarchy);
    mafRegisterObject(mafCore::mafMementoHierarchy);
//     mafRegisterObject(mafCore::mafVisitorFindObjectsByHashCode);
    mafRegisterObject(mafCore::mafVisitorBounds);

    qRegisterMetaType<mafCore::mafHierarchyPointer>("mafCore::mafHierarchyPointer");
    qRegisterMetaType<mafCore::mafProxyInterfacePointer>("mafCore::mafProxyInterfacePointer");
    qRegisterMetaType<mafCore::mafProxyInterface>("mafCore::mafProxyInterface");
    qRegisterMetaType<mafCore::mafProxyInterface>("mafCore::mafDelegatePointer");
    qRegisterMetaType<mafCore::mafPointPointer>("mafCore::mafPointPointer");
    qRegisterMetaType<mafCore::mafBoundsPointer>("mafCore::mafBoundsPointer");
}

void mafCoreRegistration::unregisterCoreObjects() {
    mafUnregisterObject(mafCore::mafObjectBase);
    mafUnregisterObject(mafCore::mafObject);
    mafUnregisterObject(mafCore::mafSQLITE);
    mafUnregisterObject(mafCore::mafMemento);
    mafUnregisterObject(mafCore::mafMementoObject);
    mafUnregisterObject(mafCore::mafLoggerBuffer);
    mafUnregisterObject(mafCore::mafLoggerFile);
    mafUnregisterObject(mafCore::mafLoggerConsole);
    mafUnregisterObject(mafCore::mafLoggerSQLITE);
    mafUnregisterObject(mafCore::mafMonitorRAM);
    mafUnregisterObject(mafCore::mafMonitorHardDisk);
    mafUnregisterObject(mafCore::mafHierarchy);
    mafUnregisterObject(mafCore::mafMementoHierarchy);
//     mafUnregisterObject(mafCore::mafVisitorFindObjectsByHashCode);
    mafUnregisterObject(mafCore::mafVisitorBounds);
}
