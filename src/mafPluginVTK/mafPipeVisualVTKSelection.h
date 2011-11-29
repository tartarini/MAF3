/*
 *  mafPipeVisualVTKSelection.h
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
#include "mafPipeVisualVTK.h"

// Foundation Class forwarding list
class vtkOutlineCornerFilter;
class vtkPolyDataMapper;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKSelection
 This class represents visual pipe used to render the boundary of the current selected data.
 This pipe will use the vtkCornerFilter to draw a corner box around the data to indicate that
 it is currently selected.
 */
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKSelection : public mafPipeVisualVTK {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKSelection(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKSelection();
    
private:
    vtkOutlineCornerFilter *m_OutlineCornerFilter; ///< Filter used to generate the outline corners for representing the selected object.
    vtkPolyDataMapper *m_Mapper; ///< Mapper for box.
};

} // namespace mafPluginVTK

#endif // MAFVISUALPIPESELECTION_H
