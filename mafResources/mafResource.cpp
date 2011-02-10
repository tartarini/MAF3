/*
 *  mafResource.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResource.h"

using namespace mafCore;
using namespace mafResources;


mafResource::mafResource(const mafString code_location) : mafObject(code_location), m_Busy(false), m_DataLoaded(false), m_InputList(NULL), m_Output(NULL) {
    m_InputList = new mafResourceList;
}

mafResource::~mafResource() {
    mafDEL(m_Output);

    m_InputList->clear();
    mafDEL(m_InputList);
}

bool mafResource::isObjectValid() const {
    if(Superclass::isObjectValid()) {
        mafResource *obj = NULL;
        foreach(obj, *m_InputList) {
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
        mafMsgWarning("%s", mafTr("Object %1 already present in input list").arg(resource->objectName()).toAscii().data());
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
        mafMsgWarning("%s", mafTr("Index %1 outside input list range.").arg(idx).toAscii().data());
    }
}

void mafResource::removeInput(mafResource *resource) {
    REQUIRE(resource != NULL);

    int idx = m_InputList->indexOf(resource);
    if(idx != -1) {
        removeInput(idx);
    } else {
        mafMsgWarning("%s", mafTr("Object %1 not present in input list").arg(resource->objectName()).toAscii().data());
    }
}

void mafResource::removeAllInputs() {
    int i = 0;
    int num = m_InputList->count();
    for( ; i < num; i++) {
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

    mafStringList hashLists;
    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    foreach(item, *list) {
        if(item.m_Name == "InputList") {
            hashLists.append(item.m_Value.toStringList());
        }
    }

    removeAllInputs();

    mafObjectRegistry *reg = mafObjectRegistry::instance();
    mafResource *res;
    mafString hash;
    foreach(hash, hashLists) {
        res = qobject_cast<mafResource *>(reg->objectFromHash(hash));
        if(res != NULL) {
            addInput(res);
        }
    }
}

void mafResource::setDataLoaded(bool dataLoaded) {
    m_DataLoaded = dataLoaded;
}

bool mafResource::initialize() {
    return true;
}

bool mafResource::terminate() {
    return true;
}

void mafResource::execute() {
}

