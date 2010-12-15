/*
 *  mafInteractor.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani on 07/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractor.h"

using namespace mafCore;
using namespace mafResources;

mafInteractor::mafInteractor(const mafString code_location) : mafObjectBase(code_location), m_DefaultInteractorStyle(NULL), m_EditingInteractorStyle(NULL) {
}

mafInteractor::mafInteractor(mafContainerInterfacePointer default_is, mafContainerInterfacePointer edit_is, const mafString code_location) : mafObjectBase(code_location), m_DefaultInteractorStyle(default_is), m_EditingInteractorStyle(edit_is) {
}

void mafInteractor::setDefaultInteractionStyle(mafContainerInterfacePointer is) {
    if(is == m_DefaultInteractorStyle) {
        return;
    }
    m_DefaultInteractorStyle = is;
}

void mafInteractor::setEditingInteractionStyle(mafContainerInterfacePointer is) {
    if(is == m_EditingInteractorStyle) {
        return;
    }
    m_EditingInteractorStyle = is;
}
