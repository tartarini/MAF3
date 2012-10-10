/*
 *  mafEvent.h
 *  mafEventBus
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009-2011 SCS-B3C. All rights reserved.
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
This class defines the MAF3 Event which inherit from QVariantHash, and contains
constructor for rapid dictionary creation.
*/
class MAFEVENTBUSSHARED_EXPORT mafEvent : public QObject {

public:
    /// Object constructor.
    mafEvent();

    /// Object destructor.
    ~mafEvent();

    /// Overload object constructor.
    mafEvent(QString topic, mafEventType event_type, mafSignatureType signature_type, QObject *objectPointer, QString signature, bool synchronous = true);
    
    /// Overload object constructor.
    mafEvent(QString topic, mafEventType event_type = mafEventTypeLocal, bool synchronous = true);

    /// Allow to assign the event type: mafEventTypeLocal or mafEventTypeRemote.
    void setEventType(mafEventType et);

    /// Return the type of the event: mafEventTypeLocal or mafEventTypeRemote.
    mafEventType eventType() const;

    /// Check if the event is local or not.
    bool isEventLocal() const;
    
    bool isSynchronous() const;

    /// Allow to set or modify the event ID
    void setEventTopic(QString topic);

    /// Return the Id associated with the event.
    QString eventTopic() const;

    /// Return the name associated to the numeric Id.
    //QString eventIdName() const;

    /// Redefined operator to have access to the entries owned.
    QVariantHash *entries();

    /// Redefined operator to have access to the entries owned.
    QVariantHash *entries() const;

    /// Overload operator for rapid access to QVariantHash Entries
    QVariant &operator[](QString key) const;
    
    /// Allows to clear the parameters list.
    void clearArgList();
    
    /// Allows to add a parameter to the event call.
    void addParameter(QGenericArgument arg);
    
    /// Allows to set the return value.
    void setReturnValue(QGenericReturnArgument retVal);
    
    /// Return the argument list
    const mafEventArgumentsListPointer argumentList() const;
    
    /// Return the pointer to the return value
    const QGenericReturnArgument *returnValue() const;
    
private:
    QVariantHash *m_EventHash; ///< Hash containing the event information.
    bool m_SynchronousEvent; ///< Flag indicating if the event has to be sent as Synchronous (the defaule) or Asynchronous.
    
    mafEventArgumentsList m_ArgList; ///< List of arguments passed to the called function.
    QGenericReturnArgument m_ReturnValue; ///< Optional return value returned by the called function.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////
    
inline bool mafEvent::isSynchronous() const {
    return m_SynchronousEvent;
}

inline void mafEvent::addParameter(QGenericArgument arg) {
    m_ArgList.append(arg);
}

inline void mafEvent::setReturnValue(QGenericReturnArgument retVal) {
    m_ReturnValue = retVal;
}

inline const mafEventArgumentsListPointer mafEvent::argumentList() const {
    const mafEventArgumentsListPointer constArgList = (const mafEventArgumentsListPointer)&m_ArgList;
    return constArgList;
}

inline const QGenericReturnArgument *mafEvent::returnValue() const {
    const QGenericReturnArgument *constRetVal = (const QGenericReturnArgument *)&m_ReturnValue;
    return constRetVal;
}
    
typedef mafEvent * mafEventPointer;

} // namespace mafEventBus

Q_DECLARE_METATYPE(mafEventBus::mafEventPointer);

#endif // MAFEVENT_H
