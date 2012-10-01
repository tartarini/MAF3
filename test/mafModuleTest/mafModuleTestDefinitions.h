/*
 *  mafModuleTestDefinitions.h
 *  mafModuleTest
 *
 *  Created by Paolo Quadrani on 27/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMODULETESTDEFINITIONS_H
#define MAFMODULETESTDEFINITIONS_H


// Includes list
#include "mafModuleTest_global.h"

namespace mafModuleTest {
    
    extern "C" {
        /// Initialize the module by instantiating the singletons and register all the classes with the mafObjectFactory.
        MAFMODULETESTSHARED_EXPORT void initializeModule();
        
        /// Shutdown the module by shuting down the singletons and un-register all the classes from the mafObjectFactory.
        MAFMODULETESTSHARED_EXPORT void shutdownModule();
    }
    
} // mafModuleTest

#endif // MAFMODULETESTDEFINITIONS_H
