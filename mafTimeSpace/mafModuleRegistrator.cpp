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
#include "mafTimer.h"
#include "mafThreadedTimer.h"

void mafTimeSpace::initializeModule() {
    mafRegisterObject(mafTimeSpace::mafTimer);
    mafRegisterObject(mafTimeSpace::mafThreadedTimer);
    mafTimeSpace::mafTimeManager::instance();
}

void mafTimeSpace::shutdownModule() {
    mafTimeSpace::mafTimeManager::instance()->shutdown();;
    mafUnregisterObject(mafTimeSpace::mafTimer);
    mafUnregisterObject(mafTimeSpace::mafThreadedTimer);
}
