/*
 *  mafSQLITE.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSQLITE_H
#define MAFSQLITE_H

// Includes list
#include "mafObjectBase.h"
#include <QSqlRecord>

// Class forwarding list
class QSqlTableModel;

namespace mafCore {

// Name spaced Class forwarding list

/**
Class name: mafSQLITE
This class wrap the SQLITE db functionalities to write data into a table of the SQLITE given DB.
*/
class MAFCORESHARED_EXPORT mafSQLITE : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafSQLITE(const QString code_location = "");

    /// Object constructor.
    mafSQLITE(const QString dbName, const QString tableName, const QString code_location = "");

    /// Assign the table's name to manage.
    void setTableName(const QString name);

    /// return true if there is any connected DB.
    bool isConnected();

    /// Allows to execute the query on the table model connected to the BD.
    void setQueryTable(const QString &queryString);

    /// Allows to execute the query on the BD.
    void setQueryDb(const QString &queryString);

    /// Return the record associated to the given index. If the index is out of bound NULL is returned.
    QSqlRecord record(int idx);

public Q_SLOTS:
    /// Slot that will insert into the DB table the elements contained in the hash.
    bool insertRecord(QHash<QString, QVariant> *recordHash);

    /// Submit all the changes to the DB.
    void submitAllChanges();

    /// Allows to compress the DB and store temporary tables into memory.
    void optimizeDB();

protected:
    /// Object destructor.
    /*virtual*/ ~mafSQLITE();

private:
    QSqlTableModel *m_TableModel; ///< Model associated to the DB table.
    bool m_DBConnected; ///< Variable containing the connection status with the database.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline bool mafSQLITE::isConnected() {
    return m_DBConnected;
}

} //namespace mafCore

#endif // MAFSQLITE_H
