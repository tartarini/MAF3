/*
 *  mafPipeVisualVTKIsoSurfaceOutOfCore.cpp
 *  mafPluginOutOfCore
 *
 *  Created by Yubo Tao on 18/06/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKIsoSurfaceOutOfCore.h"
#include <mafVTKWidget.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkMAFContourVolumeMapper.h>
#include <vtkContourFilter.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkVolume.h>
#include <vtkAlgorithmOutput.h>
#include <vtkDecimatePro.h>
#include <vtkInformation.h>
#include <vtkProperty.h>
#include <vtkVolumeProperty.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace mafPluginOutOfCore;
using namespace std;

mafPipeVisualVTKIsoSurfaceOutOfCore::mafPipeVisualVTKIsoSurfaceOutOfCore(const QString code_location) : mafPipeVisualVTK(code_location), m_Mapper(NULL), m_ContourValue(NULL) {
    m_UIFilename = "mafPipeVisualIsoSurface.ui";
    
    m_Mapper = vtkMAFContourVolumeMapper::New();
    
    m_Prop3D = vtkVolume::New();
    m_Prop3D.setDestructionFunction(&vtkVolume::Delete);
    vtkVolume::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKIsoSurfaceOutOfCore::~mafPipeVisualVTKIsoSurfaceOutOfCore() {
    m_Mapper->Delete();
}

bool mafPipeVisualVTKIsoSurfaceOutOfCore::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkStructuredPoints", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKIsoSurfaceOutOfCore::updatePipe(double t) {
    Superclass::updatePipe(t);
    m_Mapper->SetOpacity(m_OpacityValue);

    mafDataSet *data = dataSetForInput(0, t);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());

    vtkAlgorithm *producer = (*dataSet)->GetProducer();
    vtkDataObject *dataObject = producer->GetOutputDataObject(0);
    vtkDataSet* vtkData = vtkDataSet::SafeDownCast(dataObject);
    vtkData->GetScalarRange(m_Range);

    if (m_ContourValue == NULL) {
        m_ContourValue = (m_Range[1] - m_Range[0])/2;
    }
//
//    m_ContourFilter->UseScalarTreeOn();
//    m_ContourFilter->SetInputConnection(*dataSet);
//    m_ContourFilter->SetNumberOfContours(1);
//    m_ContourFilter->SetValue(0, m_ContourValue);
//    m_ContourFilter->Update();

    
    //m_Mapper->SetInput(vtkData);
    m_Mapper->SetInputConnection(0, *dataSet);
    m_Mapper->SetContourValue(m_ContourValue);
    
    //Get data contained in the mafProxy
    m_Mapper->Update();

    //Keep ImmediateModeRendering off: it slows rendering
    //m_Mapper->SetImmediateModeRendering(m_ImmediateRendering);
    updatedGraphicObject();
}

QString mafPipeVisualVTKIsoSurfaceOutOfCore::contourValue() {
    return QString::number(m_ContourValue);
}

void mafPipeVisualVTKIsoSurfaceOutOfCore::setContourValue(QString value) {
    m_ContourValue = value.toInt();
}


void mafPipeVisualVTKIsoSurfaceOutOfCore::on_contourValue_textEdited(QString stringValue) {
    m_ContourValue = stringValue.toInt();
}

void mafPipeVisualVTKIsoSurfaceOutOfCore::on_contourValue_editingFinished() {
    updateUI(widget());
    updatePipe();
}

void mafPipeVisualVTKIsoSurfaceOutOfCore::on_contourValueSlider_sliderMoved(int value) {
    m_ContourValue = value;
    updateUI(widget());
}

void mafPipeVisualVTKIsoSurfaceOutOfCore::on_contourValueSlider_sliderReleased() {
    updatePipe();
}
