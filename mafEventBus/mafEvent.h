/*
 *  mafEvent.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009-2011 B3C. All rights reserved.
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
    mafEvent(QString topic, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, QString signature);

    /// Allow to assign the event type: mafEventTypeLocal or mafEventTypeRemote.
    void setEventType(mafEventType et);

    /// Return the type of the event: mafEventTypeLocal or mafEventTypeRemote.
    mafEventType eventType() const;

    /// Check if the event is local or not.
    bool isEventLocal() const;

    /// Allow to set or modify the event ID
    void setEventTopic(QString topic);

    /// Return the Id associated with the event.
    QString eventTopic() const;

    /// Return the name associated to the numeric Id.
    //QString eventIdName() const;

    /// Redefined operator to have access to the entries owned.
    mafEventHash *entries();

    /// Redefined operator to have access to the entries owned.
    mafEventHash *entries() const;

    /// Overload operator for rapid access to mafDictionaryEntries
    QVariant &operator[](QString key) const;

private:
    mafEventHash *m_EventHash;
};

typedef mafEvent * mafEventPointer;

} // namespace mafEventBus

Q_DECLARE_METATYPE(mafEventBus::mafEventPointer);

#endif // MAFEVENT_H
