/*
 *  mafResourcesRegistration.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCESREGISTRATION_H
#define MAFRESOURCESREGISTRATION_H

// Includes list
#include "mafVME.h"
#include "mafInterpolatorBefore.h"
#include "mafInterpolatorNearest.h"
#include "mafDataSet.h"
#include "mafDataSetCollection.h"
#include "mafOperation.h"
#include "mafView.h"
#include "mafSceneNode.h"
#include "mafHierarchy.h"
#include "mafMementoResource.h"
#include "mafMementoDataSet.h"
#include "mafMementoVME.h"
#include "mafInteractor.h"

#define mafRegisterObjectAndAcceptBind(maf_object_type) \
    mafRegisterObject(maf_object_type);\
    mafResources::mafResourcesRegistration::registerBind<maf_object_type>(#maf_object_type);

#define mafUnregisterObjectAndAcceptUnbind(maf_object_type) \
    mafUnregisterObject(maf_object_type);\
    mafResources::mafResourcesRegistration::unregisterUnbind(#maf_object_type);


namespace mafResources {

/**
 Class name: mafResourcesRegistration
 This class allows object registration for all the classes present in mafResources module.
 @sa mafObjectFactory mafCoreRegistration mafSerializationRegistration mafGUIRegistration
 */
class mafResourcesRegistration {
    public:
    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in objectFactory which needs as input the object type to instantiate as string.*/
    MAFRESOURCESSHARED_EXPORT static void registerResourcesObjects();

    /// Register bind function for the acceptObject static method.
    template<typename T> static void registerBind(const mafString &className);

    /// Unregister and unbind function for the acceptObject static method.
    MAFRESOURCESSHARED_EXPORT static void unregisterUnbind(const mafString &className);

    /// Check the given object with the registered accept functions.
    MAFRESOURCESSHARED_EXPORT static mafStringList acceptObject(mafCore::mafObject *obj);

private:
    typedef bool (*acceptObjectPointer)(mafCore::mafObject *);
    typedef mafMap<mafString, acceptObjectPointer> mafBindingMap;
    MAFRESOURCESSHARED_EXPORT static mafBindingMap m_BindingMap; ///< Map containing the pointers to the accept registered functions.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

template <typename T>
void mafResourcesRegistration::registerBind( const mafString& className ) {
    if (!m_BindingMap.contains(className)) {
        m_BindingMap.insert(className, &T::acceptObject);
    }
}

extern "C" {
    /// Initialize the module by instantiating the singletons and register all the classes with the mafObjectFactory.
    MAFRESOURCESSHARED_EXPORT void initializeModule();
}

} // mafResources

#endif // MAFRESOURCESREGISTRATION_H
