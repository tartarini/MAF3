/*
 *  mafPipeVisual.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisual.h"
#include "mafVME.h"
#include <mafProxyInterface.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


mafPipeVisual::mafPipeVisual(const QString code_location) : mafPipe(code_location), m_Output(NULL) {
    initializeConnections();
}

mafPipeVisual::~mafPipeVisual() {
}

void mafPipeVisual::initializeConnections() {
}

void mafPipeVisual::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafProxyInterface *actor, QEvent * e) {
    Q_UNUSED(e);
    if (m_Output && m_Output->isEqual(actor)) {
        mafVME *vme = this->inputList()->at(0);
        if (vme != NULL) {
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(double *, pickPos));
            argList.append(mafEventArgument(unsigned long, modifiers));
            argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
            emit vmePickedSignal(pickPos, modifiers, vme);
        }
    }
}

void mafPipeVisual::setInput(mafVME *vme) { 
    if (m_InputList->count() != 0) {
        if(!m_InputList->at(0)->isEqual(vme)) {
            disconnect(this, SIGNAL(vmePickedSignal(double *, unsigned long, mafCore::mafObjectBase* )), (QObject*)m_InputList->at(0)->interactor(), SLOT(vmePicked(double *, unsigned long, mafCore::mafObjectBase *)));
            disconnect(m_InputList->at(0), SIGNAL(interactorDetach()), this, SLOT(interactorDetach()));
            disconnect(m_InputList->at(0), SIGNAL(interactorAttached()), this, SLOT(interactorAttached()));
        } else {
            return;
        }
    }
    connect(this, SIGNAL(vmePickedSignal(double *, unsigned long, mafCore::mafObjectBase* )), (QObject*)vme->interactor(), SLOT(vmePicked(double *, unsigned long, mafCore::mafObjectBase *)));
    connect(vme, SIGNAL(interactorDetach()), this, SLOT(interactorDetach()));
    connect(vme, SIGNAL(interactorAttached()), this, SLOT(interactorAttached()));
    Superclass::setInput(vme);
}

void mafPipeVisual::interactorDetach() {
    mafVME *vme = (mafVME *)QObject::sender();
    disconnect(this, SIGNAL(vmePickedSignal(double *, unsigned long, mafCore::mafObjectBase* )), (QObject*)vme->interactor(), SLOT(vmePicked(double *, unsigned long, mafCore::mafObjectBase *)));
}

void mafPipeVisual::interactorAttached() {
    mafVME *vme = (mafVME *)QObject::sender();
    connect(this, SIGNAL(vmePickedSignal(double *, unsigned long, mafCore::mafObjectBase* )), (QObject*)vme->interactor(), SLOT(vmePicked(double *, unsigned long, mafCore::mafObjectBase *)));
}


void mafPipeVisual::setVisibility(bool visible) {
    m_Visibility = visible;
}

void mafPipeVisual::setGraphicObject(QObject *graphicObject) {
    m_GraphicObject = graphicObject;
    connect(m_GraphicObject, SIGNAL(vmePickSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)),  this, SLOT(vmePick(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)) );
}
