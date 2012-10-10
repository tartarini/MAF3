/*
 *  mafQAChecker.h
 *  qaCheck
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFQACHECKER_H
#define MAFQACHECKER_H

#include <mafQAManager.h>

/**
Class name: mafQAChecker
This class gives some API to the qaCheck application that allows to execute a given script,
profile the executed script and validate the MAF3 plugin.
*/
class mafQAChecker {
public:
    /// Object constructor
    mafQAChecker();

    /// Object destructor.
    ~mafQAChecker();

    /// execute the given script or executable
    void execute(QString script, QStringList argList);

    /// profile the execution of the given script or executable
    void executeAndProfile(QString script, QStringList argList);

    /// Validate the given plugin.
    void validatePlugin(QString plugin);

private:
    mafQA::mafQAManager *m_QaManager; ///< QA Manager.
};

#endif // MAFQACHECKER_H
