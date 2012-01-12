/*
 *  mafToolVTKAxes.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTKAXES_H
#define MAFTOOLVTKAXES_H

// Includes list
#include "mafPluginVTK_global.h"
#include "mafToolVTK.h"

class vtkAxesActor;

namespace mafPluginVTK {

/**
 Class name: mafToolVTKAxes
 This is the tool representing a Cartesian axes.
 */
class MAFPLUGINVTKSHARED_EXPORT mafToolVTKAxes : public mafToolVTK {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

public:
     /// Object constructor.
    mafToolVTKAxes(const QString code_location = "");

    /// Allow to take the tool to the initial conditions.
    /*virtual*/ void resetTool();

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKAxes();

private:
    vtkAxesActor *m_AxesActor; ///< Axes to show the reference system of the node.
};

} //namespace mafPluginVTK

#endif // MAFTOOLVTKAXES_H
