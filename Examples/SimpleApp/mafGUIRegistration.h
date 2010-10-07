#ifndef MAFGUIREGISTRATION_H
#define MAFGUIREGISTRATION_H

#include <mafCoreDefinitions.h>
#include "mafMementoApplication.h"

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
    static void registerGUIObjects() {
        mafRegisterObject(mafMementoApplication);
    }
};

#endif // MAFGUIREGISTRATION_H
