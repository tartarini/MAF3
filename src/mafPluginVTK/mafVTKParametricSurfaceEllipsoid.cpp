/*
 *  mafVTKParametricSurfaceEllipsoid.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2011 SCS-B3C.s All rights reserved.
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


mafVTKParametricSurfaceEllipsoid::mafVTKParametricSurfaceEllipsoid(const QString code_location) : mafPluginVTK::mafVTKParametricSurface(code_location), m_EllipsoidSource(NULL), m_Ptf(NULL) {
    m_EllipsoidPhiRes = 20.0;
    m_EllipsoidThetaRes = 20.0;

    m_EllipsoidXLength = 1.0;
    m_EllipsoidYLength = 2.0;
    m_EllipsoidZLength = 3.0;

    m_EllipsoidSource = vtkSphereSource::New();
    m_Ptf = vtkTransformPolyDataFilter::New();
    m_Transform = vtkTransform::New();

    // modify proportion of the sphere
    m_Transform->Scale(m_EllipsoidXLength/m_EllipsoidYLength,1,m_EllipsoidZLength/m_EllipsoidYLength);
    m_Transform->Update();
    m_Ptf->SetTransform(m_Transform);
    

    m_Ptf->SetInputConnection(m_EllipsoidSource->GetOutputPort());
    m_Ptf->Update();

    // Output is the outputPort of the vtkTransformPolyDataFilter
    m_Output = m_Ptf->GetOutputPort();
    this->updateSurface();
}

mafVTKParametricSurfaceEllipsoid::~mafVTKParametricSurfaceEllipsoid(){
    m_Transform->Delete();
    m_Ptf->Delete();
    m_EllipsoidSource->Delete();
}

void mafVTKParametricSurfaceEllipsoid::updateSurface(){
    //Set parameters to surface.
    m_EllipsoidSource->SetRadius(m_EllipsoidYLength);
    m_EllipsoidSource->SetPhiResolution(m_EllipsoidPhiRes);
    m_EllipsoidSource->SetThetaResolution(m_EllipsoidThetaRes);
    m_EllipsoidSource->SetCenter(m_Center);

    m_Transform->Identity();
    m_Transform->Scale(m_EllipsoidXLength/m_EllipsoidYLength,1,m_EllipsoidZLength/m_EllipsoidYLength);
    m_Transform->Update();
    m_EllipsoidSource->Update();
}
