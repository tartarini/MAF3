/*
 *  mafVisitorBounds.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVisitorBounds.h"
#include <mafObjectFactory.h>

using namespace mafCore;
using namespace mafResources;

mafVisitorBounds::mafVisitorBounds(const QString code_location) : m_Bounds(NULL), mafVisitor(code_location) {
}

mafVisitorBounds::~mafVisitorBounds() {
    mafDEL(m_Bounds);
}

void mafVisitorBounds::visit(mafObjectBase *object) {
    QVariant boundsProp = object->property("bounds");
    if (boundsProp.isValid()) {
        mafBounds *b = boundsProp.value<mafBoundsPointer>();
        if(m_Bounds == NULL) {
            m_Bounds = new mafBounds();
            m_Bounds->setBounds(b);
        }

        m_Bounds->unite(*b, *m_Bounds);
    }
}
