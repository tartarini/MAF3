/*
 *  mafObjectBase.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafObjectBase.h"
#include "mafIdProvider.h"
#include "mafObjectRegistry.h"
#include "mafVisitor.h"

using namespace mafCore;

mafObjectBase::mafObjectBase(const mafString code_location) : QObject() {
    mafIdProvider *provider = mafIdProvider::instance();
    m_ObjectId = provider->createNewId();

    mafObjectRegistry::instance()->addObject(this, code_location);

    m_ObjectHash = QUuid::createUuid();
//    mafString id_str = id.toString();
//    createHashCode(id_str);
}

mafObjectBase::~mafObjectBase() {
    // need to anticipate the 'destroyed()' signal from QObject
    // to allws mafObjectRegitry to remove it from the hash, otherwise
    // if the signal is sent from the parent QObject, this subclas has
    // been already destroyed and its m_ObjectId is not more valid.
    mafObjectRegistry::instance()->removeObject(m_ObjectId);
    m_ObjectId = -1;
}

bool mafObjectBase::isEqual(const mafObjectBase *obj) const {
    const QMetaObject* my_meta = metaObject();

    const QMetaObject* obj_meta = obj->metaObject();
    int i = 0;
    int num = obj_meta->propertyCount();
    if(num != my_meta->propertyCount()) {
        return false;
    }
    for ( ; i < num; ++i) {
        const QMetaProperty obj_qmp = obj_meta->property(i);
        const QMetaProperty my_qmp = my_meta->property(i);
        mafString obj_name = obj_qmp.name();
        if(obj_name == "objectHash") {
            continue;
        }
        mafVariant obj_value = obj->property(obj_name.toAscii());
        mafString my_name = my_qmp.name();
        mafVariant my_value = property(my_name.toAscii());
        if((my_name != obj_name) || (my_value != obj_value)) {
            return false;
        }
    }

    return true;
}

bool mafObjectBase::isObjectValid() const {
    return m_ObjectId != -1;
}

void mafObjectBase::acceptVisitor(mafVisitor *v) {
    // Design by contract condition.
    REQUIRE(v != NULL);

    v->visit(this);
}

//void mafObjectBase::createHashCode(mafString &token) {
//    m_ObjectHash = QCryptographicHash::hash(token.toLatin1(), QCryptographicHash::Sha1);
//}
