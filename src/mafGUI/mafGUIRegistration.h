/*
 *  mafGUIRegistration.h
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFGUIREGISTRATION_H
#define MAFGUIREGISTRATION_H

#include "mafGUIDefinitions.h"

namespace mafGUI {

/**
 Class name: mafGUIRegistration
 This class allows object rigisteration for all the classes present in mafGUI module.
 @sa mafObjectFactory mafCoreRegistration mafResourcesRegistration mafSerializationRegistration
 */
class mafGUIRegistration {
    public:
    /// Register all the concrete objects that can be instantiated by the mafObjectFactory.
    /** This registration is needed to instantiate object by using the mafNEWFromString macro present in
    objectFactory which needs as input the object type to instantiate as string.*/
    MAFGUISHARED_EXPORT static void registerGUIObjects();
};

} // namespace mafGUI

#endif // MAFGUIREGISTRATION_H
