/*
 *  mafQAManager.h
 *  mafQA
 *
 *  Created by Paolo Quadrani on 17/08/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFQA_H
#define MAFQA_H

// Includes list
#include "mafQA_global.h"
#include <mafMonitorRAM.h>
#include <mafMonitorHardDisk.h>

namespace mafQA {

// Class forwarding list
class mafProfiler;

/**
 Class name: mafQAManager
 This class represent the Facade class of the MAF3 QA Module.
 */
class MAFQASHARED_EXPORT mafQAManager {
public:
    /// Return an instance of the manager
    static mafQAManager *instance();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// Allow initialisation of mafProfiler singleton.
    void profilerInit();

    /// Allows to start the mafProfiler timer.
    void profilerStart(const QString &comment = "");

    /// Allows to stop the mafProfiler timer.
    void profilerStop();

    /// Show the results of all profiles done until now by opening them into the default browser.
    void profilerViewResultsOnConsole();

    /// Save the results of the profiler in the given filename and if the 'open_file' flag is true, open the file with its default viewer
    void profilerViewResultsOnFile(QString filename, bool open_file = false);

    /// Allows to shutdown the mafProfiler and free the allocated memory.
    void profilerShutdown();

    /// Allow enabling/disabling the monitor of main memory usage by becoming observer for Alloc() method calls.
    void enableRAMMonitor(bool enable = true);

    /// Allow enabling/disabling the monitor of storage usage.
    void enableHardDiskMonitor(bool enable = true);

    /// Print the results of memory monitor on console.
    void printMemoryMonitorResultOnConsole();

    /// Print the results of memory monitor on file.
    void printMemoryMonitorResultOnFile(QString filename, bool open_file = false);

    /// Enable/disable memory monitor.
    /** Allow enabling/disabling the monitor of warning/error/exceptions messages.
     The selection of what has to be monitored is selected with the parameter ‘level’ which can be: WARNINGS, ERROR, EXCEPTIONS, ALL.*/
    void enableErrorMonitor(int level, bool enable = true);

    /// Allow checking validity of the plug-in that should be loaded and return a boolean value indicating the check result.
    bool pluginValidate(const QString &plugin);

    /// Allow running the corresponding external python script. Pyhton has to be on the system PATH.
    int runPythonScript(const QString &script_url, QStringList arguments, bool sync = false);

    /// Allow running the corresponding external script.
    int runScript(const QString &script_url, QStringList arguments, bool sync = false);

    /// Allow initialising the event monitor that will generate statistics on events.
    void eventStatisticsInit();

    /// Allow starting the event statistic monitor on the given event_id. If no event_id is given, the monitoring will be on all the events that will reach the event bus.
    void eventStatisticsStart(mafCore::mafId event_id = 0);

    /// Allow stopping the event statistic monitor on the given event_id. If no event_id is given, the monitoring will be stopped for all the events that will reach the event bus.
    void eventStatisticsStop(mafCore::mafId event_id = 0);

    /// Show the results of all the statistics done until now opening them into the default browser.
    void eventStatisticsViewResults();

    /// Open through the default internet browser the URL for the specific application and allows the user compile the poll. Return 0 on success, -1 otherwise.
    int openPollUrl(const QString &poll_url);

    /// Allow retrieving the application version, source code building number and other related build information.
    const QString applicationVersion();

private:

    struct mafQAPluginInfo {
    float m_Version;            ///< Plug-in's Version
    QString m_PluginName;     ///< Plug-in's name
    QString m_Author;         ///< Plug-in's author
    //QString m_Vendor;         ///< Plug-in's company name, not used yet
    //QString m_VendorHomepage; ///< Plug-in's company hompage, not used yet
    QString m_Description;    ///< Plug-in's general description.
    };

     /// Signature for the version query function
    typedef mafQAPluginInfo mafQAFnPluginInfo();

    /// class constructor.
    mafQAManager();

    /// Object destructor.
    ~mafQAManager();

    /// Print the result buffer on console
    void printOnConsole();

    /// Print the result buffer on file
    void printOnFile(QString filename);

    /// Execute mafMonitor code to retrieve information.
    void monitorExecute();

    mafProfiler *m_Profiler; ///< Profiler instance.
    int m_ProfilingCounter; ///< Counter needed for multiple start-stop sequences.
    bool m_EnableMonitorRAM; ///< Flag for RAM monitoring.
    bool m_EnableMonitorHD; ///< Flag for HD monitoring.
    QString *m_ResultBuffer; ///< String buffer containing results coming from mafProfiler or monitors.
    mafCore::mafMonitorRAM *m_MonitorRAM; ///< Monitor base class, needed to monitor RAM usage.
    mafCore::mafMonitorHardDisk *m_MonitorHDD; ///< Monitor base class, needed to monitor HD memory usage.
};

}

#endif // MAFQA_H
