/*
 *  mafInteractor.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 07/10/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractor.h"

using namespace mafCore;
using namespace mafResources;

mafInteractor::mafInteractor(const QString code_location) : mafObjectBase(code_location), m_Blocking(false)/*, m_DefaultInteractorStyle(NULL), m_EditingInteractorStyle(NULL)*/ {
}

/*mafInteractor::mafInteractor(mafProxyInterfacePointer default_is, mafProxyInterfacePointer edit_is, const QString code_location) : mafObjectBase(code_location), m_DefaultInteractorStyle(default_is), m_EditingInteractorStyle(edit_is) {
}*/

mafInteractor::~mafInteractor() {
}

void mafInteractor::mousePress(double *pos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    
}

void mafInteractor::mouseRelease(double *pos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    
}

void mafInteractor::mouseMove(double *pos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    
}

void mafInteractor::enter(unsigned long modifiers, QEvent *e) {
    
}

void mafInteractor::leave(unsigned long modifiers, QEvent *e) {
    
}

void mafInteractor::keyPress(QEvent *e) {
    
}

void mafInteractor::keyRelease(QEvent *e) {
    
}

void mafInteractor::mouseWheelForward(unsigned long modifiers, QEvent *e) {
    
}

void mafInteractor::mouseWheelBackward(unsigned long modifiers, QEvent *e) {
    
}

/*void mafInteractor::setDefaultInteractionStyle(mafProxyInterfacePointer is) {
    if(is == m_DefaultInteractorStyle) {
        return;
    }
    m_DefaultInteractorStyle = is;
}

void mafInteractor::setEditingInteractionStyle(mafProxyInterfacePointer is) {
    if(is == m_EditingInteractorStyle) {
        return;
    }
    m_EditingInteractorStyle = is;
}
*/
