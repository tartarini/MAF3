/*
 *  mafSQLITETest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 07/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafSQLITE.h>
#include <QSqlDatabase>
#include <QSqlQuery>

/**
 Class name: mafSQLITETest
 This class implements the test suite for mafSQLITE.
 */

//! <title>
// mafSQLITE
//! </title>
//! <description>
// mafSQLITE defines the MAF3 SQLITE class that will interface SQLITE DB calls to manage the given table on a given DB.
//! </description>

class mafSQLITETest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// create new object and check that is not NULL test case.
    void mafSQLITEConstructorTest();

    /// log a string into the log file.
    void fillTableTest();

private:
    /// Create a test DB with a table inside.
    void initializeDB();

    mafSQLITE *m_SQLITE; ///< Test variable
    QString m_DBName; ///< Test variable
};
