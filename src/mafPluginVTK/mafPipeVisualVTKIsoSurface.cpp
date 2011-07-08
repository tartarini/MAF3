/*
 *  mafPipeVisualVTKIsoSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 10/05/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKIsoSurface.h"
#include "mafVTKWidget.h"

#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkPolyDataMapper.h>
#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkActor.h>
#include <vtkAlgorithmOutput.h>
#include <vtkDecimatePro.h>
#include <vtkInformation.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace std;

mafPipeVisualVTKIsoSurface::mafPipeVisualVTKIsoSurface(const QString code_location) : mafPipeVisualVTK(code_location), m_Mapper(NULL), m_ContourFilter(NULL), m_ContourValue(1000) {
    m_UIFilename = "mafPipeVisualIsoSurface.ui";
    m_Mapper = vtkPolyDataMapper::New();
    m_Prop3D = vtkActor::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKIsoSurface::~mafPipeVisualVTKIsoSurface() {
    m_ContourFilter->Delete();
    m_Mapper->Delete();
}

bool mafPipeVisualVTKIsoSurface::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkStructuredPoints", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKIsoSurface::updatePipe(double t) {
    Superclass::updatePipe(t);

    mafDataSet *data = dataSetForInput(0, t);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

    vtkAlgorithm *producer = (*dataSet)->GetProducer();
    vtkDataObject *dataObject = producer->GetOutputDataObject(0);
    vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);
    vtkData->GetScalarRange(m_Range);

    if (m_ContourFilter == NULL) {
        m_ContourValue = (m_Range[1] - m_Range[0])/2;
    }

    m_ContourFilter = vtkContourFilter::New();
    m_ContourFilter->UseScalarTreeOn();
    m_ContourFilter->SetInputConnection(*dataSet);
    m_ContourFilter->SetNumberOfContours(1);
    m_ContourFilter->SetValue(0, m_ContourValue);
    m_ContourFilter->Update();
    
    //Get data contained in the mafProxy
    m_Mapper->SetInputConnection(m_ContourFilter->GetOutputPort());
    m_Mapper->SetScalarVisibility(m_ScalarVisibility);
    m_Mapper->Update();
    //Keep ImmediateModeRendering off: it slows rendering
    //m_Mapper->SetImmediateModeRendering(m_ImmediateRendering);
    render();
}

QString mafPipeVisualVTKIsoSurface::contourValue() {
    return QString::number(m_ContourValue);
}

void mafPipeVisualVTKIsoSurface::setContourValue(QString value) {
    m_ContourValue = value.toInt();
}

void mafPipeVisualVTKIsoSurface::on_contourValue_textEdited(QString stringValue) {
    m_ContourValue = stringValue.toInt();
}

void mafPipeVisualVTKIsoSurface::on_contourValue_editingFinished() {
    updateUI();
    updatePipe();
}

void mafPipeVisualVTKIsoSurface::on_contourValueSlider_valueChanged(int value) {
    m_ContourValue = value;
    updateUI();
}

void mafPipeVisualVTKIsoSurface::on_contourValueSlider_sliderReleased() {
    updatePipe();
}
