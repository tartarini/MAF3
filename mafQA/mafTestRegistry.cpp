/*
 *  mafTestRegistry.cpp
 *  mafTestSuiteEngine
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestRegistry.h"

using namespace mafQA;

mafTestRegistry* mafTestRegistry::instance() {
    static mafTestRegistry registry;
	return &registry;
}

void mafTestRegistry::registerTest(QObject* test) {
    m_TestSuite += test;
}

int mafTestRegistry::runTests(int argc, char* argv[]) {
	int result = 0;
    foreach(QObject* test, m_TestSuite) {
		result |= QTest::qExec(test, argc, argv);
	}
	return result;
}
