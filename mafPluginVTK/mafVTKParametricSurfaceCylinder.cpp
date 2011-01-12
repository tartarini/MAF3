/*
 *  mafVTKParametricSurfaceCylinder.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKParametricSurfaceCylinder.h"
#include <vtkSmartPointer.h>
#include <vtkCylinderSource.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>


using namespace mafCore;
using namespace mafPluginVTK;


mafVTKParametricSurfaceCylinder::mafVTKParametricSurfaceCylinder(const mafString code_location) : mafPluginVTK::mafVTKParametricSurface(code_location), m_CylinderSource(NULL) {
    m_CylinderHeight = 2.0;
    m_CylinderRadius = 1.0;
    m_CylinderRes = 20.0;

    m_CylinderSource = vtkCylinderSource::New();
    m_Output = m_CylinderSource->GetOutputPort();
    this->updateSurface();
}

mafVTKParametricSurfaceCylinder::~mafVTKParametricSurfaceCylinder(){
    m_CylinderSource->Delete();
}

void mafVTKParametricSurfaceCylinder::updateSurface(){
    //Set parameters to surface.
    m_CylinderSource->SetHeight(m_CylinderHeight);
    m_CylinderSource->SetRadius(m_CylinderRadius);
    m_CylinderSource->SetResolution(m_CylinderRes);
    m_CylinderSource->SetCenter(m_Center);
    m_CylinderSource->Update();
}
