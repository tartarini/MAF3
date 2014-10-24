/*
 *  mafReferenceCounted.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 13/12/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *  
 *  See License at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafReferenceCounted.h"
#include "mafIdProvider.h"
#include "mafObjectRegistry.h"
#include "mafVisitor.h"

using namespace mafCore;

mafReferenceCounted::mafReferenceCounted() : QObject(), m_ReferenceCount(1) {
    connect(this, SIGNAL(incrementReference()), this, SLOT(ref()), Qt::DirectConnection);
    connect(this, SIGNAL(decreaseReference()), this, SLOT(deleteObject()), Qt::DirectConnection);
}

mafReferenceCounted::~mafReferenceCounted() {
}

void mafReferenceCounted::retain() {
    Q_EMIT incrementReference();
}

void mafReferenceCounted::release() {
    Q_EMIT decreaseReference();
}

void mafReferenceCounted::ref() {
    ++m_ReferenceCount;
}

void mafReferenceCounted::deleteObject() {
    --m_ReferenceCount;
    //char *name = this->objectName().toLatin1().data();
    if(m_ReferenceCount == 0) {
        this->setParent(NULL);
        delete this;
    }
}

bool mafReferenceCounted::isObjectValid() const {
    return m_ReferenceCount > 0;
}

void mafReferenceCounted::description() const {
    qDebug() << "Reference Count: " << referenceCount();
}
