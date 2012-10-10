/*
 *  mafTimeManager.h
 *  mafTimeSpace
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFTIMEMANAGER_H
#define MAFTIMEMANAGER_H

// Includes list
#include "mafTimeSpace_global.h"
#include <mafObjectBase.h>

namespace mafTimeSpace {

// Class forwarding list

/**
 Class name: mafTimeManager
 This singletone provides the facade class for time managing. The class define some IDs to deal with created timers:
 - maf.local.time.create allows you to create a new timer.
 - maf.local.time.destroy allows to destroy the timer associated with an ID passed as argument.
 - maf.local.time.start allows to start a timer associated with the given ID in a separated thread (or in the main thread).
 - maf.local.time.isRunning allow to check if the timer asociated with the given ID is running or no.
 - maf.local.time.stop allow to stop the timer corresponding to the given ID.
 */
class MAFTIMESPACESHARED_EXPORT mafTimeManager : mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Return an instance of the Time Manager.
    static mafTimeManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// fill the given string list with the timer types.
//    void timerTypes(QStringList *timer_types);

Q_SIGNALS:
    /// Signal needed to create the timer.
    mafCore::mafId createTimerSignal(int interval, bool one_shot = true);

    /// Signal that is able to destroy the timer associated to the given ID.
    void destroyTimerSignal(mafCore::mafId id);

    /// Signal that is able to start the timer asociated with the given ID. The timer can be started also in a separated thread.
    void startTimerSignal(mafCore::mafId id, bool separate_thread = true);

    /// Signal that allows to check if the timer corresponding to the given id is running or not.
    bool isRunningSignal(mafCore::mafId timer_id);

    /// Signal that allows to stop the timer associated to the Event_Id given as argument. The id is returned by the createTimer method.
    void stopTimerSignal(mafCore::mafId id);

private Q_SLOTS:
    /// Create a new timer and connect through the mafEventBus the timer signal to the object's callback signature.
    /** This method will create a new timer for the requestor. The new timer will be connected to the requestor's callback (slot)
    and when the timer reach the timeout, the event will be raised. The method return the event_id associated to the timer created
    that can be used to stop it.*/
//    mafCore::mafId createTimer(int interval, mafCore::mafObjectBase *requestor, QString callback_signature, bool one_shot = true);

    /// Create a new timer with the given time interval (in milliseconds) and repeat behvior (one shot or no).
    /** This method create the timer with the given information for time interval and repeat behavior. The returned
    value represent the timer's id with which the caller has to register through the event bus to be notified about the
    timeout or to deal with the timer itself.*/
    mafCore::mafId createTimer(int interval, bool one_shot = true);

    /// Destroy the timer associated to the Event_Id given as argument. The id is returned by the createTimer method.
    void destroyTimer(mafCore::mafId id);

    /// Start the timer associated to the Event_Id given as argument. The id is returned by the createTimer method.
    void startTimer(mafCore::mafId id, bool separate_thread = true);

    /// Check that the timer corresponding to the given id is running or not.
    bool isRunning(mafCore::mafId timer_id);

    /// Stop the timer associated to the Event_Id given as argument. The id is returned by the createTimer method.
    void stopTimer(mafCore::mafId id);

private:
    /// Object constructor.
    mafTimeManager(const QString code_location = "");

    /// Object destructor.
    ~mafTimeManager();

    /// Initialize the signals / slots connection and generate the IDs needed for the communication with the Time & Space Module.
    void initializeConnections();

    /// Stop all running timers (included the system clock).
    void stopAllTimers();

    mafTimerHash m_TimerHash; ///< Hash that contains the created timers and the corresponding Event_Id associated.
};

} // namespace mafTimeSpace

#endif // MAFTIMEMANAGER_H
