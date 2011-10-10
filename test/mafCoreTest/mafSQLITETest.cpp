/*
 *  mafSQLITETest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 07/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafSQLITE.h>
#include <QSqlDatabase>
#include <QSqlQuery>

using namespace mafCore;

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
    void initTestCase() {
        initializeDB();
        m_SQLITE = NULL;
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_SQLITE);
    }

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

void mafSQLITETest::initializeDB() {
    m_DBName = QDir::tempPath();
    m_DBName.append("/mafSQLITETest.db");
    if (QFile::exists(m_DBName)) {
        QFile::remove(m_DBName);
    }
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(m_DBName);
    if (!db.open()) {
        qCritical() << mafTr("CRITICAL!!! Cannot open database");
        return;
    }
    QSqlQuery query(db);
    query.exec("create table maf3Team (id int primary key, firstname varchar(20), lastname varchar(20))");
    db.close();
}

void mafSQLITETest::mafSQLITEConstructorTest() {
    m_SQLITE = new mafSQLITE(m_DBName, "maf3Team", mafCodeLocation);
    QVERIFY(m_SQLITE->isConnected());
}

void mafSQLITETest::fillTableTest() {
    bool result(false);

    QHash<QString, QVariant> hashRecord;
    hashRecord.insert("firstname", QVariant("Paolo"));
    hashRecord.insert("lastname", QVariant("Quadrani"));
    result = m_SQLITE->insertRecord(&hashRecord);
    QVERIFY(result);

    hashRecord.clear();
    hashRecord.insert("firstname", QVariant("Daniele"));
    hashRecord.insert("lastname", QVariant("Giunchi"));
    result = m_SQLITE->insertRecord(&hashRecord);
    QVERIFY(result);

    hashRecord.clear();
    hashRecord.insert("firstname", QVariant("Roberto"));
    hashRecord.insert("lastname", QVariant("Mucci"));
    result = m_SQLITE->insertRecord(&hashRecord);
    QVERIFY(result);

    // Dump the table on console
    QString queryString("SELECT * FROM maf3Team");
    m_SQLITE->setQuery(queryString);

    int r = 0;
    QSqlRecord record = m_SQLITE->record(r);
    qDebug() << QString("*** maf3Team table dump ***");
    while(!record.isEmpty()) {
        qDebug() << QString("First Name: %1 - Last Name: %2").arg(record.value("firstname").toString(), record.value("lastname").toString());
        ++r;
        record = m_SQLITE->record(r);
    }
}

MAF_REGISTER_TEST(mafSQLITETest);
#include "mafSQLITETest.moc"
