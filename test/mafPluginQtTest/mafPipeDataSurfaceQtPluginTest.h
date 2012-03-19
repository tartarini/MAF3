/*
 *  mafPipeDataSurfaceQtPluginTest.h
 *  mafPluginTest
 *
 *  Created by Daniele Giunchi on 30/12/09.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPESURFACEQTPLUGINTEST_H
#define MAFDATAPIPESURFACEQTPLUGINTEST_H

// Includes list
#include "mafPluginQtTestDefinitions.h"
#include <mafPipeData.h>

namespace mafPluginQtTest {

/**
 Class name: mafPipeDataSurfaceQtPluginTest
 This class is an example data pipe that is plugged into the framework as Qt plug-in.
 */
class MAFPLUGINQTTESTSHARED_EXPORT mafPipeDataSurfaceQtPluginTest : public mafResources::mafPipeData {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public:
     /// Object constructor.
    mafPipeDataSurfaceQtPluginTest(const QString code_location = "");

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

protected:
    /*virtual*/ ~mafPipeDataSurfaceQtPluginTest();
};

} // namespace mafPluginTest

#endif // MAFDATAPIPESURFACEPLUGINTEST_H
