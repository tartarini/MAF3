/*
 *  mafVTKParametricSurfaceSphere.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2011 SCS-B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKParametricSurfaceSphere.h"
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>


using namespace mafCore;
using namespace mafPluginVTK;


mafVTKParametricSurfaceSphere::mafVTKParametricSurfaceSphere(const QString code_location) : mafPluginVTK::mafVTKParametricSurface(code_location), m_SphereSource(NULL) {
    m_SphereRadius = 1.0;
    m_SpherePhiRes = 20;
    m_SphereThetaRes = 20;

    m_SphereSource = vtkSphereSource::New();
    m_Output = m_SphereSource->GetOutputPort();
    this->updateSurface();
}

mafVTKParametricSurfaceSphere::~mafVTKParametricSurfaceSphere(){
    m_SphereSource->Delete();
}

void mafVTKParametricSurfaceSphere::updateSurface(){
    //Set parameters to surface.
    m_SphereSource->SetCenter(m_Center);
    m_SphereSource->SetPhiResolution(m_SpherePhiRes);
    m_SphereSource->SetThetaResolution(m_SphereThetaRes);
    m_SphereSource->SetRadius(m_SphereRadius);
    m_SphereSource->Update();
}
