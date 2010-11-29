/*
 *  mafTimeManager.cpp
 *  mafTimeSpace
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTimeManager.h"
#include "mafTimer.h"
#include "mafThreadedTimer.h"

using namespace mafTimeSpace;
using namespace mafCore;
using namespace mafEventBus;

mafTimeManager::mafTimeManager(const mafString code_location) : mafObjectBase(code_location) {
    mafRegisterObject(mafTimeSpace::mafThreadedTimer);
    mafRegisterObject(mafTimeSpace::mafTimer);

    initializeConnections();
}

mafTimeManager::~mafTimeManager() {
}

void mafTimeManager::initializeConnections() {
    // Create IDs for deal with timer's creation and start/stop.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.time.create");
    provider->createNewId("maf.local.time.destroy");
    provider->createNewId("maf.local.time.start");
    provider->createNewId("maf.local.time.isRunning");
    provider->createNewId("maf.local.time.stop");


    // Register signals to the event bus.
    mafRegisterLocalSignal("maf.local.time.create", this, "createTimerSignal(int, bool)");
    mafRegisterLocalSignal("maf.local.time.destroy", this, "destroyTimerSignal(mafCore::mafId)");
    mafRegisterLocalSignal("maf.local.time.start", this, "startTimerSignal(mafCore::mafId , bool)");
    mafRegisterLocalSignal("maf.local.time.isRunning", this, "isRunningSignal(mafCore::mafId)");
    mafRegisterLocalSignal("maf.local.time.stop", this, "stopTimerSignal(mafCore::mafId)");

    // ... and connect the callbacks.
    mafRegisterLocalCallback("maf.local.time.create", this, "createTimer(int, bool)");
    mafRegisterLocalCallback("maf.local.time.destroy", this, "destroyTimer(mafCore::mafId)");
    mafRegisterLocalCallback("maf.local.time.start", this, "startTimer(mafCore::mafId, bool)");
    mafRegisterLocalCallback("maf.local.time.isRunning", this, "isRunning(mafCore::mafId)");
    mafRegisterLocalCallback("maf.local.time.stop", this, "stopTimer(mafCore::mafId)");
}

mafTimeManager* mafTimeManager::instance() {
    static mafTimeManager instanceTimeManager;
    return &instanceTimeManager;
}

void mafTimeManager::shutdown() {
    mafTimerHash::iterator i;
    for (i = m_TimerHash.begin(); i != m_TimerHash.end(); ++i) {
        mafThreadedTimer *threadedTimer = i.value();
        mafDEL(threadedTimer);
    }
    m_TimerHash.clear();
}

mafId mafTimeManager::createTimer(int interval, bool one_shot) {
    REQUIRE(interval > 0);

    // Create the new timer...
    mafTimer *t = mafNEW(mafTimeSpace::mafTimer);
    t->setInterval(interval);
    t->setSingleShot(one_shot);
    mafId t_id = t->objectId();

    // Insert it in the mafTimerThreaded clas, so you can start the timer
    // in the main thread or in a separated thread.
    mafThreadedTimer *threadedTimer = new mafThreadedTimer(t, mafCodeLocation);
    // Add the new timer to the hash...
    m_TimerHash.insert(t_id, threadedTimer);
    return t_id;
}

/*mafId mafTimeManager::createTimer(int interval, mafObjectBase *requestor, mafString callback_signature, bool one_shot) {
    REQUIRE(interval > 0);
    REQUIRE(requestor != NULL);
    REQUIRE(callback_signature.length() > 0);

    // Create the new timer...
    mafTimer *t = mafNEW(mafTimeSpace::mafTimer);
    t->setInterval(interval);
    t->setSingleShot(one_shot);
    mafId t_id = t->objectId();

    // Register the requestor's callback to be called through the EventBus...
    mafRegisterLocalCallback(t_id, requestor, callback_signature);

    // Insert it in the mafTimerThreaded clas, so you can start the timer
    // in the main thread or in a separated thread.
    mafThreadedTimer *threadedTimer = new mafThreadedTimer(t, mafCodeLocation);
    // Add the new timer to the hash...
    m_TimerHash.insert(t_id, threadedTimer);
    return t_id;
}*/

void mafTimeManager::destroyTimer(mafId id) {
    REQUIRE(id > 0);
    if(m_TimerHash.contains(id)) {
        mafThreadedTimer *threadedTimer = m_TimerHash.value(id);
        mafDEL(threadedTimer);
        m_TimerHash.remove(id);
    }
}

void mafTimeManager::startTimer(mafId id, bool separate_thread) {
    REQUIRE(id > 0);
    if(m_TimerHash.contains(id)) {
        mafThreadedTimer *threadedTimer = m_TimerHash.value(id);
        if(separate_thread) {
            // Manage thread creation and move the timer on it.
            mafThread *thread =  new mafThread();
            threadedTimer->startOnThread(thread);
        } else {
            threadedTimer->start();
        }
    }
}

void mafTimeManager::stopTimer(mafId id) {
//    REQUIRE(id > 0);
    if(id < 0) {
        stopAllTimers();
        return;
    }

    if(m_TimerHash.contains(id)) {
        m_TimerHash.value(id)->stop();
    }
}

bool mafTimeManager::isRunning(mafId timer_id) {
    REQUIRE(timer_id >= 0);
    if(m_TimerHash.contains(timer_id)) {
        return m_TimerHash.value(timer_id)->isActive();
    }
    return false;
}

void mafTimeManager::stopAllTimers() {
    mafTimerHash::iterator i;
    for (i = m_TimerHash.begin(); i != m_TimerHash.end(); ++i) {
        i.value()->stop();
    }
}
