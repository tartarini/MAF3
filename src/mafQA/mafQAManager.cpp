/*
 *  mafQAManager.cpp
 *  mafQA
 *
 *  Created by Paolo Quadrani on 17/08/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
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

void mafQAManager::profilerStart(const QString &comment) {
    if(m_Profiler == NULL) {
        profilerInit();
    }

    // Stops previously started profiler before starting a new one.
    if (m_Profiler->isRunning()) {
        profilerStop();
    }
    REQUIRE(m_ProfilingCounter >= 0);
    QString profileMarker(mafTr("mafQAManager run number %1").arg(++m_ProfilingCounter));
    m_Profiler->profilerStart(profileMarker, profileMarker, comment);
}


void mafQAManager::profilerStop() {
    if(m_Profiler == NULL) {
        profilerInit();
        return;
    }

    REQUIRE(m_ProfilingCounter >= 0);
    QString profileMarker(mafTr("mafQAManager run number %1").arg(m_ProfilingCounter));
    m_Profiler->profilerStop(profileMarker, profileMarker);
}

void mafQAManager::printOnConsole() {
    qDebug() << *m_ResultBuffer;
}

void mafQAManager::printOnFile(QString filename) {
    QFile f(filename);
    if(!f.open(QIODevice::WriteOnly)) {
        QByteArray ba = mafTr("Unable to open file %1").arg(filename).toAscii();
        qCritical("%s", ba.constData());
    }
    QTextStream dataStream(&f);
    dataStream << *m_ResultBuffer;

    f.close();
}

void mafQAManager::profilerViewResultsOnConsole() {
    m_ResultBuffer = m_Profiler->bufferString();
    printOnConsole();
}

void mafQAManager::profilerViewResultsOnFile(QString filename, bool open_file) {
    m_ResultBuffer = m_Profiler->bufferString();

    printOnFile(filename);

    if(open_file) {
        QStringList argument;
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

void mafQAManager::printMemoryMonitorResultOnFile(QString filename, bool open_file) {
    // Execute the monitor calculation that will update the result buffer
    monitorExecute();

    // Write the buffer in the given filename.
    printOnFile(filename);

    // If open_file flag is true, open it with the default viewer application.
    if(open_file) {
        QStringList argument;
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
    qDebug() << "level: " << level << " enabled: " << enable;
}


bool mafQAManager::pluginValidate(const QString &plugin) {
    qDebug() << "Validating Plugin: " << plugin;

    // Try to load the plugin as a dynamic library
    QLibrary *libraryHandler = new QLibrary(plugin);
    if(!libraryHandler->load()) {
        QString msg(mafTr("'%1' is not a valid MAF3 plugin. It can't be loaded.").arg(plugin));
        qDebug() << msg;
        return false;
    }
    QString msg(mafTr("'%1' successfully loaded.").arg(plugin));
    qDebug() << msg;

    mafQAFnPluginInfo *pluginInfo;
    pluginInfo = reinterpret_cast<mafQAFnPluginInfo *>(libraryHandler->resolve("pluginInfo"));
    if(!pluginInfo) {
        QString msg(mafTr("'%1' is not a valid MAF3 plugin. Plugin information not found.").arg(plugin));
        qDebug() << msg;
        return false;
    }
    msg = QString(mafTr("'%1' contains plugin info.").arg(plugin));
    qDebug() << msg;


    if (pluginInfo().m_Author.length() == 0 || pluginInfo().m_PluginName.length() == 0 \
        || pluginInfo().m_Description.length() == 0)
    {
        QString msg(mafTr("'%1' is not a valid MAF3 plugin. Plugin information not found.").arg(plugin));;
        qDebug() << msg;
        return false;
    }

    msg = QString(mafTr("'%1' is a valid MAF3 plugin:").arg(plugin));
    qDebug() << msg;

    qDebug() << "\n";
    qDebug() << "Plug-in Information:";
    qDebug() << "Varsion: " << pluginInfo().m_Version;
    qDebug() << "Author: " << pluginInfo().m_Author;
    qDebug() << "Description: " << pluginInfo().m_Description;


    return true;
}

int mafQAManager::runPythonScript(const QString &script_url, QStringList arguments, bool sync) {
    bool res = false;
    QStringList params;
    params.append(script_url);
    params.append(arguments);
    if (sync) {
        res = QProcess::execute("python", params) == 0;
    }
    else {
        res = QProcess::startDetached("python", params);
    }
    return res ? 0 : -1;
}

int mafQAManager::runScript(const QString &script_url, QStringList arguments, bool sync) {
    bool res = false;
    if (sync) {
        res = QProcess::execute(script_url, arguments) == 0;
    } else {
        res = QProcess::startDetached(script_url, arguments);
    }
    return res ? 0 : -1;
}

void mafQAManager::eventStatisticsInit() {

}


void mafQAManager::eventStatisticsStart(mafId event_id) {
    qDebug() << "Statistics start for eventID: " << event_id;
}


void mafQAManager::eventStatisticsStop(mafId event_id) {
    qDebug() << "Statistics stop for eventID: " << event_id;
}


void mafQAManager::eventStatisticsViewResults() {

}


int mafQAManager::openPollUrl(const QString &poll_url) {
    QStringList argument;
    return runScript(poll_url, argument);
}


const QString mafQAManager::applicationVersion() {
    return "1.0";
}

