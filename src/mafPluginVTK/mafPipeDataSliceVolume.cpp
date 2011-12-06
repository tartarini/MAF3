/*
 *  mafPipeDataSliceVolume.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 24/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataSliceVolume.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkDataSet.h>
#include <vtkMAFVolumeSlicer.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkExecutive.h>
#include <vtkPointData.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataSliceVolume::mafPipeDataSliceVolume(const QString code_location) : mafPipeData(code_location) {
    m_SliceOrigin[0] = m_SliceOrigin[1] = m_SliceOrigin[2] = 0.;

    m_Normal[0] = m_Normal[1] = 0.;
    m_Normal[2] = 1.;
    this->setPlaneNormalAxes(mafPlaneNormalZ);

    m_Slicer = vtkSmartPointer<vtkMAFVolumeSlicer>::New();

    m_OutputPolygonal = m_Slicer->GetOutputPort();
    m_OutputTexture   = m_Slicer->GetTexturedOutputPort();
}

mafPipeDataSliceVolume::~mafPipeDataSliceVolume() {
}

bool mafPipeDataSliceVolume::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType == "vtkImageData" || dataType == "vtkStructuredPoints" || dataType == "vtkRectilinearGrid") {
            return true;
        }
    }
    return false;
}

void mafPipeDataSliceVolume::updatePipe(double t) {
    if (inputList()->size() == 0) {
        return;
    }

    mafDataSet *inputDataSet = dataSetForInput(0, t);
    if(inputDataSet == NULL) {
        return;
    }

    //Get data contained in the mafProxy
    mafProxy<vtkAlgorithmOutput> *volume = mafProxyPointerTypeCast(vtkAlgorithmOutput, inputDataSet->dataValue());

    m_Slicer->SetInputConnection(*volume);
    QVariantList b = inputDataSet->bounds();
    m_SliceOrigin[0] = (b[0].toDouble() + b[1].toDouble()) / 2.;
    m_SliceOrigin[1] = (b[2].toDouble() + b[3].toDouble()) / 2.;
    m_SliceOrigin[2] = (b[4].toDouble() + b[5].toDouble()) / 2.;
    m_Slicer->SetPlaneOrigin(m_SliceOrigin);

    m_Slicer->SetPlaneAxisX(m_XVector);
    m_Slicer->SetPlaneAxisY(m_YVector);

    m_Slicer->Update();

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputPolygonal);
    Superclass::updatePipe(t);
}

void mafPipeDataSliceVolume::pushSlice(double distance) {
    if (distance != 0) {
        for (int i=0; i < 3; ++i) {
            this->m_SliceOrigin[i] += distance * this->m_Normal[i];
        }
        this->updatePipe();
    }
}

void mafPipeDataSliceVolume::setNormal(double *normal) {
    REQUIRE(normal);

    m_Normal[0] = normal[0];
    m_Normal[1] = normal[1];
    m_Normal[2] = normal[2];

    m_PlaneNormalAxes = mafPlaneNormalArbitrary;

    double res[3], n;
    double xyz[3][3] = {{1., 0., 0.}, {0., 1., 0.}, {0., 0., 1.}};
    for (int a = 0; a < 3; ++a) {
        vtkMath::Cross(m_Normal, xyz[a], res);
        n = vtkMath::Norm(res);
        if (n == 0) {
            m_PlaneNormalAxes = (mafPlaneNormal)a;
            break;
        }
    }
}

void mafPipeDataSliceVolume::setPlaneNormalAxes(mafPlaneNormal axes) {
    m_PlaneNormalAxes = axes;
    switch (axes) {
        case mafPlaneNormalX:
            m_XVector[0] = 0.0001; //this is a ancestor bug... very sad.
            m_XVector[1] = 1.;
            m_XVector[2] = 0.;
            m_YVector[0] = 0.;
            m_YVector[1] = 0.;
            m_YVector[2] = 1.;
            m_Normal[0] = 1.;
            m_Normal[1] = 0.;
            m_Normal[2] = 0.;
            break;
        case mafPlaneNormalY:
            m_XVector[0] = 0.;
            m_XVector[1] = 0.;
            m_XVector[2] = 1.;
            m_YVector[0] = 1.;
            m_YVector[1] = 0.;
            m_YVector[2] = 0.;
            m_Normal[0] = 0.;
            m_Normal[1] = 1.;
            m_Normal[2] = 0.;
            break;
        case mafPlaneNormalZ:
            m_XVector[0] = 1.;
            m_XVector[1] = 0.;
            m_XVector[2] = 0.;
            m_YVector[0] = 0.;
            m_YVector[1] = 1.;
            m_YVector[2] = 0.;
            m_Normal[0] = 0.;
            m_Normal[1] = 0.;
            m_Normal[2] = 1.;
            break;
        default:
            qDebug() << mafTr("Arbitrary slicing...");
            break;
    }
}
