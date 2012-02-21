/*
 *  mafAnimateVTK.h
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 21/02/12.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFANIMATEVTK_H
#define MAFANIMATEVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafVTKWidget.h>

namespace mafPluginVTK {

/**
Class name: mafAnimateVTK
This is an utility class to animate VTKCamera.
*/

class MAFPLUGINVTKSHARED_EXPORT mafAnimateVTK : public mafCore::mafObjectBase
{
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafAnimateVTK(const QString code_location = "");

    /// Animate the camera to zoom on the passed bounding box.
    void flyTo(mafVTKWidget *widget, double bounds[6]);


protected:
    /// Object destructor.
    /* virtual */ ~mafAnimateVTK();


};

}

#endif // MAFANIMATEVTK_H
