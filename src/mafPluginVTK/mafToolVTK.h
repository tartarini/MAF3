/*
 *  mafToolVTK.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTOOLVTK_H
#define MAFTOOLVTK_H

// Includes list
#include "mafPluginVTK_global.h"
#include <mafTool.h>

class vtkRenderer;
class vtkProp3D;
class vtkInteractorObserver;

namespace mafPluginVTK {

/**
 Class name: mafToolVTK
 This is the base class for tools based on VTK library.
 */
class MAFPLUGINVTKSHARED_EXPORT mafToolVTK : public mafResources::mafTool {
    Q_OBJECT
    
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafTool);

public:
     /// Object constructor.
    mafToolVTK(const QString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTK();

    /// Called when a new graphic object has been set.
    /** This method has to be re-defined to update stuff linked to the graphic object.*/
    /*virtual*/ void updatedGraphicObject();

	/// Add a vtkProp3D to the tool layer.
	virtual void addProp(vtkProp3D *prop);
	
	/// Remove a vtkProp3D from the tool layer.
	virtual void removeProp(vtkProp3D *prop);
	
	/// Add a VTK widget to the tool layer.
	virtual void addWidget(vtkInteractorObserver *w);
	
	/// Remove a VTK widget from the tool layer.
	virtual void removeWidget(vtkInteractorObserver *w);

private:
    vtkRenderer *m_RendererTool; ///< Renderer used to place tools.
};

} //namespace mafResources

#endif // MAFTOOL_H
