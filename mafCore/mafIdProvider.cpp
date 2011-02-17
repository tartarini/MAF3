/*
 *  mafIdProvider.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafIdProvider.h"
#include "mafObjectRegistry.h"

using namespace mafCore;


mafIdProvider::mafIdProvider() {
    m_Id = 0;
}

mafIdProvider::~mafIdProvider() {
    m_Id = -1; // Invalid ID.
    m_IdHash.clear();
}

mafIdProvider* mafIdProvider::instance() {
    static mafIdProvider instanceProvider;
    return &instanceProvider;
}

mafId mafIdProvider::createNewId(const mafString id_name) {
    if(id_name.isEmpty()) {
        mafString name;
        name.append("OBJECT_ID_");
        name.append(mafString::number(m_Id));
        m_IdHash.insert(m_Id, name);
    } else {
        // ID with custom name: check if not already inserted!! No multiple names are accepted.
        mafId id_value = idValue(id_name);
        if(id_value == -1) {
            m_IdHash.insert(m_Id, id_name);
        } else {
            // if the Event id is already defined, return the previous defined ID value.
            mafMsgWarning("%s", mafTr("ID with name '%1'' has been already defined!").arg(id_name).toAscii().data());
            return id_value;
        }
    }
    ++m_Id;
    return m_Id;
}

bool mafIdProvider::removeId(const mafString id_name) {
    int removed_items = 0;
    mafId id_value = idValue(id_name);
    if(m_IdHash.contains(id_value)) {
        removed_items = m_IdHash.remove(id_value);
    }
    return removed_items != 0;
}

bool mafIdProvider::setIdName(const mafId id, const mafString id_name) {
    if(m_IdHash.contains(id) && idValue(id_name) == -1) {
        // id exists and name not yet used, so can be assigned to the id.
        m_IdHash.insert(id, id_name);
        return true;
    }
    return false;
}

const mafString mafIdProvider::idName(const mafId id_value) const {
    return m_IdHash.value(id_value);
}

mafId mafIdProvider::idValue(const mafString id_name) const {
    return m_IdHash.key(id_name, -1);
}

void mafIdProvider::shutdown() {
    m_IdHash.clear();
}
