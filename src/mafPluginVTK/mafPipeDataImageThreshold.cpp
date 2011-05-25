/*
 *  mafPipeDataImageThreshold.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 16/04/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeDataImageThreshold.h"

#include <vtkDataSet.h>
#include <vtkImageThreshold.h>
#include <vtkImageData.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeDataImageThreshold::mafPipeDataImageThreshold(const QString code_location) : mafPipeData(code_location), m_LowerThreshold(0.0), m_UpperThreshold(1.0), m_ThresholdMode(BETWEEN) {
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

void mafPipeDataImageThreshold::createPipe() {
    m_ThresholdFilter = vtkSmartPointer<vtkImageThreshold>::New();
    if(inputList()->size() == 0) {
        qWarning(mafTr("Assign an input VME before asking the creation of the data pipe!!").toAscii());
        return;
    }
    m_Output = this->inputList()->at(0);
}

void mafPipeDataImageThreshold::updatePipe(double t) {
    mafVME *inputVME = this->inputList()->at(0);

    mafDataSet *inputDataSet = inputVME->dataSetCollection()->itemAt(t);
    if(inputDataSet == NULL) {
        return;
    }

    //Get data contained in the mafProxy
    mafProxy<vtkImageData> *image = mafProxyPointerTypeCast(vtkImageData, inputDataSet->dataValue());

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

    m_Output->dataSetCollection()->itemAtCurrentTime()->setDataValue(&m_OutputValue);
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
