/*
 *  mafPipeVisualVTKBox.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 13/07/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISUALPIPEVTKBOX_H
#define MAFVISUALPIPEVTKBOX_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafPipeVisualVTK.h"

// Foundation Class forwarding list
class vtkOutlineFilter;
class vtkPolyDataMapper;

namespace mafPluginVTK {

/**
 Class name: mafPipeVisualVTKBox
 This class represents visual pipe used to render the boundary of the current data.
 This pipe will use the vtkCornerFilter to draw a box around the data to show its boundary.
 */
class MAFPLUGINVTKSHARED_EXPORT mafPipeVisualVTKBox : public mafPipeVisualVTK {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafPipeVisualVTK);

public:
    /// Object constructor;
    mafPipeVisualVTKBox(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);
    
protected:
    /// Object destructor.
    /* virtual */ ~mafPipeVisualVTKBox();
    
private:
    vtkOutlineFilter *m_OutlineFilter; ///< Filter used to generate the outline box for input data object.
    vtkPolyDataMapper *m_Mapper; ///< Mapper for the box.
};

} // namespace mafPluginVTK

#endif // MAFVISUALPIPEVTKBOX
