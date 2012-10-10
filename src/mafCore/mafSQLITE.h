/*
 *  mafSQLITE.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/10/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
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
class QSqlQuery;

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

    /// Allows to execute the query on the BD.
    /** Insert and Updates does NOT imply the submit of the changes to the Database, for submitting the changes
     it needs to call submitAllChanges() */
    QSqlQuery *executeQuery(const QString &queryString);

    /// Return the record associated to the given index. If the index is out of bound NULL is returned.
    QSqlRecord record(int idx);

public Q_SLOTS:
    /// Slot that will insert into the DB table the elements contained in the hash.
    /** Insert does NOT imply the submit of the changes to the Database, for submitting the changes
     it needs to call submitAllChanges() */
    bool insertRecord(QHash<QString, QVariant> *recordHash);

    /// Slot that will insert into the DB table the record.
    /** Insert does NOT imply the submit of the changes to the Database, for submitting the changes
     it needs to call submitAllChanges() */
    bool insertRecord(const QSqlRecord &record);
    
    /// Submit all the changes to the DB.
    void submitAllChanges();

    /// Allows to compress the DB and store temporary tables into memory.
    void optimizeDB();

protected:
    /// Object destructor.
    /*virtual*/ ~mafSQLITE();
    
    /// Method used to log the given message to the DB.
    /*virtual*/ void loggedMessage(const QtMsgType type, const QString &msg);

    
    /// Clear history logs from the last temporary DB file.
    /** This method is used to close the connection with the last opened temporary DB file.
     It is invoked by the destructor and cleanLogHistory methods.*/
    void closeLastTempFile();

private:
    QSqlTableModel *m_TableModel; ///< Model associated to the DB table.
    bool m_DBConnected; ///< Variable containing the connection status with the database.
    QSqlQuery *m_Query; ///< Represents the current executed query.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline bool mafSQLITE::isConnected() {
    return m_DBConnected;
}

} //namespace mafCore

#endif // MAFSQLITE_H
