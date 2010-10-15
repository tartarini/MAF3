/*
 *  mafEventBridgeVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 12/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafEventBridgeVTK.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkCommand.h"
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;


mafEventBridgeVTK::mafEventBridgeVTK(const mafString code_location) : mafObject(code_location), m_Interactor(NULL) {
}

mafEventBridgeVTK::mafEventBridgeVTK(vtkRenderWindowInteractor *inter, const mafString code_location) : mafObject(code_location), m_Interactor(NULL) {
    ENSURE(inter != NULL);
    m_Interactor = inter;
    initializeEventBridge();
}

void mafEventBridgeVTK::setInteractor(vtkRenderWindowInteractor *inter) {
   ENSURE(inter != NULL);
   m_Interactor = inter;
   initializeEventBridge();
}

void mafEventBridgeVTK::initializeEventBridge() {
    vtkEventQtSlotConnect *connections = vtkEventQtSlotConnect::New();

    connections->Connect(m_Interactor, vtkCommand::LeftButtonPressEvent, this, SLOT(leftButtonPressEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::LeftButtonReleaseEvent, this, SLOT(leftButtonReleaseEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::MiddleButtonPressEvent, this, SLOT(middleButtonPressEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::MiddleButtonReleaseEvent, this, SLOT(middleButtonReleaseEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::RightButtonPressEvent, this, SLOT(rightButtonPressEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::RightButtonReleaseEvent , this, SLOT(rightButtonReleaseEvent ()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::EnterEvent, this, SLOT(enterEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::LeaveEvent, this, SLOT(leaveEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::KeyPressEvent, this, SLOT(keyPressEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::KeyReleaseEvent, this, SLOT(keyReleaseEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::CharEvent, this, SLOT(charEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::TimerEvent, this, SLOT(timerEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::MouseMoveEvent, this, SLOT(mouseMoveEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::MouseWheelForwardEvent, this, SLOT(mouseWheelForwardEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::MouseWheelBackwardEvent, this, SLOT(mouseWheelBackwardEvent()), 0, 1.0);
    connections->Connect(m_Interactor, vtkCommand::PickEvent, this, SLOT(pickEvent()), 0, 1.0);

    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.resources.VTK.interaction.leftButtonPress");
    provider->createNewId("maf.local.resources.VTK.interaction.leftButtonRelease");
    provider->createNewId("maf.local.resources.VTK.interaction.middleButtonPress");
    provider->createNewId("maf.local.resources.VTK.interaction.middleButtonRelease");
    provider->createNewId("maf.local.resources.VTK.interaction.rightButtonPress");
    provider->createNewId("maf.local.resources.VTK.interaction.rightButtonRelease");
    provider->createNewId("maf.local.resources.VTK.interaction.enter");
    provider->createNewId("maf.local.resources.VTK.interaction.leave");
    provider->createNewId("maf.local.resources.VTK.interaction.keyPress");
    provider->createNewId("maf.local.resources.VTK.interaction.keyRelease");
    provider->createNewId("maf.local.resources.VTK.interaction.char");
    provider->createNewId("maf.local.resources.VTK.interaction.timer");
    provider->createNewId("maf.local.resources.VTK.interaction.mouseMove");
    provider->createNewId("maf.local.resources.VTK.interaction.mouseWheelForward");
    provider->createNewId("maf.local.resources.VTK.interaction.mouseWheelBackward");
    provider->createNewId("maf.local.resources.VTK.interaction.pick");
}

void mafEventBridgeVTK::leftButtonPressEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.leftButtonPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::leftButtonReleaseEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.leftButtonRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::middleButtonPressEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.middleButtonPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::middleButtonReleaseEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.middleButtonRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::rightButtonPressEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.rightButtonPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::rightButtonReleaseEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.rightButtonRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::enterEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.enter", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::leaveEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.leave", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::keyPressEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.keyPress", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::keyReleaseEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.keyRelease", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::charEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.char", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::timerEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.timer", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::mouseMoveEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.mouseMove", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::mouseWheelForwardEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.mouseWheelForward", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::mouseWheelBackwardEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.mouseWheelBackward", mafEventTypeLocal, &argList);
}

void mafEventBridgeVTK::pickEvent() {
    mafEventArgumentsList argList;
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.VTK.interaction.pick", mafEventTypeLocal, &argList);
}

