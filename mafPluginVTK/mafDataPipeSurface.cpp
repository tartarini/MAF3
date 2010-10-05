/*
 *  mafDataPipeSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataPipeSurface.h"

using namespace mafPluginVTK;

mafDataPipeSurface::mafDataPipeSurface(const mafString code_location) : mafDataPipe(code_location) {
}

void mafDataPipeSurface::createPipe() {
    mafMsgDebug("%s", mafTr("plugin pipeline created!!").toAscii().data());
}

void mafDataPipeSurface::updatePipe(double t) {
    mafMsgDebug("%s", mafTr("plugin executed at time %1!!").arg(t).toAscii().data());
}

