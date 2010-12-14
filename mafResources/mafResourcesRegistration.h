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
};

} // namespace mafResources

#endif // MAFRESOURCESREGISTRATION_H
