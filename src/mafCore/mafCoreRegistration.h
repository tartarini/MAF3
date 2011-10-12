/*
 *  mafCoreRegistration.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCOREREGISTRATION_H
#define MAFCOREREGISTRATION_H

// Includes list
#include "mafCore_global.h"

#define mafRegisterObjectAndAcceptBind(maf_object_type) \
    mafRegisterObject(maf_object_type);\
    mafCore::mafCoreRegistration::registerBind<maf_object_type>(#maf_object_type);

#define mafUnregisterObjectAndAcceptUnbind(maf_object_type) \
    mafUnregisterObject(maf_object_type);\
    mafCore::mafCoreRegistration::unregisterUnbind(#maf_object_type);


namespace mafCore {

/**
 Class name: mafCoreRegistration
 This class allows object reregistration for all the classes present in mafCore module.
 @sa mafObjectFactory
 */
class MAFCORESHARED_EXPORT mafCoreRegistration {
    public:
    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in objectFactory which needs as input the object type to instantiate as string.*/
    static void registerCoreObjects();

    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in objectFactory which needs as input the object type to instantiate as string.*/
    static void unregisterCoreObjects();

    /// Register bind function for the acceptObject static method.
    template<typename T> static void registerBind(const QString &className);

    /// Unregister and unbind function for the acceptObject static method.
    static void unregisterUnbind(const QString &className);

    /// Check the given object with the registered accept functions.
    static QStringList acceptObject(mafObjectBase *obj);

private:
    typedef bool (*acceptObjectPointer)(mafObjectBase *);
    typedef QMap<QString, acceptObjectPointer> mafBindingMap;
    static mafBindingMap m_BindingMap; ///< Map containing the pointers to the accept registered functions.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

template <typename T>
void mafCoreRegistration::registerBind( const QString& className ) {
    if (!m_BindingMap.contains(className)) {
        m_BindingMap.insert(className, &T::acceptObject);
    }
}

} // namespace mafCore

#endif // MAFCOREREGISTRATION_H
