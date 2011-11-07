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

#include <vtkCommand.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkMAFInteractorStyleTrackballActor.h>

using namespace mafPluginVTK;

mafInteractorVTKCamera::mafInteractorVTKCamera(const QString code_location) : mafResources::mafInteractor(code_location), m_RenderWindowInteractor(NULL) {
}

mafInteractorVTKCamera::~mafInteractorVTKCamera() {
}

void mafInteractorVTKCamera::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    switch(((QMouseEvent *)e)->button()) {
        //m_RenderWindowInteractor->FindPokedRenderer(((QMouseEvent *)e)->x(), ((QMouseEvent *)e)->y());
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
    m_RenderWindowInteractor->InvokeEvent(vtkCommand::MouseMoveEvent, e);
}

void mafInteractorVTKCamera::setInteractorVTK(vtkRenderWindowInteractor *iren) {
    m_RenderWindowInteractor = iren;
}
