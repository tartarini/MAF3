/*
 *  mafDataPipeSurfacePluginTest.cpp
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataPipeSurfacePluginTest.h"

using namespace mafPluginTest;

mafDataPipeSurfacePluginTest::mafDataPipeSurfacePluginTest(const QString code_location) : mafDataPipe(code_location) {
}

mafDataPipeSurfacePluginTest::~mafDataPipeSurfacePluginTest() {
}

void mafDataPipeSurfacePluginTest::createPipe() {
    qDebug("%s", mafTr("plugin pipeline created!!").toAscii().data());
}

void mafDataPipeSurfacePluginTest::updatePipe(double t) {
    qDebug("%s", mafTr("plugin executed at time %1!!").arg(t).toAscii().data());
}

