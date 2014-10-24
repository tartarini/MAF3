/*
 *  mafTestRegistry.cpp
 *  mafTestSuiteEngine
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafTestRegistry.h"
#include <typeinfo>

using namespace mafQA;

mafTestRegistry* mafTestRegistry::instance() {
    static mafTestRegistry registry;
	return &registry;
}

void mafTestRegistry::registerTest(QObject* test) {
    m_TestSuite += test;
}

bool mafTestRegistry::isRegistered(QObject* obj) {
	Q_FOREACH(QObject* test, m_TestSuite) {
		QString f(typeid(*test).name());
	    if(f.compare(typeid(*obj).name()) == 0) {
			return true;
		}
	}
	return false;
}

int mafTestRegistry::runTests(int argc, char* argv[]) {
	int result = 0;
    Q_FOREACH(QObject* test, m_TestSuite) {
		result |= QTest::qExec(test, argc, argv);
	}
	return result;
}
