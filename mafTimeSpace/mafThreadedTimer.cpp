/*
 *  mafThreadTimer.cpp
 *  mafTimeSpace
 *
 *  Created by Daniele Giunchi on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafThreadedTimer.h"
#include "mafTimer.h"

using namespace mafCore;
using namespace mafTimeSpace;

mafThreadedTimer::mafThreadedTimer(const mafString code_location): mafObjectBase(code_location) {
}

mafThreadedTimer::mafThreadedTimer(mafTimer *timer, const mafString code_location): mafObjectBase(code_location) , m_Timer(timer), m_Thread(NULL) {
}

mafThreadedTimer::~mafThreadedTimer() {
    // If the timer has been moved over a different thread
    // before deleting it, the thread has to be exited and deleted.
    if(m_Thread != NULL) {
        m_Thread->exit(0);
#ifdef WIN32
        delete m_Thread;
        m_Thread = NULL;
#else
        m_Thread->deleteLater();
#endif
    }
    // Delete the timer.
    mafDEL(m_Timer);
}

void mafThreadedTimer::start() {
    REQUIRE(m_Timer != NULL);

    m_Timer->start();
}

void mafThreadedTimer::startOnThread(mafThread *thread) {
    REQUIRE(m_Timer != NULL);
    REQUIRE(thread != NULL);

    m_Thread = thread;

    // Connect the 'started' signal emitted from the thread to the 'start' slot of the timer.
    // This allow starting the timer when the thread starts.
    connect(m_Thread, SIGNAL(started()), m_Timer, SLOT(start()));
    if(m_Timer->isSingleShot()) {
        connect(m_Timer, SIGNAL(timeoutReached()), m_Thread, SLOT(quit()));
    }
    m_Timer->moveToThread(m_Thread);
    m_Thread->start();
}

void mafThreadedTimer::stop() {
    m_Timer->stop();
    if(m_Thread != NULL) {
        m_Thread->quit();
    }
}

bool mafThreadedTimer::isActive() const {
    return m_Timer->isActive();
}
