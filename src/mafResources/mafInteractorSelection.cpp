/*
 *  mafInteractorSelection.cpp
 *  mafResources
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 07/10/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafInteractorSelection.h"

#include <QMouseEvent>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

mafInteractorSelection::mafInteractorSelection(const QString code_location) : mafInteractor(code_location) {
}

/*mafInteractorSelection::mafInteractorSelection(mafProxyInterfacePointer default_is, mafProxyInterfacePointer edit_is, const QString code_location) : mafInteractor(default_is, edit_is, code_location) {
}*/

mafInteractorSelection::~mafInteractorSelection() {
}

void mafInteractorSelection::mousePress(double *pickPos, unsigned long modifiers, mafCore::mafObjectBase *obj, QEvent *e) {
    //check if is possible select a new selection
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
    bool selectable(false);
    QGenericReturnArgument ret_val = mafEventReturnArgument(bool, selectable);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.canSelectVME", mafEventTypeLocal, &argList, &ret_val);
    
    if(!selectable) {
        return;
    }
    
    QMouseEvent *me = (QMouseEvent *)e;
    if(me->button() == Qt::LeftButton) {
        mafEventBus::mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, obj));
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventBus::mafEventTypeLocal, &argList);
    }
}
