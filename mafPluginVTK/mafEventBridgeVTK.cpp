/*
 *  mafEventBridgeVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/10/10.
 *  Copyright 2009 B3C.s All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafEventBridgeVTK.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkCommand.h"
#include <mafEventBusManager.h>

#include <mafContainer.h>
#include <mafContainerInterface.h>

#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>
#include <vtkRendererCollection.h>
#include <vtkRenderWindow.h>


using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;


mafEventBridgeVTK::mafEventBridgeVTK(const mafString code_location) : mafObject(code_location), m_Connections(NULL), m_Interactor(NULL), m_Modifiers(NULL) {
}

mafEventBridgeVTK::mafEventBridgeVTK(vtkRenderWindowInteractor *inter, const mafString code_location) : mafObject(code_location), m_Interactor(NULL) {
    ENSURE(inter != NULL);
    m_Interactor = inter;
    initializeEventBridge();
}

mafEventBridgeVTK::~mafEventBridgeVTK() {
    if(m_Connections) m_Connections->Delete();
}

void mafEventBridgeVTK::setInteractor(vtkRenderWindowInteractor *inter) {
   ENSURE(inter != NULL);
   m_Interactor = inter;
   initializeEventBridge();
}

void mafEventBridgeVTK::initializeEventBridge() {
    m_Connections = vtkEventQtSlotConnect::New();

    m_Connections->Connect(m_Interactor, vtkCommand::LeftButtonPressEvent, this, SLOT(leftButtonPressEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::LeftButtonReleaseEvent, this, SLOT(leftButtonReleaseEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::MiddleButtonPressEvent, this, SLOT(middleButtonPressEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::MiddleButtonReleaseEvent, this, SLOT(middleButtonReleaseEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::RightButtonPressEvent, this, SLOT(rightButtonPressEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::RightButtonReleaseEvent , this, SLOT(rightButtonReleaseEvent ()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::EnterEvent, this, SLOT(enterEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::LeaveEvent, this, SLOT(leaveEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::KeyPressEvent, this, SLOT(keyPressEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::KeyReleaseEvent, this, SLOT(keyReleaseEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::CharEvent, this, SLOT(charEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::MouseMoveEvent, this, SLOT(mouseMoveEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::MouseWheelForwardEvent, this, SLOT(mouseWheelForwardEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::MouseWheelBackwardEvent, this, SLOT(mouseWheelBackwardEvent()), 0, 1.0);
    m_Connections->Connect(m_Interactor, vtkCommand::PickEvent, this, SLOT(pickEvent()), 0, 1.0);
}


void mafEventBridgeVTK::leftButtonPressEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.leftButtonPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::leftButtonReleaseEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.leftButtonRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::middleButtonPressEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.middleButtonPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::middleButtonReleaseEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.middleButtonRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::rightButtonPressEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.rightButtonPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::rightButtonReleaseEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.rightButtonRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::enterEvent() {
    getModifiers();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.enter", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::leaveEvent() {
    getModifiers();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.leave", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::keyPressEvent() {
    getModifiers();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.keyPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::keyReleaseEvent() {
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.keyRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::charEvent() {
    getModifiers();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.keyChar", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::mouseMoveEvent() {
    getModifiers();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.mouseMove", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::mouseWheelForwardEvent() {
    getModifiers();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.mouseWheelForward", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::mouseWheelBackwardEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.mouseWheelBackward", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::pickEvent() {
    getModifiers();
    vmePickCheck();
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(unsigned long, m_Modifiers));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.pick", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::getModifiers() {
    //check "shift" modifier.
    int flag = 1<<MAF_SHIFT_KEY;
    if (m_Interactor->GetShiftKey()) {
        m_Modifiers=m_Modifiers|flag;
    } else {
        m_Modifiers=m_Modifiers&(~flag);
    }

    //check "ctrl" modifier.
    flag = 1<<MAF_CTRL_KEY;
    if (m_Interactor->GetControlKey()) {
        m_Modifiers=m_Modifiers|flag;
    } else {
        m_Modifiers=m_Modifiers&(~flag);
    }

    //check "alt" modifier.
    flag = 1<<MAF_ALT_KEY;
    if (m_Interactor->GetAltKey()) {
        m_Modifiers=m_Modifiers|flag;
    } else {
        m_Modifiers=m_Modifiers&(~flag);
    }
}

void mafEventBridgeVTK::vmePickCheck() {
    int mousePosX = 0;
    int mousePosY = 0;
    double posPicked[3];
    mafCore::mafContainer<vtkActor> actor;
    mafContainerInterface *actorPicked;

    m_Interactor->GetEventPosition(mousePosX, mousePosY);
    vtkSmartPointer<vtkCellPicker> cellPicker = vtkSmartPointer<vtkCellPicker>::New();;
    vtkRendererCollection *rc = m_Interactor->GetRenderWindow()->GetRenderers();
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
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(double *, (double *)posPicked));
        argList.append(mafEventArgument(unsigned long, m_Modifiers));
        argList.append(mafEventArgument(mafCore::mafContainerInterface *, actorPicked));
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.interaction.vmePick", mafEventTypeLocal, &argList);
    }
}
