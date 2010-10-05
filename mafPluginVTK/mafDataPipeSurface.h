/*
 *  mafDataPipeSurface.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATAPIPESURFACE_H
#define MAFDATAPIPESURFACE_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafDataPipe.h>

namespace mafPluginVTK {

/**
 Class name: mafDataPipeSurface
 This class is an example data pipe that is plugged into the framework as plug-in.
 */
class MAFPLUGINVTKSHARED_EXPORT mafDataPipeSurface : public mafResources::mafDataPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataPipe);

public:
     /// Object constructor.
    mafDataPipeSurface(const mafString code_location = "");

    /// Initialize ans create the pipeline
    /*virtual*/ void createPipe();

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);
};

} // namespace mafPluginVTK

#endif // MAFDATAPIPESURFACE_H
