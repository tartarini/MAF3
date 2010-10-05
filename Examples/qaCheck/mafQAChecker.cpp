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

void mafQAChecker::execute(mafString script, mafStringList argList) {
    m_QaManager->runPythonScript(script, argList);
}

void mafQAChecker::executeAndProfile(mafString script, mafStringList argList) {
    m_QaManager->profilerInit();
    m_QaManager->profilerStart();
    m_QaManager->runScript(script, argList, true);
    m_QaManager->profilerStop();
    m_QaManager->profilerViewResultsOnConsole();
}

void mafQAChecker::validatePlugin(mafString plugin) {
    m_QaManager->pluginValidate(plugin);
}
