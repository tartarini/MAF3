/*
 *  mafPipeVisualVTKSurface.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisualVTKSurface.h"
#include "mafVTKWidget.h"

#include <mafVME.h>
#include <mafDataSet.h>
#include <mafDataSetCollection.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkActor.h>
#include <vtkProperty.h>

#include <QColorDialog>
#include <QColor>


using namespace mafCore;
using namespace mafResources;
using namespace mafPluginVTK;
using namespace std;

mafPipeVisualVTKSurface::mafPipeVisualVTKSurface(const QString code_location) : mafPipeVisualVTK(code_location), m_Mapper(NULL), m_ScalarVisibility(true) {
    m_UIFilename = "mafPipeVisualSurface.ui";
    m_Palette = QPalette();
    m_Mapper = vtkPolyDataMapper::New();
    m_Prop3D = vtkActor::New();
    m_Prop3D.setDestructionFunction(&vtkActor::Delete);
    vtkActor::SafeDownCast(m_Prop3D)->SetMapper(m_Mapper);
    m_Output = &m_Prop3D;
}

mafPipeVisualVTKSurface::~mafPipeVisualVTKSurface() {
    m_Mapper->Delete();
}

bool mafPipeVisualVTKSurface::acceptObject(mafCore::mafObjectBase *obj) {
    mafVME *vme = qobject_cast<mafVME*>(obj);
    if(vme != NULL) {
        QString dataType = vme->dataSetCollection()->itemAtCurrentTime()->externalDataType();
        if(dataType.startsWith("vtkPolyData", Qt::CaseSensitive)) {
            return true;
        }
    }
    return false;
}

void mafPipeVisualVTKSurface::updatePipe(double t) {
    // This call will set the modified state of the pipe to false.
    Superclass::updatePipe(t);
    vtkActor::SafeDownCast(m_Prop3D)->GetProperty()->SetOpacity(m_OpacityValue);

    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }

    double col[3] = {1,1,1};
    col[0] = vme->property("colorR").toDouble();
    col[1] = vme->property("colorG").toDouble();
    col[2] = vme->property("colorB").toDouble();
    vtkActor::SafeDownCast(m_Prop3D)->GetProperty()->SetDiffuseColor(col);
    m_Palette.setColor(QPalette::Background,QColor(col[0]*255, col[1]*255, col[2]*255));
    updateUI(widget());

    mafDataSet *data = dataSetForInput(0, t);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, data->dataValue());
    
    //Get data contained in the mafProxy
    m_Mapper->SetInputConnection(*dataSet);
    m_Mapper->SetScalarVisibility(m_ScalarVisibility);
    //Keep ImmediateModeRendering off: it slows rendering
    //m_Mapper->SetImmediateModeRendering(m_ImmediateRendering);
    updatedGraphicObject();
}

void mafPipeVisualVTKSurface::on_colorButton_released() {
    double col[3];
    vtkActor::SafeDownCast(m_Prop3D)->GetProperty()->GetDiffuseColor(col);

    //open color dialog
    QColor color = QColorDialog::getColor(QColor(col[0]*255, col[1]*255, col[2]*255));
    double r = color.toRgb().red()/255.;
    double g = color.toRgb().green()/255.;
    double b = color.toRgb().blue()/255.;

    mafVME *vme = input();
    if(vme == NULL) {
        return;
    }
    vme->setProperty("colorR", r);
    vme->setProperty("colorG", g);
    vme->setProperty("colorB", b);
    updatePipe();
}

QPalette mafPipeVisualVTKSurface::palette() {
    return m_Palette;
}

void mafPipeVisualVTKSurface::on_scalarCheck_stateChanged(int state){
    m_ScalarVisibility = state;
    m_Mapper->SetScalarVisibility(m_ScalarVisibility);
    updatedGraphicObject();
}