/*
 *  mafDataPipeSurfacePluginTest.h
 *  mafPluginTest
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPESURFACEPLUGINTEST_H
#define MAFDATAPIPESURFACEPLUGINTEST_H

// Includes list
#include "mafPluginTestDefinitions.h"
#include <mafDataPipe.h>

namespace mafPluginTest {

/**
 Class name: mafDataPipeSurface
 This class is an example data pipe that is plugged into the framework as plug-in.
 */
class MAFPLUGINTESTSHARED_EXPORT mafDataPipeSurfacePluginTest : public mafResources::mafDataPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataPipe);

public:
     /// Object constructor.
    mafDataPipeSurfacePluginTest(const mafString code_location = "");

    /// Initialize ans create the pipeline
    /*virtual*/ void createPipe();

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

protected:
    /*virtual*/ ~mafDataPipeSurfacePluginTest();
};

} // namespace mafPluginTest

#endif // MAFDATAPIPESURFACEPLUGINTEST_H
