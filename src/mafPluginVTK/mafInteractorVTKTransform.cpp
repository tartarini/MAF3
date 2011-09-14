/*
 *  mafInteractorVTKTransform.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 7/7/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <QMouseEvent>

#include <mafDataSetCollection.h>

#include "mafInteractorVTKTransform.h"
#include "mafVTKWidget.h"
#include <mafMatrix.h>
#include <mafVME.h>
#include <mafDataSetCollection.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkAssembly.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkMAFInteractorStyleTrackballActor.h>

using namespace mafResources;
using namespace mafPluginVTK;

mafInteractorVTKTransform::mafInteractorVTKTransform(const QString code_location) : mafInteractor(code_location), m_DragObject(false), m_PreviousVTKInteractor(NULL) {
    m_Blocking = true;
    m_CurrentVTKInteractor = vtkMAFInteractorStyleTrackballActor::New();
}

mafInteractorVTKTransform::~mafInteractorVTKTransform(){
    m_CurrentVTKInteractor->Delete();
}

void mafInteractorVTKTransform::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    QMouseEvent *me = (QMouseEvent *) e;
    if(me->button() != Qt::LeftButton) {
        return;
    }
    
    mafVTKWidget *w = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if(w == NULL) {
        return;
    }
    
    vtkRenderWindowInteractor *rwi = w->GetRenderWindow()->GetInteractor();
    m_PreviousVTKInteractor = rwi->GetInteractorStyle();
    if(m_PreviousVTKInteractor) {
        m_PreviousVTKInteractor->Register(NULL);
    }
    rwi->SetInteractorStyle(m_CurrentVTKInteractor);
    m_CurrentVTKInteractor->Register(NULL);
    
    switch(((QMouseEvent *)e)->button()) {
        case Qt::LeftButton:
            rwi->InvokeEvent(vtkCommand::LeftButtonPressEvent, e);
            break;
        case Qt::MidButton:
            rwi->InvokeEvent(vtkCommand::MiddleButtonPressEvent, e);
            break;
        case Qt::RightButton:
            rwi->InvokeEvent(vtkCommand::RightButtonPressEvent, e);
            break;
        default:
            break;
    }

    m_DragObject = true;
}


void mafInteractorVTKTransform::mouseRelease(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    mafVTKWidget *w = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if(w == NULL) {
        return;
    }
    vtkRenderWindowInteractor *rwi = w->GetRenderWindow()->GetInteractor();

    switch(((QMouseEvent *)e)->button()) {
        case Qt::LeftButton:
            rwi->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, e);
            break;
        case Qt::MidButton:
            rwi->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, e);
            break;
        case Qt::RightButton:
            rwi->InvokeEvent(vtkCommand::RightButtonReleaseEvent, e);
            break;
        default:
            break;
    }
    
    rwi->SetInteractorStyle(m_PreviousVTKInteractor);
    
    m_DragObject = false;
}

void mafInteractorVTKTransform::mouseMove(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    if(!m_DragObject) return;
    
    mafVTKWidget *w = qobject_cast<mafVTKWidget *>(m_GraphicObject);
    if(w == NULL) {
        return;
    }
    vtkRenderWindowInteractor *rwi = w->GetRenderWindow()->GetInteractor();

    if(m_Renderer == NULL) {
        return;
    }
    
    mafResources::mafVME *vme = qobject_cast<mafResources::mafVME*>(obj);
    if(vme == NULL) {
        return;
    }
    
    rwi->InvokeEvent(vtkCommand::MouseMoveEvent, e);
    vtkAssembly *assembly = m_CurrentVTKInteractor->GetInteractionAssembly();
    vtkMatrix4x4 *m = assembly->GetMatrix();
    mafMatrix c;
    for(int i = 0; i<4; ++i) {
        for(int j = 0; j<4; ++j) {
            c.setElement(i,j, m->Element[i][j]);
        }
    }
    vme->dataSetCollection()->synchronizeItemWithPose(c);
    Q_EMIT(updateGuiSignal(NULL));
}
