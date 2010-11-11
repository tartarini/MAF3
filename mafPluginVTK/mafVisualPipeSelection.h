/*
 *  mafVisualPipeSelection.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPESELECTION_H
#define MAFVISUALPIPESELECTION_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafVisualPipe.h>

// Foundation Class forwarding list
class vtkActor;
class vtkOutlineCornerFilter;

namespace mafPluginVTK {

/**
 Class name: mafVisualPipeSelection
 This class represents visual pipe used to render the boundary of the current selected data.
 This pipe will use the vtkCornerFilter to draw a corner box around the data to indicate that
 it is currently selected.
 */
class MAFPLUGINVTKSHARED_EXPORT mafVisualPipeSelection : public mafResources::mafVisualPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafVisualPipe);

public:
    /// Object constructor;
    mafVisualPipeSelection(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafVisualPipeSelection();

    /// Accept function
    static bool acceptObject(mafCore::mafObject *obj);

    /// Initialize and create the pipeline.
    /*virtual*/ void createPipe();

public slots:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);

private:
    mafCore::mafContainer<vtkActor> m_Actor; ///< Output container.
    vtkOutlineCornerFilter *m_OutlineCornerFilter; ///< Filter used to generate the outline corners for representing the selected object.
};

} // namespace mafPluginVTK

#endif // MAFVISUALPIPESELECTION_H
