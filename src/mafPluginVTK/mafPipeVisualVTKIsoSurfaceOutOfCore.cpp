/*
 *  mafPipeVisualVTKIsoSurfaceOutOfCore.cpp
 *  mafPluginVTK
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
#include <mafVolume.h>
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
#include <vtkUnsignedCharArray.h>
#include <vtkCharArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkShortArray.h>
#include <vtkUnsignedIntArray.h>
#include <vtkIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>

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
        if(dataType.startsWith("mafPluginOutOfCore::mafVolume", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKIsoSurfaceOutOfCore::updatePipe(double t) {
    Superclass::updatePipe(t);
    m_Mapper->SetOpacity(m_OpacityValue);

    mafDataSet *data = dataSetForInput(0, t);
    mafProxy<mafVolume> *dataSet = mafProxyPointerTypeCast(mafVolume, data->dataValue());
    
    vtkImageData *vtkData = vtkImageData::New();
    toVTKImageData(dataSet, vtkData);
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

    
    m_Mapper->SetInput(vtkData);
    //m_Mapper->SetInputConnection(0, *dataSet);
    m_Mapper->SetContourValue(m_ContourValue);
    
    //Get data contained in the mafProxy
    m_Mapper->Update();

    //Keep ImmediateModeRendering off: it slows rendering
    //m_Mapper->SetImmediateModeRendering(m_ImmediateRendering);
    updatedGraphicObject();

    vtkData->Delete();
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

void mafPipeVisualVTKIsoSurfaceOutOfCore::toVTKImageData(mafProxy<mafVolume> *volume, vtkImageData *vtkData) {
    mafVolume *volData = *volume;
    pair<int, int> typePairs[] = { make_pair(mafVolUnsignedChar,  VTK_UNSIGNED_CHAR),
                                   make_pair(mafVolChar,          VTK_CHAR),
                                   make_pair(mafVolUnsignedShort, VTK_UNSIGNED_SHORT),
                                   make_pair(mafVolShort,         VTK_SHORT),
                                   make_pair(mafVolUnsignedInt,   VTK_UNSIGNED_INT),
                                   make_pair(mafVolInt,           VTK_INT),
                                   make_pair(mafVolFloat,         VTK_FLOAT),
                                   make_pair(mafVolDouble,        VTK_DOUBLE)
                                  };
    map<int, int> typeMap(typePairs, typePairs + sizeof(typePairs) / sizeof(typePairs[0]));

    // Data type
    mafDataType dataType = volData->dataType();
    vtkData->SetScalarType(typeMap[dataType]);
    
    // Start positions ?
    // Original Dimensions ?
    // Dimensions
    int *dimensions = volData->dimensions();
    int extent[6] = { 0, dimensions[0] - 1, 0, dimensions[1] - 1, 0, dimensions[2] - 1 };
    vtkData->SetExtent(extent);

    // Component number
    int componentNum = volData->componentNum();
    vtkData->SetNumberOfScalarComponents(componentNum);

    // Big endian ?
    // Data values
    vtkData->AllocateScalars();
    vtkDataArray *voxels = vtkData->GetPointData()->GetScalars();
    if(voxels == NULL) {
        return;
    }
    int voxelNum = dimensions[0] * dimensions[1] * dimensions[2] * componentNum;
    void *values = voxels->WriteVoidPointer(0, voxelNum);
    memcpy (values, volData->dataValue(), voxelNum * getByteNum(dataType));

    // Spacing
    float *spacing = volData->spacing();
    vtkData->SetSpacing(spacing[0], spacing[1], spacing[2]);

    // Pose matrix ?
}
