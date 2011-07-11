/*
 *  mafInteractor.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 07/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractor.h"

using namespace mafCore;
using namespace mafResources;

mafInteractor::mafInteractor(const QString code_location) : mafObjectBase(code_location), m_DefaultInteractorStyle(NULL), m_EditingInteractorStyle(NULL) {
}

mafInteractor::mafInteractor(mafProxyInterfacePointer default_is, mafProxyInterfacePointer edit_is, const QString code_location) : mafObjectBase(code_location), m_DefaultInteractorStyle(default_is), m_EditingInteractorStyle(edit_is) {
}

mafInteractor::~mafInteractor() {
}

void mafInteractor::vmePicked(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    Q_UNUSED(pickPos);
    Q_UNUSED(modifiers);
    Q_UNUSED(obj);
    Q_UNUSED(e);

}

void mafInteractor::setDefaultInteractionStyle(mafProxyInterfacePointer is) {
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
