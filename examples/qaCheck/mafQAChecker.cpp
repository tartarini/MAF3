/*
 *  mafQAChecker.cpp
 *  qaCheck
 *
 *  Created by Roberto Mucci on 20/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafQAChecker.h"

using namespace mafQA;

mafQAChecker::mafQAChecker() {
    m_QaManager = mafQAManager::instance();
}

mafQAChecker::~mafQAChecker() {
    m_QaManager->shutdown();
}

void mafQAChecker::execute(QString script, QStringList argList) {
    m_QaManager->runPythonScript(script, argList);
}

void mafQAChecker::executeAndProfile(QString script, QStringList argList) {
    m_QaManager->profilerInit();
    m_QaManager->profilerStart();
    m_QaManager->runScript(script, argList, true);
    m_QaManager->profilerStop();
    m_QaManager->profilerViewResultsOnConsole();
}

void mafQAChecker::validatePlugin(QString plugin) {
    m_QaManager->pluginValidate(plugin);
}
