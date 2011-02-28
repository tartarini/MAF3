/*
 *  mafTimeSpaceDefinitions.h
 *  mafTimeSpace
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTIMESPACEDEFINITIONS_H
#define MAFTIMESPACEDEFINITIONS_H

// Includes list
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>


namespace mafTimeSpace {

// Class forwarding list
class mafTimer;
class mafThreadedTimer;

///< Enum that identify the mafTimer's types: One shot or cyclic.
typedef enum {
    mafTimerTypeOneShot,
    mafTimerTypeCyclic
} mafTimerType;

typedef QHash<mafCore::mafId, mafTimeSpace::mafThreadedTimer *> mafTimerHash; ///< Hash table that store the association between the mafId of the crearted timer and its instance.

}

#endif // MAFTIMESPACEDEFINITIONS_H
