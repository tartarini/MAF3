/*
 *  mafDataPipeImageThreshold.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/04/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafDataPipeImageThreshold.h"
#include <mafDataSet.h>

#include <vtkImageThreshold.h>
#include <vtkImageData.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafDataPipeImageThreshold::mafDataPipeImageThreshold(const mafString code_location) : mafDataPipe(code_location), m_LowerThreshold(0.0), m_UpperThreshold(1.0), m_ThresholdMode(BETWEEN) {
}

mafDataPipeImageThreshold::~mafDataPipeImageThreshold() {
}

bool mafDataPipeImageThreshold::acceptObject(mafCore::mafObject *obj) {
    mafVME *vme = dynamic_cast<mafVME*>(obj);
    if(vme != NULL) {
        mafString dataType = vme->dataSetCollection()->itemAtCurrentTime()->dataValue()->externalDataType();
        if(dataType == "vtkImageData") {
            return true;
        }
    }
    return false;
}

void mafDataPipeImageThreshold::createPipe() {
    m_ThresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();
}

void mafDataPipeImageThreshold::updatePipe(double t) {
    Q_UNUSED(t); // Consider also to calculate the threshold for all the timestamp if the input is time varying

    mafVMEList *inputList = this->inputList();

    mafDataSet *inputDataSet = inputList->at(0)->dataSetCollection()->itemAtCurrentTime();
    if(inputDataSet == NULL) {
        return;
    }

    //Get data contained in the mafContainer
    mafContainer<vtkImageData> *image = mafContainerPointerTypeCast(vtkImageData, inputDataSet->dataValue());

    double sr[2];
    (*image)->GetScalarRange(sr);

    m_ThresholdFilter->SetInputConnection((*image)->GetProducerPort());
    switch(m_ThresholdMode) {
        case LOWER:
            m_ThresholdFilter->ThresholdByLower(m_LowerThreshold);
            break;
        case UPPER:
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

    if (!m_InPlace) {
        m_Output = mafNEW(mafResources::mafVME);
        m_Output = inputList->at(0);
        m_Output->dataSetCollection()->itemAtCurrentTime()->setDataValue(&m_OutputValue);
    } else {
        inputList->at(0)->dataSetCollection()->itemAtCurrentTime()->setDataValue(&m_OutputValue);
        m_Output = inputList->at(0);
    }

}

void mafDataPipeImageThreshold::setLowerThrehsold(double threshold) {
    if(m_LowerThreshold != threshold) {
        m_LowerThreshold = threshold;
    }
}

void mafDataPipeImageThreshold::setUpperThrehsold(double threshold) {
    if(m_UpperThreshold != threshold) {
        m_UpperThreshold = threshold;
    }
}
