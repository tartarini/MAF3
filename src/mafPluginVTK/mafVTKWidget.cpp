/*
 *  mafVTKWidget.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/10/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVTKWidget.h"
#include "mafAxes.h"

#include <QInputEvent>
#include <mafProxyInterface.h>
#include <mafInteractionManager.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkAssemblyPath.h>
#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>
#include <vtkProp.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

mafVTKWidget::mafVTKWidget(QWidget* parent, Qt::WFlags f) : QVTKWidget(parent, f), m_Axes(NULL) {
    initializeConnections();
}

mafVTKWidget::~mafVTKWidget() {
    if (m_Axes != NULL) {
        delete m_Axes;
        m_Axes = NULL;
    }
}

void mafVTKWidget::initializeConnections() {
    bool result(false);
    result = connect(this, SIGNAL(mousePressSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), mafInteractionManager::instance(), SLOT(mousePress(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
    result = connect(this, SIGNAL(mouseReleaseSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), mafInteractionManager::instance(), SLOT(mouseRelease(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
    result= connect(this, SIGNAL(mouseMoveSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), mafInteractionManager::instance(), SLOT(mouseMove(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
}

void mafVTKWidget::showAxes(bool show) {
    vtkRenderer *ren = GetRenderWindow()->GetRenderers()->GetFirstRenderer();
    if(ren) {
        m_Axes = new mafAxes(ren);
    } else {
        return;
    }
    m_Axes->setVisibility(show);
}

void mafVTKWidget::mousePressEvent(QMouseEvent* e) {
    // Emit a mouse press event for anyone who might be interested
    emit mouseEvent(e);

    vtkRenderWindowInteractor* iren = NULL;
    if(this->mRenWin) {
        iren = this->mRenWin->GetInteractor();
    }

    if(!iren || !iren->GetEnabled()) {
        return;
    }

    // give interactor the event information
    iren->SetEventInformationFlipY(e->x(), e->y(),
                                   (e->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                   (e->modifiers() & Qt::ShiftModifier ) > 0 ? 1 : 0,
                                   0,
                                   e->type() == QEvent::MouseButtonDblClick ? 1 : 0);

    this->getModifiers(iren);
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));

    // Check if a VME has been picked
    this->mousePress(iren, e);

}

void mafVTKWidget::mouseReleaseEvent(QMouseEvent* e) {
    vtkRenderWindowInteractor* iren = NULL;
    if(this->mRenWin) {
        iren = this->mRenWin->GetInteractor();
    }

    if(!iren || !iren->GetEnabled()) {
        return;
    }

    // give vtk event information
    iren->SetEventInformationFlipY(e->x(), e->y(),
                                   (e->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                   (e->modifiers() & Qt::ShiftModifier ) > 0 ? 1 : 0);

    this->getModifiers(iren);
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));

    // Check if a VME has been picked
    this->mouseRelease(iren, e);

}

void mafVTKWidget::mouseMoveEvent(QMouseEvent* e) {
    vtkRenderWindowInteractor* iren = NULL;
    if(this->mRenWin) {
        iren = this->mRenWin->GetInteractor();
    }
    
    if(!iren || !iren->GetEnabled()) {
        return;
    }
    
    // give interactor the event information
    iren->SetEventInformationFlipY(e->x(), e->y(),
                                   (e->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                   (e->modifiers() & Qt::ShiftModifier ) > 0 ? 1 : 0);
    
    this->getModifiers(iren);
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    
    // Check if a VME has been picked
    this->mouseMove(iren, e);
    
}


void mafVTKWidget::wheelEvent(QWheelEvent* e) {
    vtkRenderWindowInteractor* iren = NULL;
    if(this->mRenWin) {
        iren = this->mRenWin->GetInteractor();
    }

    if(!iren || !iren->GetEnabled()) {
        return;
    }

    // VTK supports wheel mouse events only in version 4.5 or greater
    // give event information to interactor
    iren->SetEventInformationFlipY(e->x(), e->y(),
                                   (e->modifiers() & Qt::ControlModifier) > 0 ? 1 : 0,
                                   (e->modifiers() & Qt::ShiftModifier ) > 0 ? 1 : 0);

    this->getModifiers(iren);
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));

    //wheel event signal need to be created

    // invoke vtk event
    // if delta is positive, it is a forward wheel event
    if(e->delta() > 0) {
        iren->InvokeEvent(vtkCommand::MouseWheelForwardEvent, e); //Move into InteractorManager?
    } else {
        iren->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, e); //Move into InteractorManager?
    }
}

void mafVTKWidget::getModifiers(vtkRenderWindowInteractor* iren) {
    //check "shift" modifier.
    int flag = 1<<MAF_SHIFT_KEY;
    if (iren->GetShiftKey()) {
        m_Modifiers=m_Modifiers|flag;
    } else {
        m_Modifiers=m_Modifiers&(~flag);
    }

    //check "ctrl" modifier.
    flag = 1<<MAF_CTRL_KEY;
    if (iren->GetControlKey()) {
        m_Modifiers=m_Modifiers|flag;
    } else {
        m_Modifiers=m_Modifiers&(~flag);
    }

    //check "alt" modifier.
    flag = 1<<MAF_ALT_KEY;
    if (iren->GetAltKey()) {
        m_Modifiers=m_Modifiers|flag;
    } else {
        m_Modifiers=m_Modifiers&(~flag);
    }
}


void mafVTKWidget::mousePress(vtkRenderWindowInteractor* iren, QEvent *e) {
    int mousePosX = 0;
    int mousePosY = 0;
    double posPicked[3];
    mafCore::mafProxy<vtkProp> actorPicked;
    vtkProp *actor = NULL;
    
    iren->GetEventPosition(mousePosX, mousePosY);
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    vtkRendererCollection *rc = iren->GetRenderWindow()->GetRenderers();
    vtkRenderer *r = NULL;
    rc->InitTraversal();
    while(r = rc->GetNextItem()) {
        int picked = cellPicker->Pick(mousePosX,mousePosY,0,r);
        if(picked) {
            cellPicker->GetPickPosition(posPicked);
            vtkAssemblyPath *path = cellPicker->GetPath();
            actor = path->GetLastNode()->GetViewProp();
            actorPicked = actor;
        }
    }
    
    emit mousePressSignal(posPicked, m_Modifiers, &actorPicked, e);
    
    // invoke appropriate VTK event
    switch(((QMouseEvent *)e)->button()) {
        case Qt::LeftButton:
            iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, e); //Move into InteractorManager?
            break;
            
        case Qt::MidButton:
            iren->InvokeEvent(vtkCommand::MiddleButtonPressEvent, e); //Move into InteractorManager?
            break;
            
        case Qt::RightButton:
            iren->InvokeEvent(vtkCommand::RightButtonPressEvent, e); //Move into InteractorManager?
            break;
            
        default:
            break;
    }

    
}
void mafVTKWidget::mouseRelease(vtkRenderWindowInteractor* iren, QEvent *e) {
    int mousePosX = 0;
    int mousePosY = 0;
    double posPicked[3];
    mafCore::mafProxy<vtkProp> actorPicked;
    vtkProp *actor = NULL;
    
    iren->GetEventPosition(mousePosX, mousePosY);
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    vtkRendererCollection *rc = iren->GetRenderWindow()->GetRenderers();
    vtkRenderer *r = NULL;
    rc->InitTraversal();
    while(r = rc->GetNextItem()) {
        int picked = cellPicker->Pick(mousePosX,mousePosY,0,r);
        if(picked) {
            cellPicker->GetPickPosition(posPicked);
            vtkAssemblyPath *path = cellPicker->GetPath();
            actor = path->GetLastNode()->GetViewProp();
            actorPicked = actor;
        }
    }
    
    
    emit mouseReleaseSignal(posPicked, m_Modifiers, &actorPicked, e);
    
    // invoke appropriate vtk event
    switch(((QMouseEvent *)e)->button()) {
        case Qt::LeftButton:
            iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, e); //Move into InteractorManager?
            break;
            
        case Qt::MidButton:
            iren->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, e); //Move into InteractorManager?
            break;
            
        case Qt::RightButton:
            iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent, e); //Move into InteractorManager?
            break;
            
        default:
            break;
    }

}
void mafVTKWidget::mouseMove(vtkRenderWindowInteractor* iren, QEvent *e) {
    int mousePosX = 0;
    int mousePosY = 0;
    double posPicked[3];
    mafCore::mafProxy<vtkProp> actorPicked;
    vtkProp *actor = NULL;
    
    iren->GetEventPosition(mousePosX, mousePosY);
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();
    vtkRendererCollection *rc = iren->GetRenderWindow()->GetRenderers();
    vtkRenderer *r = NULL;
    rc->InitTraversal();
    while(r = rc->GetNextItem()) {
        int picked = cellPicker->Pick(mousePosX,mousePosY,0,r);
        if(picked) {
            cellPicker->GetPickPosition(posPicked);
            vtkAssemblyPath *path = cellPicker->GetPath();
            actor = path->GetLastNode()->GetViewProp();
            actorPicked = actor;
        }
    }
    
    
    emit mouseMoveSignal(posPicked, m_Modifiers, &actorPicked, e);
    
    // invoke vtk event
    iren->InvokeEvent(vtkCommand::MouseMoveEvent, e); //Move into InteractorManager?

}

// overloaded resize handler
void mafVTKWidget::resizeEvent(QResizeEvent* event) {
    QVTKWidget::resizeEvent(event);
}
