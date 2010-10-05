/*
 *  mafModuleRegistration.h
 *  mafTimeSpace
 *
 *  Created by Daniele Giunchi on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMODULEREGISTRATOR_H
#define MAFMODULEREGISTRATOR_H

#include "mafTimeManager.h"

namespace mafTimeSpace {

extern "C" {
    /// Initialize the module by instantiating the singletons and register all the classes with the mafObjectFactory.
    MAFTIMESPACESHARED_EXPORT void initializeModule();
}

} // namespace mafTimeSpace

#endif // MAFMODULEREGISTRATOR_H
