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

#include <QInputEvent>
#include <mafContainer.h>
#include <mafContainerInterface.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>
#include <vtkActor.h>
#include <vtkCellPicker.h>
#include <vtkSmartPointer.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;

mafVTKWidget::mafVTKWidget(QWidget* parent, Qt::WFlags f) : QVTKWidget(parent, f)
{
}

void mafVTKWidget::mousePressEvent(QMouseEvent* e){
    // Emit a mouse press event for anyone who might be interested
    emit mouseEvent(e);

    vtkRenderWindowInteractor* iren = NULL;
    if(this->mRenWin)
      {
      iren = this->mRenWin->GetInteractor();
      }

    if(!iren || !iren->GetEnabled())
      {
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
    this->vmePickCheck(iren, e);

    // invoke appropriate vtk event
    switch(e->button())
      {
      case Qt::LeftButton:
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.leftButtonPress", mafEventTypeLocal, &argList);
        iren->InvokeEvent(vtkCommand::LeftButtonPressEvent, e); //Move into InteractorManager?
        break;

      case Qt::MidButton:
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.middleButtonPress", mafEventTypeLocal, &argList);
        iren->InvokeEvent(vtkCommand::MiddleButtonPressEvent, e); //Move into InteractorManager?
        break;

      case Qt::RightButton:
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.rightButtonPress", mafEventTypeLocal, &argList);
        iren->InvokeEvent(vtkCommand::RightButtonPressEvent, e); //Move into InteractorManager?
        break;

      default:
        break;
      }
}

void mafVTKWidget::mouseReleaseEvent(QMouseEvent* e){
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    {
    iren = this->mRenWin->GetInteractor();
    }

  if(!iren || !iren->GetEnabled())
    {
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
  this->vmePickCheck(iren, e);

  // invoke appropriate vtk event
  switch(e->button())
    {
    case Qt::LeftButton:
      mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.leftButtonRelease", mafEventTypeLocal, &argList);
      iren->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, e); //Move into InteractorManager?
      break;

    case Qt::MidButton:
      mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.middleButtonRelease", mafEventTypeLocal, &argList);
      iren->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, e); //Move into InteractorManager?
      break;

    case Qt::RightButton:
      mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.rightButtonRelease", mafEventTypeLocal, &argList);
      iren->InvokeEvent(vtkCommand::RightButtonReleaseEvent, e); //Move into InteractorManager?
      break;

    default:
      break;
    }
}

void mafVTKWidget::wheelEvent(QWheelEvent* e){
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    {
    iren = this->mRenWin->GetInteractor();
    }

  if(!iren || !iren->GetEnabled())
    {
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

  // Check if a VME has been picked
  this->vmePickCheck(iren, e);

  // invoke vtk event
  // if delta is positive, it is a forward wheel event
  if(e->delta() > 0)
    {
      mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.mouseWheelForward", mafEventTypeLocal, &argList);
    iren->InvokeEvent(vtkCommand::MouseWheelForwardEvent, e); //Move into InteractorManager?
    }
  else
    {
      mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.mouseWheelBackward", mafEventTypeLocal, &argList);
    iren->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, e); //Move into InteractorManager?
    }
}

void mafVTKWidget::mouseMoveEvent(QMouseEvent* e){
  vtkRenderWindowInteractor* iren = NULL;
  if(this->mRenWin)
    {
    iren = this->mRenWin->GetInteractor();
    }

  if(!iren || !iren->GetEnabled())
    {
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
  this->vmePickCheck(iren, e);

  // invoke vtk event
  mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.mouseMove", mafEventTypeLocal, &argList);
  iren->InvokeEvent(vtkCommand::MouseMoveEvent, e); //Move into InteractorManager?
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

void mafVTKWidget::vmePickCheck(vtkRenderWindowInteractor* iren, QEvent *e) {
    int mousePosX = 0;
    int mousePosY = 0;
    double posPicked[3];
    mafCore::mafContainer<vtkActor> actor;
    actor = NULL;
    mafContainerInterface *actorPicked;

    iren->GetEventPosition(mousePosX, mousePosY);
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();;
    vtkRendererCollection *rc = iren->GetRenderWindow()->GetRenderers();
    vtkRenderer *r = NULL;
    rc->InitTraversal();
    while(r = rc->GetNextItem()) {
        if(cellPicker->Pick(mousePosX,mousePosY,0,r)) {
            cellPicker->GetPickPosition(posPicked);
            actor = cellPicker->GetActor();
            actorPicked = &(actor);
        }
    }

    if (actor != NULL) {
        int m = e->type();


        mafEventArgumentsList argList;
        argList.append(mafEventArgument(double *, (double *)posPicked));
        argList.append(mafEventArgument(unsigned long, m_Modifiers));
        argList.append(mafEventArgument(mafCore::mafContainerInterface *, actorPicked));
        argList.append(mafEventArgument(QEvent *, e));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.vmePick", mafEventTypeLocal, &argList);
    }
}


