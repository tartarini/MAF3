/*
 *  mafObjectRegistry.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafObjectRegistry.h"
#include "mafObjectBase.h"
#include "mafObjectFactory.h"
#include "mafVisitorFindObjectsByHashCode.h"

using namespace mafCore;


mafObjectRegistry* mafObjectRegistry::instance() {
    static mafObjectRegistry instanceObjectRegistry;
    return &instanceObjectRegistry;
}

mafObjectRegistry::~mafObjectRegistry() {
    mafObjectRegistry::instance()->dumpLiveObjects();
}

void mafObjectRegistry::addObject(mafObjectBase *obj, const mafString location) {
    // Create the registry item with object's information
    mafObjectRegistryItem ri;
    ri.m_Object = obj;
    ri.m_InstantiateLocationInfo = location;
    ri.m_AllocationTime = mafTime::currentTime();

    // Insert the item into the registry.
    m_Registry.insert(obj->objectId(), ri);
}

void mafObjectRegistry::removeObject(mafId obj_id) {
    m_Registry.remove(obj_id);
}

void mafObjectRegistry::dumpLiveObjects() {
    if(m_Registry.empty()) {
        return;
    }

    // Dump on standard console the objects that still are present into the registry (memory leaks!!)
    mafRegistryHashType::const_iterator iter = m_Registry.constBegin();
    while(iter != m_Registry.constEnd()) {
        mafString name = iter.value().m_Object->metaObject()->className();
        mafString loc = iter.value().m_InstantiateLocationInfo;
        mafTime t = iter.value().m_AllocationTime;
        mafMsgDebug() << "class name: " << name << ", allocated by: " << loc << ", at time: " << t.toString("hh:mm:ss");
        ++iter;
    }
}

void mafObjectRegistry::liveObjects(mafObjectsList *objects) {
    REQUIRE(objects != NULL);

    objects->clear();

    if(m_Registry.empty()) {
        return;
    }

    // Fill the mafObjectsList given with the all registered objects.
    mafRegistryHashType::const_iterator iter = m_Registry.constBegin();
    while(iter != m_Registry.constEnd()) {
        objects->append(iter.value().m_Object);
        ++iter;
    }
}

mafObjectsList *mafObjectRegistry::findObjectsThreaded(mafVisitorFindObjects *v) {
    // Initialize the max thread count depending on the number of core present into the PC.
    int idealThreadCount = mafThread::idealThreadCount();
    mafThreadPool::globalInstance()->setMaxThreadCount(idealThreadCount);
    // Use the QtConcurrent framework to run the function in a separate thread.
    mafFuture<mafObjectsList *> future = mafConcurrent::run(this, &mafCore::mafObjectRegistry::findObjects, v);
    // Wait that the search is performed.
    future.waitForFinished();
    // Return the search result to the caller.
    return future.result();
}

mafObjectsList *mafObjectRegistry::findObjects(mafVisitorFindObjects *v) {
    // Following the visitor pattern search for objects given the visitor search algorithm.
    mafRegistryHashType::const_iterator iter = m_Registry.constBegin();
    v->objectsList()->clear();
    while(iter != m_Registry.constEnd()) {
        iter.value().m_Object->acceptVisitor(v);
        ++iter;
    }
    return v->objectsList();
}

mafObjectBase *mafObjectRegistry::objectFromHash(const mafString &hash) {
    mafVisitorFindObjectsByHashCode *v = new mafVisitorFindObjectsByHashCode(hash , mafCodeLocation);

    mafObjectsList l = *(findObjectsThreaded(v));
    mafDEL(v);

    ENSURE(l.count() == 0 || l.count() == 1);
    return l.count() == 0 ? NULL : l.at(0);
}
