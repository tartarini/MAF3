/*
 *  mafPipeDataSurfacePluginTest.cpp
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 14/12/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSurfacePluginTest.h"

using namespace mafPluginTest;

mafPipeDataSurfacePluginTest::mafPipeDataSurfacePluginTest(const QString code_location) : mafPipeData(code_location) {
    QByteArray ba = mafTr("plugin pipeline created!!").toLatin1();
    qDebug("%s", ba.data());
}

mafPipeDataSurfacePluginTest::~mafPipeDataSurfacePluginTest() {
}

void mafPipeDataSurfacePluginTest::updatePipe(double t) {
    QByteArray ba = mafTr("plugin executed at time %1!!").arg(t).toLatin1();
    qDebug("%s", ba.data());
    Superclass::updatePipe(t);
}
