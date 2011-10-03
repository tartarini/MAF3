/*
 *  mafThreadSynchronizer.cpp
 *  mafCore
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 03/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */


#include "mafThreadSynchronizer.h"

using namespace mafCore;

mafThreadSynchronizer *mafThreadSynchronizer::m_Instance = NULL;

mafThreadSynchronizer *mafThreadSynchronizer::instance(void) {
    static QMutex mutex;

    mutex.lock();
    if(!m_Instance) {
	    m_Instance = new mafThreadSynchronizer;
    }
    mutex.unlock();

    return m_Instance;
}

void mafThreadSynchronizer::shutdown() {

}

mafThreadSynchronizer::mafThreadSynchronizer(void) {
    m_Mutex = new QMutex;
    m_Condition = new QWaitCondition;
}

mafThreadSynchronizer::~mafThreadSynchronizer(void) {
    delete m_Mutex;
    delete m_Condition;
}

void mafThreadSynchronizer::lock(void) {
    m_Mutex->lock();
}

void mafThreadSynchronizer::unlock(void) {
    m_Mutex->unlock();
}

void mafThreadSynchronizer::wait(void) {
    m_Condition->wait(m_Mutex);
}

void mafThreadSynchronizer::wake(void) {
    m_Condition->wakeOne();
}
