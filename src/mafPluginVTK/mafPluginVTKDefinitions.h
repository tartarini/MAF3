/*
 *  mafPipesLibraryDefinitions.h
 *  mafPipesLibrary
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFPLUGINVTKDEFINITIONS_H
#define MAFPLUGINVTKDEFINITIONS_H


// Includes list
#include "mafPluginVTK_global.h"
#include <mafCoreRegistration.h>
#include <mafResourcesRegistration.h>
#include <mafResourcesDefinitions.h>
#include <mafContainer.h>

namespace mafPluginVTK {

///< Enumeration that define the key modifiers values.
enum {
    MAF_SHIFT_KEY = 0,
    MAF_CTRL_KEY,
    MAF_ALT_KEY
};

}


#endif // MAFPLUGINVTKDEFINITIONS_H
