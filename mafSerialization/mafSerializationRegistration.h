/*
 *  mafSerializationRegistrations.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZATIONREGISTRATION_H
#define MAFSERIALIZATIONREGISTRATION_H

// Includes list
#include "mafSerialization_global.h"
#include "mafCodecRawBinary.h"
#include "mafCodecRawASCII.h"
#include "mafCodecVolume.h"
#include "mafCodecBrickedVolume.h"
#include "mafCodecXML.h"
#include "mafSerializerFileSystem.h"
//#include "mafCSVReader.h"

namespace mafSerialization {

/**
 Class name: mafSerializationRegistration
 This class allows object rigisteration for all the classes present in mafSerialization module.
 */
class MAFSERIALIZATIONSHARED_EXPORT mafSerializationRegistration {
    public:
    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in objectFactory which needs as input the object type to instantiate as string.*/
    MAFSERIALIZATIONSHARED_EXPORT static void registerSerializationObjects();

    /// Unregister all the concrete objects from mafObjectFactory.
    MAFSERIALIZATIONSHARED_EXPORT static void unregisterSerializationObjects();
};

extern "C" {
    /// Initialize the module by instantiating the singletons and register all the classes with the mafObjectFactory.
    MAFSERIALIZATIONSHARED_EXPORT void initializeModule();

    /// Shutdown the module by shuting down the singletons and un-register all the classes from the mafObjectFactory.
    MAFSERIALIZATIONSHARED_EXPORT void shutdownModule();
}

} // mafSerialization

#endif // MAFSERIALIZATIONREGISTRATION_H
