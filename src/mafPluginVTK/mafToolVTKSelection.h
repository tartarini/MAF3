/*
 *  mafToolVTKSelection.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTKSELECTION_H
#define MAFTOOLVTKSELECTION_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include "mafToolVTK.h"

// Foundation Class forwarding list
class vtkOutlineCornerFilter;
class vtkPolyDataMapper;
class vtkActor;

namespace mafPluginVTK {

/**
 Class name: mafToolVTKSelection
 This class represents tool used to render the boundary of the current selected data.
 This tool will use the vtkCornerFilter to draw a corner box around the data to indicate that
 it is currently selected.
 */
class MAFPLUGINVTKSHARED_EXPORT mafToolVTKSelection : public mafToolVTK {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafToolVTK);

public Q_SLOTS:
    /// Called when the graphic object has been initialized ready to use
    /*virtual*/ void graphicObjectInitialized();

    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1);

public:
    /// Object constructor;
    mafToolVTKSelection(const QString code_location = "");

    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// Allow to take the tool to the initial conditions.
    /*virtual*/ void resetTool();

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTKSelection();
    
private:
    vtkOutlineCornerFilter *m_OutlineCornerFilter; ///< Filter used to generate the outline corners for representing the selected object.
    vtkPolyDataMapper *m_Mapper; ///< Mapper for box.
    vtkActor *m_Actor; ///< Actor representing the box.
};

} // namespace mafPluginVTK

#endif // MAFTOOLVTKSELECTION_H
