/*
 *  mafPipeVisualSelection.h
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
#include <mafPipeVisual.h>

// Foundation Class forwarding list
class vtkActor;
class vtkOutlineCornerFilter;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualSelection
 This class represents visual pipe used to render the boundary of the current selected data.
 This pipe will use the vtkCornerFilter to draw a corner box around the data to indicate that
 it is currently selected.
 */
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualSelection : public mafResources::mafPipeVisual {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
    /// Object constructor;
    mafPipeVisualSelection(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);

    /// Initialize and create the pipeline.
    /*virtual*/ void createPipe();

public slots:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);

protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualSelection();

private:
    mafCore::mafContainer<vtkActor> m_Actor; ///< Output container.
    vtkOutlineCornerFilter *m_OutlineCornerFilter; ///< Filter used to generate the outline corners for representing the selected object.
};

} // namespace mafPluginVTK

#endif // MAFVISUALPIPESELECTION_H
