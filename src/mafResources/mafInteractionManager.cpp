/*
 *  mafInteractionManager.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractionManager.h"
#include "mafVME.h"
#include "mafView.h"
#include "mafInteractor.h"

using namespace mafCore;
using namespace mafResources;

mafInteractionManager* mafInteractionManager::instance() {
    // Create the instance of the Interaction Manager manager.
    static mafInteractionManager instanceInteractionManager;
    return &instanceInteractionManager;
}

void mafInteractionManager::shutdown() {
}

mafInteractionManager::mafInteractionManager(const QString code_location) : mafObjectBase(code_location), m_VME(NULL), m_DefaultInteractor(NULL) {
    initializeConnection();
}

mafInteractionManager::~mafInteractionManager() {
}

void mafInteractionManager::initializeConnection() {
    mafRegisterLocalCallback("maf.local.resources.view.select", this, "setActiveView(mafCore::mafObjectBase *)")
    // VME selection callback.
    // Callback related to the VME selection
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "vmeSelect(mafCore::mafObjectBase *)")
}

void mafInteractionManager::mousePress(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e) {
    // Reset the VME picked.
    m_VME = NULL;
    // Ask the visual pipes to pick a VME
    Q_EMIT vmePickSignal(pos, modifiers, proxy, e);

    // m_VME is initialized with the picked VME if any otherwise it remains NULL
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->mousePress(pos, modifiers, m_VME, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->mousePress(pos, modifiers, m_VME, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->mousePress(pos, modifiers, m_VME, e);
    }
}

void mafInteractionManager::mouseRelease(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->mouseRelease(pos, modifiers, m_VME, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->mouseRelease(pos, modifiers, m_VME, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->mouseRelease(pos, modifiers, m_VME, e);
    }
}


void mafInteractionManager::mouseMove(double *pos, unsigned long modifiers, mafCore::mafProxyInterface *proxy, QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->mouseMove(pos, modifiers, m_VME, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->mouseMove(pos, modifiers, m_VME, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->mouseMove(pos, modifiers, m_VME, e);
    }
}

void mafInteractionManager::enter(unsigned long modifiers, QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->enter(modifiers, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->enter(modifiers, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->enter(modifiers, e);
    }
}

void mafInteractionManager::leave(unsigned long modifiers, QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->leave(modifiers, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->leave(modifiers, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->leave(modifiers, e);
    }
}

void mafInteractionManager::keyPress(QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->keyPress(e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->keyPress(e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->keyPress(e);
    }
}

void mafInteractionManager::keyRelease(QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->keyRelease(e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->keyRelease(e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->keyRelease(e);
    }
}


void mafInteractionManager::mouseWheelForward(unsigned long modifiers, QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->mouseWheelForward(modifiers, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->mouseWheelForward(modifiers, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->mouseWheelForward(modifiers, e);
    }
}

void mafInteractionManager::mouseWheelBackward(unsigned long modifiers, QEvent *e) {
    if(m_VME && m_VME->activeInteractor()) {
        m_VME->activeInteractor()->mouseWheelBackward(modifiers, e);
        if(!m_VME->activeInteractor()->isBlocking() && m_DefaultInteractor) {
            m_DefaultInteractor->mouseWheelBackward(modifiers, e);
        }
    } else if (m_DefaultInteractor) {
        m_DefaultInteractor->mouseWheelBackward(modifiers, e);
    }
}

void mafInteractionManager::vmePicked(double *pos, unsigned long modifiers, mafVME *vme, QEvent *e) {
    m_VME = vme;
}

void mafInteractionManager::setActiveView(mafObjectBase *obj) {
    mafView *v = qobject_cast<mafView *>(obj);
    m_DefaultInteractor = v->activeInteractor();
}

void mafInteractionManager::vmeSelect(mafObjectBase *node) {
    m_VME = (mafVME *) node;
}


