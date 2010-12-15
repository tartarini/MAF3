/*
 *  mafCoreRegistration.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreRegistration.h"

using namespace mafCore;

mafMap<mafString, mafCoreRegistration::acceptObjectPointer> mafCoreRegistration::m_BindingMap = mafBindingMap();

mafStringList mafCoreRegistration::acceptObject(mafCore::mafObjectBase *obj) {
    mafBindingMap::iterator iter = m_BindingMap.begin();
    mafBindingMap::iterator iter_end = m_BindingMap.end();
    mafStringList res_list;
    while(iter != iter_end) {
        if(iter.value()(obj)) {
            res_list.append(iter.key());
        }
        iter++;
    }
    return res_list;
}

void mafCoreRegistration::unregisterUnbind( const mafString& className ) {
    m_BindingMap.remove(className);
}
