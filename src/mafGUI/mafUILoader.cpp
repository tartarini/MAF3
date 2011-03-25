/*
 *  mafUILoader.cpp
 *  mafGUI
 *
 *  Created by Daniele Giunchi on 29/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafUILoader.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

mafUILoader::mafUILoader(const QString code_location) : mafObjectBase(code_location) {
    mafIdProvider *provider = mafIdProvider::instance();
    mafId ui_id = provider->idValue("maf.local.gui.uiloaded");
    if(ui_id == -1) {
        provider->createNewId("maf.local.gui.uiloaded");
        mafRegisterLocalSignal("maf.local.gui.uiloaded", this, "uiLoadedSignal(mafCore::mafContainerInterface *)");
    }
}

mafUILoader::~mafUILoader() {
    mafIdProvider *provider = mafIdProvider::instance();
    provider->removeId("maf.local.gui.uiloaded");
}
