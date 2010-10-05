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
    mafDataSet *dataSet = dynamic_cast<mafDataSet*>(obj);
    if(dataSet != NULL) {
        mafString dataType = dataSet->dataValue()->externalDataType();
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

    mafDataSetList *inputList = this->inputList();

    mafDataSet *inputDataSet = inputList->at(0);
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

    if(m_Output == NULL) {
        m_Output = mafNEW(mafResources::mafDataSet);
        m_OutputValue = m_ThresholdFilter->GetOutput();
    }

    m_Output->setDataValue(&m_OutputValue);
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
