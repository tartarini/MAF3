/*
 *  mafCoreSingletons.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCORESINGLETONS_H
#define MAFCORESINGLETONS_H

#include "mafCore_global.h"

#include "mafIdProvider.h"
#include "mafObjectRegistry.h"
#include "mafObjectFactory.h"
#include "mafMessageHandler.h"

namespace mafCore {

/**
 Class name: mafCoreSingletons
 This class allows to initialize and shutdown all the singletons present in mafCore module.
 */
class MAFCORESHARED_EXPORT mafCoreSingletons {
    public:
    /// Initialize all the singletones present in the mafCore module.
    static void mafSingletonsInitialize();

    /// Shutdown all the singletones and destroy the unique instance.
    static void mafSingletonsShutdown();
};

} // namespace mafCore

extern "C" {
    /// Given the library name, it will load the library and will call the 'initModule' global function.
    /** This function will load the library name given as parameter, will search for the inner method 'initModule'
    and will call it so to initialize the module's singletones and register defined objects into the mafObjectFactory.*/
    MAFCORESHARED_EXPORT mafLibrary *mafInitializeModule(mafString module_library);

    /// Given the library handler, it will call the shutdown of the singletones defined in it and unload the library.
    MAFCORESHARED_EXPORT bool mafShutdownModule(mafLibrary *libraryHandler);
}

#endif // MAFCORESINGLETONS_H
