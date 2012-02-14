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
#include "mafBounds.h"

using namespace mafCore;

mafVisitorBounds::mafVisitorBounds(const QString code_location) : m_Bounds(NULL), mafVisitor(code_location) {
}

mafVisitorBounds::~mafVisitorBounds() {
}

void mafVisitorBounds::visit(mafObjectBase *object) {
    mafBounds *b = object->property("bounds").value<mafBoundsPointer>();
    
}
