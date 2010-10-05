/*
 *  mafQAManager.cpp
 *  mafQA
 *
 *  Created by Paolo Quadrani on 17/08/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafQAManager.h"
#include "mafProfiler.h"
#include <mafObjectFactory.h>

using namespace mafQA;
using namespace mafCore;

#ifdef WIN32
    #define OPEN_CMD "start"
#else
    #ifdef __APPLE__
        #define OPEN_CMD "open"
    #else
        // Linux open command ?!?
        #define OPEN_CMD ""
    #endif
#endif

mafQAManager* mafQAManager::instance() {
    static mafQAManager instanceQAManager;
    return &instanceQAManager;
}

void mafQAManager::shutdown() {
}

mafQAManager::mafQAManager() : m_Profiler(NULL), m_ProfilingCounter(0), m_EnableMonitorRAM(false), m_EnableMonitorHD(false), m_ResultBuffer(NULL), m_MonitorRAM(NULL), m_MonitorHDD(NULL) {
}

mafQAManager::~mafQAManager() {
    // shutdown the profiler eventually started.
    profilerShutdown();
    mafDEL(m_MonitorRAM);
    mafDEL(m_MonitorHDD);
}

void mafQAManager::profilerInit() {
    // Create the instance for the profiler
    m_Profiler = mafProfiler::instance();
    // and reset the profiling counter.
    m_ProfilingCounter = 0;
    // stops previously started profiler.
    if (m_Profiler->isRunning()) {
        profilerStop();
    }
}

void mafQAManager::profilerStart(const mafString &comment) {
    if(m_Profiler == NULL) {
        profilerInit();
    }

    // Stops previously started profiler before starting a new one.
    if (m_Profiler->isRunning()) {
        profilerStop();
    }
    REQUIRE(m_ProfilingCounter >= 0);
    mafString profileMarker(mafTr("mafQAManager run number %1").arg(++m_ProfilingCounter));
    m_Profiler->profilerStart(profileMarker, profileMarker, comment);
}


void mafQAManager::profilerStop() {
    if(m_Profiler == NULL) {
        profilerInit();
        return;
    }

    REQUIRE(m_ProfilingCounter >= 0);
    mafString profileMarker(mafTr("mafQAManager run number %1").arg(m_ProfilingCounter));
    m_Profiler->profilerStop(profileMarker, profileMarker);
}

void mafQAManager::printOnConsole() {
    mafMsgDebug() << *m_ResultBuffer;
}

void mafQAManager::printOnFile(mafString filename) {
    mafFile f(filename);
    if(!f.open(QIODevice::WriteOnly)) {
        mafMsgCritical("%s", mafTr("Unable to open file %1").arg(filename).toAscii().data());
    }
    mafTextStream dataStream(&f);
    dataStream << *m_ResultBuffer;

    f.close();
}

void mafQAManager::profilerViewResultsOnConsole() {
    m_ResultBuffer = m_Profiler->bufferString();
    printOnConsole();
}

void mafQAManager::profilerViewResultsOnFile(mafString filename, bool open_file) {
    m_ResultBuffer = m_Profiler->bufferString();

    printOnFile(filename);

    if(open_file) {
        mafStringList argument;
        runScript(filename, argument);
    }
}

void mafQAManager::profilerShutdown() {
    if(m_Profiler != NULL) {
        m_Profiler->shutdown();
    }
}

void mafQAManager::enableRAMMonitor(bool enable) {
    m_EnableMonitorRAM = enable;
}

void mafQAManager::enableHardDiskMonitor(bool enable) {
    m_EnableMonitorHD = enable;
}

void mafQAManager::printMemoryMonitorResultOnConsole() {
    // Execute the monitor calculation that will update the result buffer
    monitorExecute();

    // Dump the result buffer on the standard console.
    printOnConsole();
}

void mafQAManager::printMemoryMonitorResultOnFile(mafString filename, bool open_file) {
    // Execute the monitor calculation that will update the result buffer
    monitorExecute();

    // Write the buffer in the given filename.
    printOnFile(filename);

    // If open_file flag is true, open it with the default viewer application.
    if(open_file) {
        mafStringList argument;
        runScript(filename, argument);
    }
}

void mafQAManager::monitorExecute() {
    if (m_EnableMonitorRAM) {
        mafDEL(m_MonitorRAM);
        m_MonitorRAM = mafNEW(mafCore::mafMonitorRAM);
        m_MonitorRAM->update();
        m_ResultBuffer = m_MonitorRAM->resultBuffer();
    }

    if (m_EnableMonitorHD) {
        mafDEL(m_MonitorHDD);
        m_MonitorHDD = mafNEW(mafCore::mafMonitorHardDisk);
        m_MonitorHDD->update();
        m_ResultBuffer = m_MonitorHDD->resultBuffer();
    } else {
        return;
    }
}

void mafQAManager::enableErrorMonitor(int level, bool enable) {
    mafMsgDebug() << "level: " << level << " enabled: " << enable;
}


bool mafQAManager::pluginValidate(const mafString &plugin) {
    mafMsgDebug() << "Validating Plugin: " << plugin;

    // Try to load the plugin as a dynamic library
    mafLibrary *libraryHandler = new mafLibrary(plugin);
    if(!libraryHandler->load()) {
        mafString msg(mafTr("'%1' is not a valid MAF3 plugin. It can't be loaded.").arg(plugin));
        mafMsgDebug() << msg;
        return false;
    }
    mafString msg(mafTr("'%1' successfully loaded.").arg(plugin));
    mafMsgDebug() << msg;

    mafQAFnPluginInfo *pluginInfo;
    pluginInfo = reinterpret_cast<mafQAFnPluginInfo *>(libraryHandler->resolve("pluginInfo"));
    if(!pluginInfo) {
        mafString msg(mafTr("'%1' is not a valid MAF3 plugin. Plugin information not found.").arg(plugin));
        mafMsgDebug() << msg;
        return false;
    }
    msg = mafString(mafTr("'%1' contains plugin info.").arg(plugin));
    mafMsgDebug() << msg;


    if (pluginInfo().m_Author.length() == 0 || pluginInfo().m_PluginName.length() == 0 \
        || pluginInfo().m_Description.length() == 0)
    {
        mafString msg(mafTr("'%1' is not a valid MAF3 plugin. Plugin information not found.").arg(plugin));;
        mafMsgDebug() << msg;
        return false;
    }

    msg = mafString(mafTr("'%1' is a valid MAF3 plugin:").arg(plugin));
    mafMsgDebug() << msg;

    mafMsgDebug() << "\n";
    mafMsgDebug() << "Plug-in Information:";
    mafMsgDebug() << "Varsion: " << pluginInfo().m_Version;
    mafMsgDebug() << "Author: " << pluginInfo().m_Author;
    mafMsgDebug() << "Description: " << pluginInfo().m_Description;


    return true;
}

int mafQAManager::runPythonScript(const mafString &script_url, mafStringList arguments, bool sync) {
    bool res = false;
    mafStringList params;
    params.append(script_url);
    params.append(arguments);
    if (sync) {
        res = mafProcess::execute("python", params) == 0;
    }
    else {
        res = mafProcess::startDetached("python", params);
    }
    return res ? 0 : -1;
}

int mafQAManager::runScript(const mafString &script_url, mafStringList arguments, bool sync) {
    bool res = false;
    if (sync) {
        res = mafProcess::execute(script_url, arguments) == 0;
    } else {
        res = mafProcess::startDetached(script_url, arguments);
    }
    return res ? 0 : -1;
}

void mafQAManager::eventStatisticsInit() {

}


void mafQAManager::eventStatisticsStart(mafId event_id) {
    mafMsgDebug() << "Statistics start for eventID: " << event_id;
}


void mafQAManager::eventStatisticsStop(mafId event_id) {
    mafMsgDebug() << "Statistics stop for eventID: " << event_id;
}


void mafQAManager::eventStatisticsViewResults() {

}


int mafQAManager::openPollUrl(const mafString &poll_url) {
    mafStringList argument;
    return runScript(poll_url, argument);
}


const mafString mafQAManager::applicationVersion() {
    return "1.0";
}

