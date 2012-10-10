/*
 *  mafResourcesRegistration.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCESREGISTRATION_H
#define MAFRESOURCESREGISTRATION_H

// Includes list
#include "mafResources_global.h"

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

    /// Un-Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This method is needed to remove objects from the objectFactory when the mafResources module is dynamically unloaded from the application.*/
    MAFRESOURCESSHARED_EXPORT static void unregisterResourcesObjects();
};

} // namespace mafResources

#endif // MAFRESOURCESREGISTRATION_H
