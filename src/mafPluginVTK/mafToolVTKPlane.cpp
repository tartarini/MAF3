/*
 *  mafToolVTKPlane.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 25/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKPlane.h"
#include "mafSceneNodeVTK.h"

#include <vtkAssembly.h>
#include <vtkPlane.h>
#include <vtkCommand.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkActor.h>


// Callback for the interaction
// This does the actual work: updates the vtkPlane implicit function.
// This in turn causes the pipeline to update and clip the object.
class vtkIPWCallback : public vtkCommand {
public:
	static vtkIPWCallback *New() { return new vtkIPWCallback; }
  	virtual void Execute(vtkObject *caller, unsigned long, void*) {
    	vtkImplicitPlaneWidget2 *planeWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
    	vtkImplicitPlaneRepresentation *rep = reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
    	rep->GetPlane(this->Plane);
    }
  	vtkIPWCallback() : Plane(0), Actor(0) {}
  	vtkPlane *Plane;
  	vtkActor *Actor;
};



using namespace mafPluginVTK;


mafToolVTKPlane::mafToolVTKPlane(const QString code_location) : mafToolVTK(code_location) {
    m_Plane = vtkPlane::New();
}

mafToolVTKPlane::~mafToolVTKPlane() {
    m_Plane->Delete();
}

void mafToolVTKPlane::setSceneNode(mafResources::mafSceneNode *node) {
    vtkAssembly *assembly = NULL;
    if (m_SceneNode) {
        // remove the axes from the old scene node...
        assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
//        assembly->RemovePart(m_AxesActor);
    }
    // initialize the member variable...
    Superclass::setSceneNode(node);

    // ... then add the axes actor to the new assembly...
    if (m_SceneNode) {
        assembly = ((mafSceneNodeVTK *)m_SceneNode)->nodeAssembly();
//        assembly->AddPart(m_AxesActor);
    }
}

void mafToolVTKPlane::graphicObjectInitialized() {
  	vtkSmartPointer<vtkIPWCallback> myCallback = 
    vtkSmartPointer<vtkIPWCallback>::New();
  	myCallback->Plane = m_Plane;
//  	myCallback->Actor = actor;
 
  	vtkSmartPointer<vtkImplicitPlaneRepresentation> rep = 
    vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
  	rep->SetPlaceFactor(1.25); // This must be set prior to placing the widget
//  	rep->PlaceWidget(actor->GetBounds());
//  	rep->SetNormal(plane->GetNormal());
  	rep->SetOrigin(0,0,50); //this doesn't seem to work?
 
  	m_PlaneWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
//  	m_PlaneWidget->SetInteractor(renderWindowInteractor);
  	m_PlaneWidget->SetRepresentation(rep);
  	m_PlaneWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
//    addProp(m_AxesActor);
}

void mafToolVTKPlane::updateVisibility() {
//    m_AxesActor->SetVisibility(visibility() ? 1 : 0);
  	m_PlaneWidget->On();
}
