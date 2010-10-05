/*
 *  mafEvent.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009-2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEVENT_H
#define MAFEVENT_H

#include "mafEventDefinitions.h"

namespace mafEventBus {

 /**
Class name: mafEvent
This class defines the MAF3 Event which inherit from mafDictionary, and contains
constructor for rapid dictionary creation.
@sa mafDictionary
*/
class MAFEVENTBUSSHARED_EXPORT mafEvent : public QObject {

public:
    /// Object constructor.
    mafEvent();

    /// Object destructor.
    ~mafEvent();

    /// Overload object constructor.
    mafEvent(mafString topic, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, mafString signature);

    /// Allow to assign the event type: mafEventTypeLocal or mafEventTypeRemote.
    void setEventType(mafEventType et);

    /// Return the type of the event: mafEventTypeLocal or mafEventTypeRemote.
    mafEventType eventType() const;

    /// Check if the event is local or not.
    bool isEventLocal() const;

    /// Allow to set or modify the event ID
    void setEventTopic(mafString topic);

    /// Return the Id associated with the event.
    mafString eventTopic() const;

    /// Return the name associated to the numeric Id.
    //mafString eventIdName() const;

    /// Redefined operator to have access to the entries owned.
    mafEventHash *entries();

    /// Redefined operator to have access to the entries owned.
    mafEventHash *entries() const;

    /// Overload operator for rapid access to mafDictionaryEntries
    mafVariant &operator[](mafString key) const;

private:
    mafEventHash *m_EventHash;
};

typedef mafEvent * mafEventPointer;

inline mafEventHash *mafEvent::entries() {
    return m_EventHash;
}

inline mafEventHash *mafEvent::entries() const {
    return m_EventHash;
}

inline mafVariant &mafEvent::operator[](mafString key) const{
    return (*m_EventHash)[key];
}

inline mafEventType mafEvent::eventType() const {
    return static_cast<mafEventType>(entries()->value(TYPE).toInt());
}

inline mafString mafEvent::eventTopic() const {
    return entries()->value(TOPIC).toString();
}

inline bool mafEvent::isEventLocal() const {
    int et = entries()->value(TYPE).toInt();
    return et == mafEventTypeLocal;
}

inline void mafEvent::setEventType(mafEventType et) {
    entries()->insert(TYPE, static_cast<int>(et));
}

inline void mafEvent::setEventTopic(mafString topic) {
    entries()->insert(TOPIC, topic);
}

/*inline mafString mafEvent::eventIdName() const {
    mafId id = eventId();
    return mafIdProvider::instance()->idName(id);
}*/

} // namespace mafEventBus

Q_DECLARE_METATYPE(mafEventBus::mafEventPointer);

#endif // MAFEVENT_H
