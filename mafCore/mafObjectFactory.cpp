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

void mafObjectFactory::shutdown() {

}

bool mafObjectFactory::unregisterObject( const mafString& className ) {
    // The return value of the 'remove' operation is the number of object removed.
    int res = m_ObjectMap.remove(className);
    return res != 0;
}

bool mafObjectFactory::isObjectRegistered( const mafString &className ) {
    return m_ObjectMap.contains(className);
}

mafObjectBase *mafObjectFactory::instantiateObjectBase( const mafString &className, const mafString location) {
    // Check the existence of object type into the hash and then return its instance.
    if (isObjectRegistered(className)) {
        return m_ObjectMap.value(className)->make(location);
    } else {
        return NULL;
    }
}

