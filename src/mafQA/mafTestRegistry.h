/*
 *  mafTestRegistry.h
 *  mafTestSuiteEngine
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef TESTREGISTRY_
#define TESTREGISTRY_

// Includes list
#include <QtTest/QtTest>
#include "mafQA_global.h"

namespace mafQA {

// Class forwarding list

/**
 * A registry of QtTest test classes.
 * All test classes registered with MAF_REGISTER_TEST add
 * themselves to this registry. All registered tests can then be run at
 * once using runTests().
 */
class MAFQASHARED_EXPORT mafTestRegistry {
public:
    ///!brief Retrieve the single instance of the registry.
    static mafTestRegistry* instance();

    ///!brief Register a QtTest test.
    /** This method is called  by MAF_REGISTER_TEST, and you should
      not use this method directly.
     */
    void registerTest(QObject*);

    ///!brief Run all registered tests using QTest::qExec()
    int runTests(int argc, char* argv[]);

private:
    ///!brief Private constructor for the singletone.
    mafTestRegistry() {}

		private:
    QList<QObject*> m_TestSuite; ///< Test suite list
};

}

#endif // TESTREGISTRY_
