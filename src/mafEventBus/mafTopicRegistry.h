/*
 *  mafTopicRegistry.h
 *  mafEventBus
 *
 *  Created by Roberto Mucci on 26/01/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#ifndef MAFTOPICREGISTRY_H
#define MAFTOPICREGISTRY_H

// Includes list
#include "mafEventDefinitions.h"

namespace mafEventBus {

/**
  Class name: mafTopicRegistry
  This singletone provides the registration of topic and topic owner in a hash.
*/
class MAFEVENTBUSSHARED_EXPORT mafTopicRegistry {
public:
    /// Return an instance of the event bus.
    static mafTopicRegistry *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// Register a new topic.
    bool registerTopic(const QString topic, const QObject *owner);

    /// Unregister a topic.
    bool unregisterTopic(const QString topic);

    /// Return the owner of a topic.
    const QObject *owner(const QString topic) const;

    /// Check if a topic is present in the topic hash.
    bool isTopicRegistered(const QString topic) const;

    /// Dump of the topic hash.
    void dump();

private:
    /// Object constructor.
    mafTopicRegistry();

    QHash<QString, const QObject*> m_TopicHash; ///< Hash containing pairs (topic,owner).
};

} //nameSpace mafEventBus

#endif // MAFTOPICREGISTRY_H
