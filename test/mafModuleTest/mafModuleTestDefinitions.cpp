/*
 *  mafModuleTestDefinitions.cpp
 *  mafModuleTest
 *
 *  Created by Paolo Quadrani on 27/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafModuleTestDefinitions.h"
#include <QDebug>

void mafModuleTest::initializeModule() {
    qDebug() << "Module initialized...";
}

void mafModuleTest::shutdownModule() {
    qDebug() << "Module shutdown...";
}
