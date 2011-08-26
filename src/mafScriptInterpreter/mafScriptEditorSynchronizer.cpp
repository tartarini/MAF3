/*
 *  mafScriptEditorSynchronizer.cpp
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi on 08/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */


#include "mafScriptEditorSynchronizer.h"

using namespace mafScriptInterpreter;

mafScriptEditorSynchronizer *mafScriptEditorSynchronizer::instance(void)
{
    static QMutex mutex;

    mutex.lock();
    if(!m_instance)
	m_instance = new mafScriptEditorSynchronizer;
    mutex.unlock();

    return m_instance;
}

void mafScriptEditorSynchronizer::lock(void)
{
    m_Mutex->lock();
}

void mafScriptEditorSynchronizer::unlock(void)
{
    m_Mutex->unlock();
}

void mafScriptEditorSynchronizer::wait(void)
{
    m_Condition->wait(m_Mutex);
}

void mafScriptEditorSynchronizer::wake(void)
{
    m_Condition->wakeOne();
}

mafScriptEditorSynchronizer::mafScriptEditorSynchronizer(void)
{
    m_Mutex = new QMutex;
    m_Condition = new QWaitCondition;
}

mafScriptEditorSynchronizer::~mafScriptEditorSynchronizer(void)
{
    delete m_Mutex;
    delete m_Condition;
}

mafScriptEditorSynchronizer *mafScriptEditorSynchronizer::m_instance = NULL;
