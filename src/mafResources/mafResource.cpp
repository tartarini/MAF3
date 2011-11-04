/*
 *  mafResource.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani - Daniele Giunchi on 30/12/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResource.h"
#include "mafInteractor.h"

using namespace mafCore;
using namespace mafResources;


mafResource::mafResource(const QString code_location) : mafObject(code_location), m_Output(NULL), m_InputList(NULL) {
    m_InputList = new mafResourceList;
    m_Lock = new QReadWriteLock(QReadWriteLock::Recursive);
}

mafResource::~mafResource() {
    m_InputList->clear();
    delete m_InputList;
    m_InputList = NULL;
    
    while (!m_InteractorStack.isEmpty()) {
        popInteractor();
    }
    
    delete m_Lock;
}

bool mafResource::isObjectValid() const {
    if(Superclass::isObjectValid()) {
        mafResource *obj = NULL;
        Q_FOREACH(obj, *m_InputList) {
            if(obj == NULL) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool mafResource::isEqual(const mafObjectBase *obj) const {
    mafResource *resource = qobject_cast<mafResource *>((mafObjectBase *)obj);
    if(resource == NULL) {
        return false;
    }
    if(Superclass::isEqual(obj)) {
        return (*m_InputList == *((resource)->inputList()));
    }
    return false;
}

void mafResource::inputDestroyed() {
    mafResource *resource = (mafResource *)QObject::sender();
    removeInput(resource);
}

int mafResource::setInput(mafResource *resource, const int idx) {
    REQUIRE(resource != NULL);
    REQUIRE(idx >= 0);

    if(m_InputList->contains(resource)) {
        QByteArray ba = mafTr("Object %1 already present in input list").arg(resource->objectName()).toAscii();
        qWarning("%s", ba.data());
        return m_InputList->indexOf(resource);
    }

    if (resource->isObjectValid()) {
        connect(resource, SIGNAL(destroyed()), this, SLOT(inputDestroyed()));

        if(idx >= m_InputList->size()) {
            m_InputList->append(resource);
            return m_InputList->count() - 1;
        } else {
            m_InputList->replace(idx, resource);
            return idx;
        }
    }

    return -1;
}

int mafResource::addInput(mafResource *resource) {
    return setInput(resource, m_InputList->count());
}

void mafResource::removeInput(const int idx) {
    REQUIRE(idx >= 0);

    if(idx < m_InputList->count()) {
        // disconnect from the object destroyed signal.
        mafResource *obj = m_InputList->at(idx);
        disconnect(obj, SIGNAL(destroyed()),this, SLOT(inputDestroyed()));
        // remove the object from the input list
        m_InputList->removeAt(idx);
    } else {
        QByteArray ba = mafTr("Index %1 outside input list range.").arg(idx).toAscii();
        qWarning("%s", ba.data());
    }
}

void mafResource::removeInput(mafResource *resource) {
    REQUIRE(resource != NULL);

    int idx = m_InputList->indexOf(resource);
    if(idx != -1) {
        removeInput(idx);
    } else {
        QByteArray ba = mafTr("Object %1 not present in input list").arg(resource->objectName()).toAscii();
        qWarning("%s", ba.data());
    }
}

void mafResource::removeAllInputs() {
    int i = 0;
    int num = m_InputList->count();
    for( ; i < num; ++i) {
        removeInput(i);
    }
}

mafResource *mafResource::input(const int idx) {
    REQUIRE(idx >= 0);

    if (idx < m_InputList->size()) {
        return m_InputList->at(idx);
    } else {
        return NULL;
    }
}

mafMemento *mafResource::createMemento() const {
    mafMemento *m = Superclass::createMemento();
    mafMementoResource *mementoResource = new mafMementoResource(this, m_InputList, mafCodeLocation);
    m->setParent(mementoResource);

    return mementoResource;
}

void mafResource::setMemento(mafMemento *memento, bool deep_memento) {
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    mafMemento *m = (mafMemento *)memento->children().at(0);
    Superclass::setMemento(m, deep_memento);

    QStringList hashLists;
    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    Q_FOREACH(item, *list) {
        if(item.m_Name == "InputList") {
            hashLists.append(item.m_Value.toStringList());
        }
    }

    removeAllInputs();

    mafObjectRegistry *reg = mafObjectRegistry::instance();
    mafResource *res;
    QString hash;
    Q_FOREACH(hash, hashLists) {
        res = qobject_cast<mafResource *>(reg->objectFromHash(hash));
        if(res != NULL) {
            addInput(res);
        }
    }
}

void mafResource::terminate() {
}

void mafResource::execute() {
    Q_EMIT executionEnded();
}

void mafResource::pushInteractor(mafInteractor *i) {
    Q_EMIT interactorDetach();
    m_Lock->lockForWrite();
    m_InteractorStack.push(i);
    i->retain();
    m_Lock->unlock();
    Q_EMIT interactorAttached();
}

mafInteractor *mafResource::popInteractor() {
    mafInteractor *interactor = m_InteractorStack.pop();
    mafDEL(interactor);
    return interactor;
}
