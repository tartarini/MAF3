/*
 *  mafLoggerSQLITE.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 07/10/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGGERSQLITE_H
#define MAFLOGGERSQLITE_H

// Includes list
#include "mafLogger.h"
#include <QTemporaryFile>

namespace mafCore {

// Class forwarding list

/**
 Class name: mafLoggerSQLITE
 This class defines the MAF3 logging class that will store messages into a SQLITE DB.
 @sa maf Logger mafLoggerConsole mafLoggerBuffer mafLoggerFile mafMessageHandler
 */
class MAFCORESHARED_EXPORT mafLoggerSQLITE : public mafLogger {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafLogger);

public:
    /// Object constructor.
    mafLoggerSQLITE(const QString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Clear all the logged messages until now.
    /** This methods close the connection with the last DB file and remove it, then open a new log DB file.
    This will update also the m_LastLogFile member variable.*/
    /*virtual*/ void clearLogHistory();

    /// Return the filename associated to the last session of logs.
    const QString lastLogFile() const;

protected:
    /// Object destructor.
    /* virtual */~mafLoggerSQLITE();

    /// Method used to log the given message to the DB.
    /*virtual*/ void loggedMessage(const QtMsgType type, const QString &msg);

    /// Clear history logs from the last temporary DB file.
    /** This method is used to close the connection with the last opened temporary DB file.
    It is invoked by the destructor and cleanLogHistory methods.*/
    void closeLastTempFile();

    /// Create a new temporary file reference.
    /** This method is called by the constructor and by the clearLogHistory methods
    to initialize a new temporary file.*/
    void initializeNewTemporaryFile();

private:
    QTemporaryFile *m_TempFileLog; ///< Temporary File containing all the logged messages for a specific session.
    QString m_LastLogFile; ///< Filename of last logged file. Useful to retrieve information of file log when application cresh.
};

inline const QString mafLoggerSQLITE::lastLogFile() const {
	return m_LastLogFile;
}

} // namespace mafCore

#endif // MAFLOGGERSQLITE_H
