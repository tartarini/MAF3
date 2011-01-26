/*
 *  mafTopicRegistry.cpp
 *  mafEventBus
 *
 *  Created by Roberto Mucci on 26/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTopicRegistry.h"

using namespace mafEventBus;

mafTopicRegistry* mafTopicRegistry::instance() {
    static mafTopicRegistry instanceTopicRegistry;
    return &instanceTopicRegistry;
}

mafTopicRegistry::mafTopicRegistry() {
}

void mafTopicRegistry::shutdown() {
    m_TopicHash.clear();
}

bool mafTopicRegistry::registerTopic(const mafString topic, const QObject *owner) {
    if(topic.isEmpty() || owner == NULL ){
        return false;
    }
    if(m_TopicHash.contains(topic)){
        //topic already registered
        mafMsgWarning() << mafTr("Topic %1 already owned by %2").arg(topic, owner->metaObject()->className());
        return false;
    }
    m_TopicHash.insert(topic,owner);
    return true;
}

const QObject *mafTopicRegistry::owner(const mafString topic) const {
    return m_TopicHash.value(topic,NULL);
}

bool mafTopicRegistry::isTopicRegistered(const mafString topic) const {
    return m_TopicHash.contains(topic);
}

void mafTopicRegistry::dump() {
    mafHash<mafString, const QObject*>::const_iterator i = m_TopicHash.constBegin();
    while (i != m_TopicHash.constEnd()) {
        const QObject *obj = i.value();
        mafMsgDebug() << "Topic: " << i.key() << "   Owner: " << obj->metaObject()->className();
        ++i;
    }
}
