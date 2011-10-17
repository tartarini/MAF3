/*
 *  mafSQLITE.cpp
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSQLITE.h"

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlError>

using namespace mafCore;

mafSQLITE::mafSQLITE(const QString code_location) : mafObjectBase(code_location), m_TableModel(NULL), m_DBConnected(false), m_Query(NULL) {
}

mafSQLITE::mafSQLITE(const QString dbName, const QString tableName, const QString code_location) : mafObjectBase(code_location), m_TableModel(NULL), m_DBConnected(false), m_Query(NULL) {
    
    QStringList connectionNames = QSqlDatabase::connectionNames();
    QSqlDatabase db;
    QString driverName("QSQLITE");
    
    if(!connectionNames.contains(dbName)) {
        db= QSqlDatabase::addDatabase(driverName, dbName);
        db.setDatabaseName(dbName);
    } else {
        db = QSqlDatabase::database(dbName);
    }
    
    if (!db.open()) {
        qCritical() << mafTr("Unable to establish a database connection with db ") << dbName;
        return;
    }

    m_DBConnected = true;

    m_TableModel = new QSqlTableModel(this, db);
    m_TableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    if (!tableName.isEmpty()) {
        setTableName(tableName);
    }
}

mafSQLITE::~mafSQLITE() {
    // submit all changes before dye.
    if (m_TableModel) {
        submitAllChanges();
        m_TableModel->database().close();
    }
    
    if(m_Query) {
        delete m_Query;
        m_Query = NULL;
    }
}

void mafSQLITE::setTableName(const QString name) {
    REQUIRE(m_TableModel != NULL);

    m_TableModel->setTable(name);
    m_TableModel->select();
}

QSqlQuery *mafSQLITE::executeQuery(const QString &queryString) {
    QSqlDatabase db = m_TableModel->database();
    if(m_Query == NULL) {
        m_Query = new QSqlQuery(db);
    }
    //qDebug() << queryString << " " << db.tables();
    bool res(m_Query->exec(queryString));
    //qDebug() << m_Query->lastError().text();
    return res ? m_Query : NULL;
}

QSqlRecord mafSQLITE::record(int idx) {
    QSqlRecord r;
    if (idx >= m_TableModel->rowCount()) {
        return r;
    }
    r = m_TableModel->record(idx);
    return r;
}

bool mafSQLITE::insertRecord(QHash<QString, QVariant> *recordHash) {
    REQUIRE(m_TableModel != NULL);

    // Get a new empty record.
    QSqlRecord record = m_TableModel->record();

    // Fill the record with the information contained into the hash <field name, field value>
    QHash<QString, QVariant>::const_iterator iter = recordHash->constBegin();
    for (; iter != recordHash->constEnd(); ++iter) {
        record.setValue(iter.key(), iter.value());
    }
    
    return this->insertRecord(record);
}

bool mafSQLITE::insertRecord(const QSqlRecord &record) {
    // Insert the record into the table
    bool result = m_TableModel->insertRecord(-1, record);
    
    // Set the object as modified and emit the related signal to notify the observer.
    if (result) {
        setModified();
        Q_EMIT modifiedObject();
    }
    return result;
}

void mafSQLITE::submitAllChanges() {
    REQUIRE(m_TableModel != NULL);

    if (modified()) {
        m_TableModel->submitAll();
        setModified(false);
    }
}

void mafSQLITE::optimizeDB() {
    executeQuery("VACUUM");
    executeQuery("PRAGMA temp_store = MEMORY;");
}
