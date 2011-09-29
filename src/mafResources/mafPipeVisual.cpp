/*
 *  mafPipeVisual.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafPipeVisual.h"
#include "mafVME.h"
#include "mafInteractionManager.h"
#include "mafInteractor.h"
#include <mafProxyInterface.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


mafPipeVisual::mafPipeVisual(const QString code_location) : mafPipe(code_location), m_Output(NULL), m_GraphicObject(NULL), m_PipeVisualSelection(NULL), m_Visibility(false) {
    initializeConnections();
}

mafPipeVisual::~mafPipeVisual() {
}

void mafPipeVisual::initializeConnections() {
    //connections between visual pipe and interaction manager (vmePick and Picked signal)
    connect(mafInteractionManager::instance(), SIGNAL(vmePickSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)), this, SLOT(vmePick(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
    connect(this, SIGNAL(vmePickedSignal(double *, unsigned long, mafVME *, QEvent *)), mafInteractionManager::instance(), SLOT(vmePicked(double *, unsigned long, mafVME *, QEvent *)));
}

bool mafPipeVisual::vmePick(double *pickPos, unsigned long modifiers, mafCore::mafProxyInterface *actor, QEvent * e) {
    Q_UNUSED(e);
    if (m_Output && m_Output->isEqual(actor)) {
        mafVME *vme = this->inputList()->at(0);
        if (vme != NULL) {
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(double *, pickPos));
            argList.append(mafEventArgument(unsigned long, modifiers));
            argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
            Q_EMIT vmePickedSignal(pickPos, modifiers, vme, e);
            
            //this code will enable interactor to use information regarding graphic objects and vme
            mafInteractor *interactor = vme->activeInteractor();
            if(interactor) {
                interactor->setVME(vme);
                interactor->setGraphicObject(m_GraphicObject);
            }
            
            return true;
        }
    }
    
    return false;
}

void mafPipeVisual::setVisibility(bool visible) {
    if(m_Visibility != visible){
        m_Visibility = visible;
        updateVisibility();
    }
}

void mafPipeVisual::updateVisibility() {
}

void mafPipeVisual::setGraphicObject(QObject *graphicObject) {
    //if (m_GraphicObject != NULL) {
        //disconnect(m_GraphicObject, SIGNAL(vmePickSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)),  this, SLOT(vmePick(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
    //}

    m_GraphicObject = graphicObject;

    if (m_GraphicObject != NULL) {
        //connect between render object coming from external library and visual pipe (vme Pick signal)
        //connect(m_GraphicObject, SIGNAL(vmePickSignal(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)),  this, SLOT(vmePick(double *, unsigned long, mafCore::mafProxyInterface *, QEvent *)));
        setModified();
        updatedGraphicObject();
    }
}

void mafPipeVisual::updatedGraphicObject() {
    qDebug() << mafTr("Graphic object updated...");
}
