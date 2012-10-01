/*
 *  mafToolVTK.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 27/09/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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

public Q_SLOTS:
    /// Called when a new graphic object has been set.
    /** This method has to be re-defined to update stuff linked to the graphic object.*/
    /*virtual*/ void updatedGraphicObject();

public:
     /// Object constructor.
    mafToolVTK(const QString code_location = "");

    /// Assign the scene node to the tool so to have access to the VME and eventually its visual representation.
    /*virtual*/ void setSceneNode(mafResources::mafSceneNode *node);

protected:
    /// Object destructor.
    /* virtual */ ~mafToolVTK();

	/// Add a vtkProp3D to the tool layer.
	virtual void addProp(vtkProp3D *prop, bool inAssembly = false);
	
	/// Remove a vtkProp3D from the tool layer.
	virtual void removeProp(vtkProp3D *prop, bool inAssembly = false);
	
	/// Add a VTK widget to the tool layer.
	virtual void addWidget(vtkInteractorObserver *w);
	
	/// Remove a VTK widget from the tool layer.
	virtual void removeWidget(vtkInteractorObserver *w); 

    /// update visibility for actor or volume passed as parameter
    /*virtual*/ void updateVisibility();

private:
    vtkRenderer *m_RendererTool; ///< Renderer used to place tools.
    QList<vtkProp3D *> m_PropList; ///< List of vtkProp3D associated with the tool.
    QList<vtkInteractorObserver *> m_WidgetList; ///< List of widgets associated with the tool.
};

} //namespace mafResources

#endif // MAFTOOL_H
