/*
 *  mafProfiler.h
 *  mafQA
 *
 *  Created by Paolo Quadrani on 21/08/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFPROFILER_
#define MAFPROFILER_

#include "mafQA_global.h"
#include <vector>

namespace mafQA {

/**
 Class name: mafProfiler
 This class allows calculating timing performances of blocks or code between two
 Start and Stop consecutive calls.
 */
class MAFQASHARED_EXPORT mafProfiler {
public:
    /// Return an instance of the profiler.
    static mafProfiler *instance();
    
    /// Register a new entry point for the profiler. ProfilerStop will close this entry.
    void profilerStart(const mafString &funcName, const mafString &funcLocation, const mafString &notes, bool profileMultipleTimes = false, int numTotalProfileTimesBeforeLog = 1000);
    
    /// Close the current entry of the profiler.
    void profilerStop(const mafString &funcName, const mafString &funcLocation);
    
    /// Return the status of the Profiler.*/
    bool isRunning();

    /// Destroy the singleton instance. To be called at the end of the application.
    void shutdown();

    /// Return the profile results.
    mafString *bufferString();

protected:
    /// Logs the results of the profiling test.
    void logResults(int nID);
    
private:
    /// Constructor.
    mafProfiler();
    /// Destructor.
    ~mafProfiler();
    /// Copy constructor.
    mafProfiler(const mafProfiler &ref);
    /// Copy operator redefinition.
    mafProfiler &operator=(const mafProfiler &ref);
    
    /// Structure representing the timing information associated to a monitored function or piece of code.
    struct mafProfilerInformation {
        mafString FunctionName;
        mafString FunctionLocation;
        mafString Notes;
        unsigned int NumTimesCalled;
        unsigned int NumTotalProfileTimes;
        mafTime *StartTime;
        bool MultipleCalls;
        std::vector<int> ElapsedTime;
        int TotalElapsedTime;
    };
    
    static mafProfiler *m_Instance; ///< Unique instance of the Profiler.
    bool m_Running; ///< Flag to indicate that the profiler is running or no.
    std::vector<mafProfilerInformation> m_ProfiledFunctions; ///< Vector of all timing of profiled functions.
    
    mafString m_BufferString; ///< Buffer containing the Profiler's results.
    mafTextStream m_LogStream; ///< Stream that contain the Profiler results.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafString *mafProfiler::bufferString() {
    return &m_BufferString;
}

} // namespace mafQA

#endif  // MAFPROFILER_
