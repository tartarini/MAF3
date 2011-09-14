/*
 *  mafObject.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#include "mafObject.h"
#include "mafObjectFactory.h"
#include "mafMementoObject.h"
#include "mafVisitor.h"

using namespace mafCore;

mafObject::mafObject(const QString code_location) : mafObjectBase(code_location), m_IconFile(""), m_LockStatus(mafObjectLockNone), m_ProgressStatus(0),  m_TagList(NULL), m_Dictionary(NULL) {
    m_TagList = new QVariantList();
    m_ScriptList = new QVariantList();
    m_Dictionary = new mafDictionary;
}

mafObject::~mafObject() {
    delete m_TagList;
    delete m_ScriptList;
    delete m_Dictionary;
}

void mafObject::setIconFile(QString icon_file) {
    m_IconFile = icon_file;
}

mafMemento *mafObject::createMemento() const {
    return new mafMementoObject(this, mafCodeLocation);
}

void mafObject::setMemento(mafMemento *memento, bool deep_memento) {
    // Design by contract condition.
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    mafMementoPropertyItem item;
    Q_FOREACH(item, *memento->mementoPropertyList()) {
        QString name = item.m_Name;
        if(name != "objectHash" || deep_memento) {
            if( name.compare("TagList") == 0 ) {
                setTagList(item.m_Value.toList());
            } else if ( name.compare("dictionary") == 0 ) {
                dictionary()->clear();
                dictionary()->unite(item.m_Value.toHash());
            } else if (name.compare("ScriptList") == 0) {
                setScriptList(item.m_Value.toList());
            } else {
                this->setProperty(name.toAscii(), item.m_Value);
            }
        }
    }
}

bool mafObject::isObjectValid() const {
    return Superclass::isObjectValid() && (m_TagList != NULL) && (m_ScriptList != NULL);
}

bool mafObject::addTag(QString tag) {
    if(!m_TagList->contains(tag)) {
        m_TagList->append(tag);
        return true;
    }
    return false;
}

bool mafObject::removeTag(QString tag) {
    return m_TagList->removeOne(tag);
}

void mafObject::filterTags(QString tag_pattern, QVariantList *filtered_list) {
    filtered_list->clear();
    QVariant temp;
    Q_FOREACH(temp, *m_TagList) {
        if(temp.toString().contains(tag_pattern)) {
            filtered_list->append(temp);
        }
    }
}

bool mafObject::addScript(QString script) {
    if(!m_ScriptList->contains(script)) {
        m_ScriptList->append(script);
        return true;
    }
    return false;
}

bool mafObject::removeScript(QString script) {
    return m_ScriptList->removeOne(script);
}

bool mafObject::removeScript(int scriptIndex) {
    int s = m_ScriptList->size();
    m_ScriptList->removeAt(scriptIndex);
    return s > m_ScriptList->size();
}

bool mafObject::isEqual(const mafObjectBase *obj) const {
    if(Superclass::isEqual(obj)) {
        bool result = true;
        int i=0, size = m_TagList->size();
        for(; i < size; ++i) {
            if(m_TagList->at(i) != ((mafObject *)obj)->tagList()->at(i)) {
                result = false;
                break;
            }
        }
        if (result) {
            i=0;
            size = m_ScriptList->size();
            for(; i < size; ++i) {
                if(m_ScriptList->at(i) != ((mafObject *)obj)->scriptList()->at(i)) {
                    result = false;
                    break;
                }
            }
        }
        return result;
    }
    return false;
}

void mafObject::setTagList(const QVariantList &list) {
    m_TagList->clear();
    m_TagList->append(list);
}

void mafObject::setScriptList(const QVariantList &list) {
    m_ScriptList->clear();
    m_ScriptList->append(list);
}

void mafObject::setProgressStatus(int status) {
    m_ProgressStatus = status;
}
