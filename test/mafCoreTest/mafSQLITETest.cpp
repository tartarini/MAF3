/*
 *  mafSQLITETest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 08/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

void mafSQLITETest::initTestCase() {
    initializeDB();
    m_SQLITE = NULL;
}

void mafSQLITETest::cleanupTestCase() {
    mafDEL(m_SQLITE);
}

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
    
    m_SQLITE->submitAllChanges();

    // Dump the table on console
    QString queryString("SELECT * FROM maf3Team");
    QSqlQuery *q = m_SQLITE->executeQuery(queryString);

    QVERIFY(q);
    
    qDebug() << QString("*** maf3Team table dump ***");
    while(q->next()) {
        QSqlRecord record = q->record();
        qDebug() << QString("First Name: %1 - Last Name: %2").arg(record.value("firstname").toString(), record.value("lastname").toString());
    }
    
}

#include "mafSQLITETest.moc"
