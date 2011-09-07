/*
 *  mafTimer.h
 *  mafTimeSpace
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTIMER_H
#define MAFTIMER_H

// Includes list
#include "mafTimeSpace_global.h"
#include <mafObjectBase.h>
#include <QTimer>

namespace mafTimeSpace {

// Class forwarding list

/**
 Class name: mafTimer
 This class provides the wrapper for the QTimer necessary to send events through the Event Bus.
 By default the constructed timer is single shot. To enable cyclic timing use setSingleShot method and
 pass to it a 'false' boolean value. All the timing value (intervals) are expressed in milliseconds.
 @sa mafEventBus
 */
class MAFTIMESPACESHARED_EXPORT mafTimer : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafTimer(const QString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Set the timer interval (in msec).
    void setInterval(int interval);

    /// Return the timer interval in milliseconds.
    int interval();

    /// Set the flag for single shot timer or cyclic timer.
    void setSingleShot(bool one_shot);

    /// Return true if the timer is active.
    bool isActive() const;

    /// Return true if the timer has been set as single shot timer.
    bool isSingleShot() const;

    /// Start the timer with the given interval (in msec)..
    void start(int interval);

    /// Stop the timer.
    void stop();

Q_SIGNALS:
    /// Signal emitted when the inner QTimer rise the timeout event.
    void timeoutReached();

public Q_SLOTS:
    /// method that will be called by QTimer whe the timeout is reached.
    void update();

    /// Start the timer with the predefined interval (via setInterval).
    void start();

protected:
    /// Object destructor.
    /* virtual */ ~mafTimer();

private:
    QTimer *m_Timer; ///< Wrapped timer instance.
    int m_TimerId; ///< ID identifing the running timer.
    QString m_ObjectId; ///< ID identifing the object to be notified.
};

inline int mafTimer::interval() {
    return m_Timer->interval();
}

inline bool mafTimer::isSingleShot() const {
    return m_Timer->isSingleShot();
}

} // namespace mafTimeSpace

#endif // MAFTIMER_H
