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

#define mafRegisterObjectAndAcceptBind(maf_object_type) \
    mafRegisterObject(maf_object_type);\
    mafCore::mafCoreRegistration::registerBind<maf_object_type>(#maf_object_type);

#define mafUnregisterObjectAndAcceptUnbind(maf_object_type) \
    mafUnregisterObject(maf_object_type);\
    mafCore::mafCoreRegistration::unregisterUnbind(#maf_object_type);


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

    /// Register bind function for the acceptObject static method.
    template<typename T> static void registerBind(const mafString &className);

    /// Unregister and unbind function for the acceptObject static method.
    static void unregisterUnbind(const mafString &className);

    /// Check the given object with the registered accept functions.
    static mafStringList acceptObject(mafObjectBase *obj);

private:
    typedef bool (*acceptObjectPointer)(mafObjectBase *);
    typedef mafMap<mafString, acceptObjectPointer> mafBindingMap;
    static mafBindingMap m_BindingMap; ///< Map containing the pointers to the accept registered functions.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

template <typename T>
void mafCoreRegistration::registerBind( const mafString& className ) {
    if (!m_BindingMap.contains(className)) {
        m_BindingMap.insert(className, &T::acceptObject);
    }
}

} // namespace mafCore

#endif // MAFCOREREGISTRATION_H
