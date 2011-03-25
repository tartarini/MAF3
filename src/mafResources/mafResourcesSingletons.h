/*
 *  mafResourcesSingletons.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFRESOURCESSINGLETONS_H
#define MAFRESOURCESSINGLETONS_H

#include "mafResources_global.h"
#include "mafResourcesRegistration.h"
#include "mafPluginManager.h"
#include "mafViewManager.h"
#include "mafVMEManager.h"
#include "mafOperationManager.h"
#include "mafInteractionManager.h"

namespace mafResources {

/**
 Class name: mafResourcesSingletons
 This class allows to initialize and shutdown all the singletons present in mafResources module.
 */
class mafResourcesSingletons {
    public:
    /// Initialize all the singletons present in the mafResources module.
    MAFRESOURCESSHARED_EXPORT static void mafSingletonsInitialize();

    /// Shutdown all the singletons and destroy the unique instance.
    MAFRESOURCESSHARED_EXPORT static void mafSingletonsShutdown();
};

extern "C" {
    /// Initialize the module by instantiating the singletons and register all the classes with the mafObjectFactory.
    MAFRESOURCESSHARED_EXPORT void initializeModule();

    /// Shutdown the module by shuting down the singletons and un-register all the classes from the mafObjectFactory.
    MAFRESOURCESSHARED_EXPORT void shutdownModule();
}

} // namespace mafResources

#endif // MAFRESOURCESSINGLETONS_H
