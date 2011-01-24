/*
 *  mafVTKParametricSurfaceEllipsoid.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKParametricSurfaceEllipsoid.h"
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>


using namespace mafCore;
using namespace mafPluginVTK;


mafVTKParametricSurfaceEllipsoid::mafVTKParametricSurfaceEllipsoid(const mafString code_location) : mafPluginVTK::mafVTKParametricSurface(code_location), m_EllipsoidSource(NULL), m_Ptf(NULL) {
    m_EllipsoidRadius = 1.0;
    m_EllipsoidPhiRes = 10.0;
    m_EllipsoidTheRes = 10.0;

    m_EllipsoidXLenght = 1.0;
    m_EllipsoidYLenght = 2.0;
    m_EllipsoidZLenght = 3.0;

    m_EllipsoidSource = vtkSphereSource::New();
    m_Ptf = vtkTransformPolyDataFilter::New();
    m_Transofrm = vtkTransform::New();

    // modify proportion of the sphere
    m_Transofrm->Scale(m_EllipsoidXLenght/m_EllipsoidYLenght,1,m_EllipsoidZLenght/m_EllipsoidYLenght);
    m_Transofrm->Update();
    m_Ptf->SetTransform(m_Transofrm);

    m_Ptf->SetInputConnection(m_EllipsoidSource->GetOutputPort());
    m_Ptf->Update();

    // Output is the outputPort of the vtkTransformPolyDataFilter
    m_Output = m_Ptf->GetOutputPort();
}

mafVTKParametricSurfaceEllipsoid::~mafVTKParametricSurfaceEllipsoid(){
    m_Transofrm->Delete();
    m_Ptf->Delete();
    m_EllipsoidSource->Delete();
}

void mafVTKParametricSurfaceEllipsoid::updateSurface(){
    //Set parameters to surface.
    m_EllipsoidSource->SetRadius(m_EllipsoidRadius);
    m_EllipsoidSource->SetPhiResolution(m_EllipsoidPhiRes);
    m_EllipsoidSource->SetThetaResolution(m_EllipsoidTheRes);
    m_EllipsoidSource->SetCenter(m_Center);
    m_Transofrm->Update();
    m_EllipsoidSource->Update();
}
