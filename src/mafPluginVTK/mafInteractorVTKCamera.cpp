/*
 *  mafInteractorVTKCamera.cpp
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 7/7/11.
 *  Copyright 2011 B3C.s All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <QMouseEvent>

#include "mafInteractorVTKCamera.h"
#include "mafVTKWidget.h"

#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkMAFInteractorStyleTrackballCamera.h>

using namespace mafPluginVTK;

mafInteractorVTKCamera::mafInteractorVTKCamera(const QString code_location) : mafResources::mafInteractor(code_location), m_RenderWindowInteractor(NULL) {
    m_CurrentVTKInteractor = vtkMAFInteractorStyleTrackballCamera::New();
}

mafInteractorVTKCamera::~mafInteractorVTKCamera() {
    m_CurrentVTKInteractor->Delete();
}

void mafInteractorVTKCamera::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    if (m_RenderWindowInteractor == NULL) {
        mafVTKWidget *w = qobject_cast<mafVTKWidget *>(m_GraphicObject);
        if(w == NULL) {
            return;
        }
        m_RenderWindowInteractor = w->GetRenderWindow()->GetInteractor();
    }
    m_RenderWindowInteractor->SetInteractorStyle(m_CurrentVTKInteractor);
    m_CurrentVTKInteractor->Register(NULL);

    switch(((QMouseEvent *)e)->button()) {
        case Qt::LeftButton:
            m_RenderWindowInteractor->InvokeEvent(vtkCommand::LeftButtonPressEvent, e);
            break;
        case Qt::MidButton:
            m_RenderWindowInteractor->InvokeEvent(vtkCommand::MiddleButtonPressEvent, e);
            break;
        case Qt::RightButton:
            m_RenderWindowInteractor->InvokeEvent(vtkCommand::RightButtonPressEvent, e);
            break;
        default:
            break;
    }
}

void mafInteractorVTKCamera::mouseRelease(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    switch(((QMouseEvent *)e)->button()) {
        case Qt::LeftButton:
            m_RenderWindowInteractor->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, e);
            break;
        case Qt::MidButton:
            m_RenderWindowInteractor->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, e);
            break;
        case Qt::RightButton:
            m_RenderWindowInteractor->InvokeEvent(vtkCommand::RightButtonReleaseEvent, e);
            break;
        default:
            break;
    }
}

void mafInteractorVTKCamera::mouseMove(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    vtkCamera *camera = m_RenderWindowInteractor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
    camera->SetClippingRange(0.1, 1000);
    m_RenderWindowInteractor->InvokeEvent(vtkCommand::MouseMoveEvent, e);
}
