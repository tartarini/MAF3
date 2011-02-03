/*
 *  mafVTKParametricSurfaceCone.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 19/11/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKParametricSurfaceCone.h"
#include <vtkSmartPointer.h>
#include <vtkConeSource.h>
#include <vtkAlgorithmOutput.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>


using namespace mafCore;
using namespace mafPluginVTK;


mafVTKParametricSurfaceCone::mafVTKParametricSurfaceCone(const mafString code_location) : mafPluginVTK::mafVTKParametricSurface(code_location), m_ConeSource(NULL) {
    m_ConeHeight = 2.0;
    m_ConeRadius = 1.0;
    m_ConeRes = 20.0;
    m_ConeCapping = 1;

    m_ConeSource = vtkConeSource::New();
    m_Output = m_ConeSource->GetOutputPort();
    this->updateSurface();
}

mafVTKParametricSurfaceCone::~mafVTKParametricSurfaceCone(){
    m_ConeSource->Delete();
}

void mafVTKParametricSurfaceCone::updateSurface(){
    //Set parameters to surface.
    m_ConeSource->SetHeight(m_ConeHeight);
    m_ConeSource->SetRadius(m_ConeRadius);
    m_ConeSource->SetCapping(m_ConeCapping);
    m_ConeSource->SetResolution(m_ConeRes);
    m_ConeSource->SetCenter(m_Center);
    m_ConeSource->Update();
}
