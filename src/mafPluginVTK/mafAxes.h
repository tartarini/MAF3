/*
 *  mafAxes.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 30/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFAXES_H
#define MAFAXES_H

#include "mafPluginVTKDefinitions.h"

//----------------------------------------------------------------------------
// forward declarations :
//----------------------------------------------------------------------------
class mafResources::mafVME;
class vtkRenderer;
class vtkCoordinate;
class vtkActor2D;
class vtkMatrix4x4;

namespace mafPluginVTK {

/**
Class name: mafAxes
create a vtkAxes and insert it in the passed renderer as a vtkActor2D

If a VME is provided, the axes will represent 
the local VME-reference system.

If VME is NULL, the axes will represent the global
reference system
*/
class MAFPLUGINVTKSHARED_EXPORT mafAxes {
public:
    mafAxes(vtkRenderer *ren, mafResources::mafVME* vme = NULL);
    virtual ~mafAxes();
	
    /** Set the axes visibility. */
    void setVisibility(bool show);

    /** force the position of the Axes, if omitted the ABS VME matrix is used instead. Work only if VME was specified */
    void setPose(vtkMatrix4x4 *abs_pose_matrix = NULL);

private:
    mafResources::mafVME *m_Vme;
    vtkRenderer   *m_Renderer;
	vtkCoordinate *m_Coord;
	vtkActor2D    *m_AxesActor;
};

} // namespace mafPluginVTK

#endif // MAFAXES_H
