/*
 *  mafToolVTKPlane.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 19/1/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafToolVTKPlane.h"
#include "mafSceneNodeVTK.h"

#include <mafPipeDataSlice.h>

#include <vtkCommand.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkMAFImplicitPlaneRepresentation.h>


// Callback for the interaction
// This does the actual work: updates the vtkPlane implicit function.
// This in turn causes the pipeline to update and clip the object.
class vtkIPWCallback : public vtkCommand {
public:
	static vtkIPWCallback *New() { return new vtkIPWCallback; }
  	virtual void Execute(vtkObject *caller, unsigned long, void*);
  	vtkIPWCallback() : Tool(0) {}
    mafResources::mafTool *Tool;
};

void vtkIPWCallback::Execute(vtkObject *caller, unsigned long, void*) {
    vtkImplicitPlaneWidget2 *planeWidget = reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
    vtkMAFImplicitPlaneRepresentation *rep = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
    double ori[3], nor[3];
    rep->GetOrigin(ori);
    rep->GetNormal(nor);
    mafCore::mafPoint *o, *n;
    o = new mafCore::mafPoint(ori);
    n = new mafCore::mafPoint(nor);
    QVariant vo, vn;
    vo.setValue<mafCore::mafPoint>(*o);
    vn.setValue<mafCore::mafPoint>(*n);
    Tool->setProperty("origin", vo);
    Tool->setProperty("normal", vn);
    Tool->setModified();
    mafDEL(o);
    mafDEL(n);
}


//-----------------------------------------------------------------------------------------------------

using namespace mafPluginVTK;
using namespace mafCore;

mafToolVTKPlane::mafToolVTKPlane(const QString code_location) : mafToolVTK(code_location) {
    VTK_CREATE(vtkIPWCallback, myCallback);
    myCallback->Tool = this;

    double b[6] = {0,1,0,1,0,1};

    m_Origin = new mafPoint(0,0,0);
    m_Normal = new mafPoint(0,0,1);
    m_VOI = new mafBounds(b);

    VTK_CREATE(vtkMAFImplicitPlaneRepresentation, rep);
    rep->SetPlaceFactor(1.25);
    rep->DrawPlaneOff();
    rep->DrawOutlineBoxOff();
    rep->SetRotationEnabled(0);
    rep->OutlineTranslationOff();
    rep->SetOrigin(0,0,0);
    rep->SetNormal(0,0,1);

    m_PlaneWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();
    m_PlaneWidget->SetRepresentation(rep);
    m_PlaneWidget->AddObserver(vtkCommand::InteractionEvent,myCallback);
}

mafToolVTKPlane::~mafToolVTKPlane() {
    mafDEL(m_Origin);
    mafDEL(m_Normal);
    mafDEL(m_VOI);
}

void mafToolVTKPlane::setVOI(mafCore::mafBounds *bounds) {
    *m_VOI = *bounds;
    vtkMAFImplicitPlaneRepresentation *rep = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidget->GetRepresentation());
    double b[6];
    b[0] = bounds->xMin();
    b[1] = bounds->xMax();
    b[2] = bounds->yMin();
    b[3] = bounds->yMax();
    b[4] = bounds->zMin();
    b[5] = bounds->zMax();
    rep->PlaceWidget(b);
    setModified();
}

void mafToolVTKPlane::setOrigin(mafCore::mafPoint *o) {
    *m_Origin = *o;
    vtkMAFImplicitPlaneRepresentation *rep = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidget->GetRepresentation());
    rep->SetOrigin(m_Origin->x(), m_Origin->y(), m_Origin->z());
    setModified();
}

void mafToolVTKPlane::setNormal(mafCore::mafPoint *n) {
    *m_Normal = *n;
    vtkMAFImplicitPlaneRepresentation *rep = reinterpret_cast<vtkMAFImplicitPlaneRepresentation*>(m_PlaneWidget->GetRepresentation());
    rep->SetNormal(m_Normal->x(), m_Normal->y(), m_Normal->z());
    setModified();
}

void mafToolVTKPlane::resetTool() {
    removeWidget(m_PlaneWidget);
}

void mafToolVTKPlane::graphicObjectInitialized() {
    addWidget(m_PlaneWidget);
}
