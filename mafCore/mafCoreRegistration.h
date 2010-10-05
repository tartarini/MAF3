/*
 *  mafCoreRegistration.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCOREREGISTRATION_H
#define MAFCOREREGISTRATION_H

// Includes list
#include "mafCore_global.h"
#include "mafObjectFactory.h"
#include "mafObject.h"
#include "mafMementoObject.h"
#include "mafLoggerBuffer.h"
#include "mafLoggerFile.h"
#include "mafLoggerConsole.h"
#include "mafMonitorRAM.h"
#include "mafMonitorHardDisk.h"
#include "mafOntology.h"

namespace mafCore {

/**
 Class name: mafCoreRegistration
 This class allows object rigisteration for all the classes present in mafCore module.
 @sa mafObjectFactory
 */
class MAFCORESHARED_EXPORT mafCoreRegistration {
    public:
    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in objectFactory which needs as input the object type to instantiate as string.*/
    static void registerCoreObjects() {
        mafRegisterObject(mafCore::mafObjectBase);
        mafRegisterObject(mafCore::mafObject);
        mafRegisterObject(mafCore::mafMemento);
        mafRegisterObject(mafCore::mafMementoObject);
        mafRegisterObject(mafCore::mafLoggerBuffer);
        mafRegisterObject(mafCore::mafLoggerFile);
        mafRegisterObject(mafCore::mafLoggerConsole);
        mafRegisterObject(mafCore::mafMonitorRAM);
        mafRegisterObject(mafCore::mafMonitorHardDisk);
        mafRegisterObject(mafCore::mafOntology);
    }
};

} // namespace mafCore

#endif // MAFCOREREGISTRATION_H
