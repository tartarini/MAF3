/*
 *  mafSerializationRegistrations.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSerializationRegistration.h"

#include "mafSerializationManager.h"

using namespace mafSerialization;

void mafSerialization::initializeModule() {
    mafSerializationRegistration::registerSerializationObjects();
    mafSerialization::mafSerializationManager::instance();
}

