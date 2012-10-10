/*
 *  mafPipeDataSurfacePluginTest.h
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPESURFACEPLUGINTEST_H
#define MAFDATAPIPESURFACEPLUGINTEST_H

// Includes list
#include "mafPluginTestDefinitions.h"
#include <mafPipeData.h>

namespace mafPluginTest {

/**
 Class name: mafPipeDataSurfacePluginTest
 This class is an example data pipe that is plugged into the framework as plug-in.
 */
class MAFPLUGINTESTSHARED_EXPORT mafPipeDataSurfacePluginTest : public mafResources::mafPipeData {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeData);

public:
     /// Object constructor.
    mafPipeDataSurfacePluginTest(const QString code_location = "");

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

protected:
    /*virtual*/ ~mafPipeDataSurfacePluginTest();
};

} // namespace mafPluginTest

#endif // MAFDATAPIPESURFACEPLUGINTEST_H
