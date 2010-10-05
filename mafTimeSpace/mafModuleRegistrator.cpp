/*
 *  mafModuleRegistration.cpp
 *  mafTimeSpace
 *
 *  Created by Daniele Giunchi on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafModuleRegistrator.h"

void mafTimeSpace::initializeModule() {
    mafTimeSpace::mafTimeManager::instance();
}

