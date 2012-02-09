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

void mafObjectRegistry::addObject(mafObjectBase *obj, const QString location) {
    // Create the registry item with object's information
    mafObjectRegistryItem ri;
    ri.m_Object = obj;
    ri.m_InstantiateLocationInfo = location;
    ri.m_AllocationTime = QTime::currentTime();
    ri.m_ReferenceCount = obj->referenceCount();

    // Insert the item into the registry.
    m_Registry.insert(obj->objectId(), ri);
}

void mafObjectRegistry::removeObject(mafId obj_id) {
    m_Registry.remove(obj_id);
}

void mafObjectRegistry::dumpLiveObjects() {
    // Dump on standard console the objects that still are present into the registry (memory leaks!!)
    unsigned int totalReferenceCountLeakedObjects = 0;
    unsigned int totalLeakedObjects = m_Registry.size();
    mafRegistryHashType::const_iterator iter = m_Registry.constBegin();
    while(iter != m_Registry.constEnd()) {
        QString name = iter.value().m_Object->metaObject()->className();
        QString obj_name = iter.value().m_Object->objectName();
        QString loc = iter.value().m_InstantiateLocationInfo;
        QTime t = iter.value().m_AllocationTime;
        unsigned int refCount = iter.value().m_ReferenceCount;
        totalReferenceCountLeakedObjects += refCount;
        qDebug() << "class name: " << name 
                  << ", obj name: " << obj_name
                  << ", refCount: " << refCount 
                  << ", allocated by: " << loc
                  << ", at time: " << t.toString("hh:mm:ss");
        ++iter;
    }
    
    qDebug() << "Total Number of Leaked Objects: " << totalLeakedObjects;
    qDebug() << "Total Number of Leaked Objects Reference counts: " << totalReferenceCountLeakedObjects;
}

void mafObjectRegistry::liveObjects(QObjectList *objects) {
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

void mafObjectRegistry::applyVisitorToObjectListThreaded(mafVisitor *v, mafObjectsList *objectList) {
    // Initialize the max thread count depending on the number of core present into the PC.
    int idealThreadCount = QThread::idealThreadCount();
    QThreadPool::globalInstance()->setMaxThreadCount(idealThreadCount);
    // Use the QtConcurrent framework to run the function in a separate thread.
    QFuture<void> future = QtConcurrent::run(this, &mafCore::mafObjectRegistry::applyVisitorToObjectList, v, objectList);
    // Wait that the search is performed.
    future.waitForFinished();
}

void mafObjectRegistry::applyVisitorToObjectList(mafVisitor *v, mafObjectsList *objectList) {
    mafObjectsList::const_iterator iter = objectList->constBegin();
    while(iter != objectList->constEnd()) {
        QObject *obj = iter.value();
        mafObjectBase *objBase = qobject_cast<mafObjectBase*>(obj);
        if (objBase) {
            objBase->acceptVisitor(v);
        }
        ++iter;
    }
}

mafObjectsList *mafObjectRegistry::findObjectsThreaded(mafVisitorFindObjects *v) {
    // Initialize the max thread count depending on the number of core present into the PC.
    int idealThreadCount = QThread::idealThreadCount();
    QThreadPool::globalInstance()->setMaxThreadCount(idealThreadCount);
    // Use the QtConcurrent framework to run the function in a separate thread.
    QFuture<mafObjectsList *> future = QtConcurrent::run(this, &mafCore::mafObjectRegistry::findObjects, v);
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
        QObject *obj = iter.value().m_Object;
        mafObjectBase *objBase = qobject_cast<mafObjectBase*>(obj);
        if (objBase) {
            objBase->acceptVisitor(v);
        }
        ++iter;
    }
    return v->objectsList();
}

mafObjectBase *mafObjectRegistry::objectFromHash(const QString &hash) {
    mafVisitorFindObjectsByHashCode *v = new mafVisitorFindObjectsByHashCode(hash , mafCodeLocation);

    mafObjectsList l = *(findObjectsThreaded(v));
    mafDEL(v);

    ENSURE(l.count() == 0 || l.count() == 1);
    return l.count() == 0 ? NULL : l.at(0);
}
