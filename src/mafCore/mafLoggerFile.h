/*
 *  mafLoggerFile.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGGERFILE_H
#define MAFLOGGERFILE_H

// Includes list
#include "mafLogger.h"
#include <QTemporaryFile>

namespace mafCore {

// Class forwarding list

/**
 Class name: mafLoggerFile
 This class defines the MAF3 logging class that will store messages into a file on file system.
 @sa maf Logger mafLoggerConsole mafLoggerBuffer mafMessageHandler
 */
class MAFCORESHARED_EXPORT mafLoggerFile : public mafLogger {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafLogger);

public:
    /// Object constructor.
    mafLoggerFile(const QString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Clear all the logged messages until now.
    /** This methods close the connection with the last temporary file and remove it, then open a new log file.
    This will update also the m_LastLogFile member variable.*/
    /*virtual*/ void clearLogHistory();

    /// Return the filename associated to the last session of logs.
    const QString lastLogFile() const;

protected:
    /// Object destructor.
    /* virtual */~mafLoggerFile();

    /// Method used to log the given message to the file system.
    /*virtual*/ void loggedMessage(const QtMsgType type, const QString &msg);

    /// Clear history logs from the last temporary file.
    /** This method is used to close the connection with the last opened temporary file.
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

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline const QString mafLoggerFile::lastLogFile() const {
    return m_LastLogFile;
}

} // namespace mafCore

#endif // MAFLOGGERFILE_H
