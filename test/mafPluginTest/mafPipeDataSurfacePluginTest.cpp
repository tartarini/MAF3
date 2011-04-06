/*
 *  mafPipeDataSurfacePluginTest.cpp
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSurfacePluginTest.h"

using namespace mafPluginTest;

mafPipeDataSurfacePluginTest::mafPipeDataSurfacePluginTest(const QString code_location) : mafPipeData(code_location) {
}

mafPipeDataSurfacePluginTest::~mafPipeDataSurfacePluginTest() {
}

void mafPipeDataSurfacePluginTest::createPipe() {
    qDebug("%s", mafTr("plugin pipeline created!!").toAscii().data());
}

void mafPipeDataSurfacePluginTest::updatePipe(double t) {
    qDebug("%s", mafTr("plugin executed at time %1!!").arg(t).toAscii().data());
}

