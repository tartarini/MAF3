/*
 *  mafToolVTKOrthoPlane.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 19/1/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKOrthoPlane.h"
#include <mafSceneNodeVTK.h>

#include <mafPipeDataSlice.h>

#include <vtkCommand.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkMAFImplicitPlaneRepresentation.h>


// Callback for the interaction
// This does the actual work: updates the vtkPlane implicit function.
// This in turn causes the pipeline to update and clip the object.
class vtkIOPWCallback : public vtkCommand {
public:
	static vtkIOPWCallback *New() { return new vtkIOPWCallback; }
  	virtual void Execute(vtkObject *caller, unsigned long, void*);
  	vtkIOPWCallback() : Tool(0) {}
    mafResources::mafTool *Tool;
};

void vtkIOPWCallback::Execute(vtkObject *caller, unsigned long, void*) {
    vtkImplicitPlaneWidget2 *planeWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
    vtkMAFImplicitPlaneRepresentation *rep = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
    double ori[3];
    rep->GetOrigin(ori);
    mafCore::mafPoint *o = new mafCore::mafPoint(ori);
    QVariant vo;
    vo.setValue<mafCore::mafPoint>(*o);
    Tool->setProperty("origin", vo);
    Tool->setModified();
    mafDEL(o);
}

//-----------------------------------------------------------------------------------------------------

using namespace mafPluginVTK;
using namespace mafCore;

mafToolVTKOrthoPlane::mafToolVTKOrthoPlane(const QString code_location) : mafToolVTK(code_location) {
    VTK_CREATE(vtkIOPWCallback, myCallback);
    myCallback->Tool = this;

    double b[6] = {0,1,0,1,0,1};

    m_Origin = new mafPoint(0,0,0);
    m_VOI = new mafBounds(b);

    VTK_CREATE(vtkMAFImplicitPlaneRepresentation, repX);
    repX->SetPlaceFactor(1.25);
    repX->DrawPlaneOff();
    repX->DrawOutlineBoxOff();
    repX->SetRotationEnabled(0);
    repX->OutlineTranslationOff();
    repX->SetOrigin(0,0,0);
    repX->NormalToXAxisOn();

    VTK_CREATE(vtkMAFImplicitPlaneRepresentation, repY);
    repY->SetPlaceFactor(1.25);
    repY->DrawPlaneOff();
    repY->DrawOutlineBoxOff();
    repY->SetRotationEnabled(0);
    repY->OutlineTranslationOff();
    repY->SetOrigin(0,0,0);
    repY->NormalToYAxisOn();

    VTK_CREATE(vtkMAFImplicitPlaneRepresentation, repZ);
    repZ->SetPlaceFactor(1.25);
    repZ->DrawPlaneOff();
    repZ->DrawOutlineBoxOff();
    repZ->SetRotationEnabled(0);
    repZ->OutlineTranslationOff();
    repZ->SetOrigin(0,0,0);
    repZ->NormalToZAxisOn();

    m_PlaneWidgetX = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    m_PlaneWidgetX->SetRepresentation(repX);
    m_PlaneWidgetX->AddObserver(vtkCommand::InteractionEvent,myCallback);

    m_PlaneWidgetY = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    m_PlaneWidgetY->SetRepresentation(repY);
    m_PlaneWidgetY->AddObserver(vtkCommand::InteractionEvent,myCallback);

    m_PlaneWidgetZ = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    m_PlaneWidgetZ->SetRepresentation(repZ);
    m_PlaneWidgetZ->AddObserver(vtkCommand::InteractionEvent,myCallback);
}

mafToolVTKOrthoPlane::~mafToolVTKOrthoPlane() {
    mafDEL(m_Origin);
    mafDEL(m_VOI);
}

void mafToolVTKOrthoPlane::setVOI(mafCore::mafBounds *bounds) {
    double b[6];
    *m_VOI = *bounds;
    b[0] = bounds->xMin();
    b[1] = bounds->xMax();
    b[2] = bounds->yMin();
    b[3] = bounds->yMax();
    b[4] = bounds->zMin();
    b[5] = bounds->zMax();
    vtkMAFImplicitPlaneRepresentation *repX = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidgetX->GetRepresentation());
    repX->PlaceWidget(b);
    vtkMAFImplicitPlaneRepresentation *repY = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidgetY->GetRepresentation());
    repY->PlaceWidget(b);
    vtkMAFImplicitPlaneRepresentation *repZ = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidgetZ->GetRepresentation());
    repZ->PlaceWidget(b);
    setModified();
}

void mafToolVTKOrthoPlane::setOrigin(mafCore::mafPoint *o) {
    *m_Origin = *o;
    vtkMAFImplicitPlaneRepresentation *repX = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidgetX->GetRepresentation());
    repX->SetOrigin(m_Origin->x(), m_Origin->y(), m_Origin->z());
    vtkMAFImplicitPlaneRepresentation *repY = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidgetY->GetRepresentation());
    repY->SetOrigin(m_Origin->x(), m_Origin->y(), m_Origin->z());
    vtkMAFImplicitPlaneRepresentation *repZ = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidgetZ->GetRepresentation());
    repZ->SetOrigin(m_Origin->x(), m_Origin->y(), m_Origin->z());
    setModified();
}

void mafToolVTKOrthoPlane::resetTool() {
    removeWidget(m_PlaneWidgetX);
    removeWidget(m_PlaneWidgetY);
    removeWidget(m_PlaneWidgetZ);
}

void mafToolVTKOrthoPlane::graphicObjectInitialized() {
    addWidget(m_PlaneWidgetX);
    addWidget(m_PlaneWidgetY);
    addWidget(m_PlaneWidgetZ);
}
