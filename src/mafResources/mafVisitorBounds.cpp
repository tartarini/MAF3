/*
 *  mafVisitorBounds.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisitorBounds.h"
#include <mafObjectFactory.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

mafVisitorBounds::mafVisitorBounds(const QString code_location) : m_Bounds(NULL), m_AbsolutePoseFlag(false), mafVisitor(code_location) {
}

mafVisitorBounds::~mafVisitorBounds() {
    mafDEL(m_Bounds);
}

void mafVisitorBounds::visit(mafObjectBase *object) {
    QVariant boundsProp = object->property("bounds");
    if (boundsProp.isValid()) {
        mafBounds *b = new mafBounds();
        b->setBounds(boundsProp.value<mafBoundsPointer>());
        if(m_AbsolutePoseFlag) {
            mafMatrixPointer absMatrix = NULL;
            mafEventArgumentsList argList;
            argList.append(mafEventArgument(mafCore::mafObjectBase *, object));
            QGenericReturnArgument ret_val = mafEventReturnArgument(mafResources::mafMatrixPointer, absMatrix);
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.absolutePoseMatrix", mafEventTypeLocal, &argList, &ret_val);
            
            qDebug() << object->objectName();
            b->transformBounds(absMatrix);
            //b->description();
            delete absMatrix;
            absMatrix = NULL;
        }
        
        if(m_Bounds == NULL) {
            //qDebug() << "New bounds...";
            m_Bounds = new mafBounds();
            m_Bounds->setBounds(b);
        }

        m_Bounds->unite(*b);
        mafDEL(b);
    }
}
