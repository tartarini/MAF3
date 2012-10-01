/*
 *  mafProfiler.cpp
 *  mafQA
 *
 *  Created by Paolo Quadrani on 21/08/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafProfiler.h"

using namespace mafQA;
using namespace mafCore;

mafProfiler *mafProfiler::m_Instance = 0;

mafProfiler::mafProfiler() : m_Running(false) {
    m_LogStream.setString(&m_BufferString);
}

mafProfiler::~mafProfiler() {
}

mafProfiler* mafProfiler::instance() {
    if(!m_Instance)
        m_Instance = new mafProfiler;
    return m_Instance;
}

void mafProfiler::profilerStart(const QString &funcName, const QString &funcLocation, const QString &notes, bool profileMultipleTimes, int numTotalProfileTimesBeforeLog) {
    m_Running = true;
    int index = -1;
    for(int i = m_ProfiledFunctions.size() - 1; i >= 0; --i) {
        if(m_ProfiledFunctions[i].FunctionName == funcName && m_ProfiledFunctions[i].FunctionLocation == funcLocation) {
            index = i;
            break;
        }
    }
    if (index < 0) {
        mafProfiler::mafProfilerInformation profileInfo;
        profileInfo.FunctionName = funcName;
        profileInfo.FunctionLocation = funcLocation;
        profileInfo.Notes = notes;
        profileInfo.MultipleCalls = profileMultipleTimes;
        profileInfo.NumTotalProfileTimes = profileMultipleTimes ? numTotalProfileTimesBeforeLog : 1;
        profileInfo.NumTimesCalled = 1;
        profileInfo.StartTime = new QTime();
        profileInfo.StartTime->start();
        profileInfo.TotalElapsedTime = 0;
        m_ProfiledFunctions.push_back(profileInfo);
    } else {
        m_ProfiledFunctions[index].NumTimesCalled += 1;
        m_ProfiledFunctions[index].StartTime->start();
    }
}

void mafProfiler::profilerStop(const QString &funcName, const QString &funcLocation) {
    m_Running = false;
    int index = -1;
    for(int i = m_ProfiledFunctions.size() - 1; i >= 0; --i) {
        if(m_ProfiledFunctions[i].FunctionName == funcName && m_ProfiledFunctions[i].FunctionLocation == funcLocation) {
            index = i;
            break;
        }
    }
    ASSERT(index >= 0);
    int elapsedTime = m_ProfiledFunctions[index].StartTime->elapsed();
    m_ProfiledFunctions[index].TotalElapsedTime += elapsedTime;
    m_ProfiledFunctions[index].ElapsedTime.push_back(elapsedTime);
    if (m_ProfiledFunctions[index].MultipleCalls) {
        if (m_ProfiledFunctions[index].NumTimesCalled == m_ProfiledFunctions[index].NumTotalProfileTimes) {
            logResults(index);
        }
    } else {
        logResults(index);
    }
}

void mafProfiler::logResults( int index ) {
    m_LogStream << "Function Name: " << m_ProfiledFunctions[index].FunctionName << "\n";
    m_LogStream << "Location: " << m_ProfiledFunctions[index].FunctionLocation << "\n";
    if (m_ProfiledFunctions[index].Notes.size() > 0) {
        m_LogStream << "Notes: " << m_ProfiledFunctions[index].Notes << "\n";
    }

    if (m_ProfiledFunctions[index].MultipleCalls) {
        m_LogStream << QString("Number of times called: ") << m_ProfiledFunctions[index].NumTimesCalled << "\n";
        double meanElapsedTime = static_cast< double >( m_ProfiledFunctions[index].TotalElapsedTime ) / m_ProfiledFunctions[index].NumTotalProfileTimes;
        m_LogStream << QString("Avg. mSecs: ") << QString::number(meanElapsedTime) << "\n";
        m_LogStream << QString("Total mSecs: ") << QString::number(m_ProfiledFunctions[index].TotalElapsedTime) << "\n";
    } else {
        double elapsed_time;
        m_LogStream << "Call Number: " << m_ProfiledFunctions[index].NumTimesCalled << "\n";
        elapsed_time = static_cast< double >(m_ProfiledFunctions[index].ElapsedTime[m_ProfiledFunctions[index].NumTimesCalled - 1]);
        m_LogStream << "mSecs: " << QString::number(elapsed_time) << "\n";
    }

    m_LogStream << "\n";
}

bool mafProfiler::isRunning() {
    return m_Running;
}

void mafProfiler::shutdown() {
    delete m_Instance;
    m_Instance = NULL;
}
