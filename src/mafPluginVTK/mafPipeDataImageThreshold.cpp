/*
 *  mafPipeDataImageThreshold.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/04/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataImageThreshold.h"
#include <mafVME.h>
#include <mafDataSet.h>

#include <vtkDataSet.h>
#include <vtkImageThreshold.h>
#include <vtkImageData.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataImageThreshold::mafPipeDataImageThreshold(const QString code_location) : mafPipeData(code_location), m_LowerThreshold(0.0), m_UpperThreshold(1.0), m_ThresholdMode(mafThresholdModeBetween) {
    m_ThresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();
}

mafPipeDataImageThreshold::~mafPipeDataImageThreshold() {
}

bool mafPipeDataImageThreshold::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType == "vtkImageData") {
            return true;
        }
    }
    return false;
}

void mafPipeDataImageThreshold::updatePipe(double t) {
    if (inputList()->size() == 0) {
        return;
    }

    mafDataSet *inputDataSet = dataSetForInput(0, t);
    if(inputDataSet == NULL) {
        return;
    }

    //Get data contained in the mafProxy
    mafProxy<vtkAlgorithmOutput> *image = mafProxyPointerTypeCast(vtkAlgorithmOutput, inputDataSet->dataValue());

    m_ThresholdFilter->SetInputConnection((*image));
    switch(m_ThresholdMode) {
        case mafThresholdModeLower:
            m_ThresholdFilter->ThresholdByLower(m_LowerThreshold);
            break;
        case mafThresholdModeUpper:
            m_ThresholdFilter->ThresholdByUpper(m_UpperThreshold);
            break;
        default:
            m_ThresholdFilter->ThresholdBetween(m_LowerThreshold, m_UpperThreshold);
            break;
    }
    m_ThresholdFilter->Update();

    if(m_OutputValue == NULL) {
        m_OutputValue = m_ThresholdFilter->GetOutput();
    }

    output(t)->dataSetCollection()->itemAt(t)->setDataValue(&m_OutputValue);

    Superclass::updatePipe(t);
}

void mafPipeDataImageThreshold::setLowerThrehsold(double threshold) {
    if(m_LowerThreshold != threshold) {
        m_LowerThreshold = threshold;
    }
}

void mafPipeDataImageThreshold::setUpperThrehsold(double threshold) {
    if(m_UpperThreshold != threshold) {
        m_UpperThreshold = threshold;
    }
}
