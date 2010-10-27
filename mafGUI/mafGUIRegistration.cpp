/*
 *  mafGUIRegistration.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUIRegistration.h"
#include <mafObjectFactory.h>

using namespace mafGUI;

void mafGUIRegistration::registerGUIObjects() {
        mafRegisterObject(mafGUI::mafGUIManager);
}

