/*
 *  mafPipeVisualVTKSliceVolume.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 09/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSliceVolume.h"

#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

#include <vtkAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkDataObject.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkTexture.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;

mafPipeVisualVTKSliceVolume::mafPipeVisualVTKSliceVolume(const QString code_location) : mafPipeVisualVTKSlice(code_location) {
//    m_UIFilename = "mafPipeVisualVTKSliceVolume.ui";

    m_SlicerPipe = mafNEW(mafPluginVTK::mafPipeDataSliceVolume);
    m_SlicerPipe->setParent(this);

    m_Mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    m_Mapper->ScalarVisibilityOff();

    mafProxyInterfacePointer t = m_SlicerPipe->textureOutput();
    mafProxy<vtkAlgorithmOutput> *tp = mafProxyPointerTypeCast(vtkAlgorithmOutput, t);

    vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
    texture->SetInputConnection(*tp);

    m_Prop3D = vtkActor::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    vtkActor::SafeDownCast(m_Prop3D)->SetTexture(texture);
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKSliceVolume::~mafPipeVisualVTKSliceVolume() {
    mafDEL(m_SlicerPipe);
}

bool mafPipeVisualVTKSliceVolume::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        // VME must be not NULL
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType == "vtkImageData" || dataType == "vtkStructuredPoints" || dataType == "vtkRectilinearGrid") {
            // and contains only volumetric dataset to be acceptable from this class.
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKSliceVolume::updatePipe(double t) {
    Superclass::updatePipe(t);

    // Calling 'origin' method instead of using the m_Origin variable directly
    // will cause that someone else can add itself as delegate of this class and produce
    // the slice origin outside.
    mafPointPointer o = origin();

    //Set the ABS matrix
    if(m_TransformMatrix) {
        m_SlicerPipe->setTransformMatrix(*m_TransformMatrix);
    }
    // Assign the input to the data pipe (it could be changed from the lest update).
    m_SlicerPipe->setInput(input());
    // Assign the origin of the slice.
    m_SlicerPipe->setSliceOrigin(o->x(), o->y(), o->z());
    // and ask the pipe to perform the cut.
    m_SlicerPipe->updatePipe(t);

    // gather the result from the data pipe and attach it to the mapper to be visualized.
    mafVME *output = m_SlicerPipe->output();
    mafDataSetCollection *collection = output->dataSetCollection();
    mafDataSet *dataSet = collection->itemAt(t);
    mafProxy<vtkAlgorithmOutput> *data = mafProxyPointerTypeCast(vtkAlgorithmOutput, dataSet->dataValue());
    m_Mapper->SetInputConnection(*data);

    vtkActor *actor = vtkActor::SafeDownCast(m_Prop3D);
    updatedGraphicObject();
}
