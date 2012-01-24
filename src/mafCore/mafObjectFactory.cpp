/*
 *  mafObjectFactory.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafObjectFactory.h"

using namespace mafCore;


mafObjectFactory* mafObjectFactory::instance() {
    static mafObjectFactory instanceObjectFactory;
    return &instanceObjectFactory;
}


mafObjectFactory::~mafObjectFactory() {
    qDeleteAll( m_ObjectMap.values() );
}

void mafObjectFactory::shutdown() {

}

bool mafObjectFactory::unregisterObject( const QString& className ) {
    // The return value of the 'remove' operation is the number of object removed.
    int res = m_ObjectMap.remove(className);
    return res != 0;
}

bool mafObjectFactory::isObjectRegistered( const QString &className ) {
    return m_ObjectMap.contains(className);
}

mafObjectBase *mafObjectFactory::instantiateObjectBase( const QString &className, const QString location) {
    // Check the existence of object type into the hash and then return its instance.
    if (isObjectRegistered(className)) {
        return m_ObjectMap.value(className)->make(location);
    } else {
        qWarning() << mafTr("Can not create object instance of type: ") << className;
        return NULL;
    }
}

QObject *mafObjectFactory::instantiateQtObject( const QString &className, const QString location ) {
    // Check the existence of object type into the hash and then return its instance.
    if (isObjectRegistered(className)) {
        QObject *obj = m_ObjectMap.value(className)->make();
        //mafObjectRegistry::instance()->addObject(obj, location);
        return obj;
    } else {
        qWarning() << mafTr("Can not create object instance of type: ") << className;
        return NULL;
    }
}