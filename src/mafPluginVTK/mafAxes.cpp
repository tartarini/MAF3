/*
 *  mafAxes.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 30/06/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafAxes.h"

#include <mafVME.h>
#include <mafEventBusManager.h>
#include <mafMatrix.h>

#include <vtkRenderer.h>
#include <vtkLookupTable.h>
#include <vtkViewport.h>
#include <vtkCamera.h>
#include <vtkAxes.h>
#include <vtkPolyDataMapper2D.h>
#include <vtkActor2D.h>
#include <vtkProperty2D.h>
#include <vtkLocalAxisCoordinate.h>
#include <vtkGlobalAxisCoordinate.h>

using namespace mafPluginVTK;
using namespace mafEventBus;

mafAxes::mafAxes(vtkRenderer *ren, mafResources::mafVME* vme) {
    m_Vme = vme;
    m_Renderer = ren;
    assert(m_Renderer);

	if(m_Vme) {
        m_Coord = vtkLocalAxisCoordinate::New();

        askAbsolutePoseMatrix();
        ((vtkLocalAxisCoordinate*) m_Coord)->SetScaleFactor(m_Vme->length()/8.0);
    } else {
		m_Coord = vtkGlobalAxisCoordinate::New();
    }

    vtkLookupTable *axesLUT = vtkLookupTable::New();
    axesLUT->SetNumberOfTableValues(3);
    axesLUT->SetTableValue(0,1,0,0,1);
    axesLUT->SetTableValue(1,0,1,0,1);
    axesLUT->SetTableValue(2,0,0,1,1);
    axesLUT->Build();

    vtkAxes *axes = vtkAxes::New();
    axes->SetScaleFactor(1);

    vtkPolyDataMapper2D	*axesMapper = vtkPolyDataMapper2D::New();
    axesMapper->SetInput(axes->GetOutput());
    axesMapper->SetTransformCoordinate(m_Coord);
    axesMapper->SetLookupTable(axesLUT);
    axesMapper->SetScalarRange(0, 0.5);
    axesLUT->Delete();
    axes->Delete();

    m_AxesActor = vtkActor2D::New();
    m_AxesActor->SetMapper(axesMapper); 
    m_AxesActor->GetProperty()->SetLineWidth(2);
    m_AxesActor->VisibilityOff();
    m_AxesActor->PickableOff();
    m_Renderer->Register(NULL);
    m_Renderer->AddActor2D(m_AxesActor);
    axesMapper->Delete();
}

void mafAxes::askAbsolutePoseMatrix() {
    mafEventArgumentsList argList;
    mafResources::mafMatrixPointer absMatrix = NULL;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_Vme));
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrixPointer, absMatrix);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);

    vtkMatrix4x4 *m = vtkMatrix4x4::New();
    if (absMatrix) {
        m->DeepCopy(absMatrix->rawData());
        delete absMatrix;
        absMatrix = NULL;
    }

    ((vtkLocalAxisCoordinate*) m_Coord)->SetMatrix(m);
    m_Coord->Modified();
    m->Delete();
}

mafAxes::~mafAxes() {
	m_Renderer->RemoveActor2D(m_AxesActor);
    m_Renderer->UnRegister(NULL);
    m_Coord->Delete();
    m_AxesActor->Delete();
}

void mafAxes::setVisibility(bool show) {
    m_AxesActor->SetVisibility(show);
}

void mafAxes::updatePose( vtkMatrix4x4 *abs_pose_matrix ) {
    // WARNING - I am assuming that if m_Vme != NULL --> m_Coord ISA vtkLocalAxisCoordinate
    if(!m_Vme) {
        return;
    }
    assert(m_Coord);

    if( abs_pose_matrix ) {
        vtkLocalAxisCoordinate *coord = (vtkLocalAxisCoordinate*) m_Coord; 
        coord->SetMatrix(abs_pose_matrix);
        coord->Modified();
    } else {
        askAbsolutePoseMatrix();
    }
}
