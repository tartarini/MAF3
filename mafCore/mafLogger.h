/*
 *  mafLogger.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGGER_H
#define MAFLOGGER_H

// Includes list
#include "mafObjectBase.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafLogger
 This class defines the MAF3 base object for logging.
 It allows you configuring the logging mode with which filter logged messages as needed and enable/disable logging.
 You cannot use directly this class for logging but a subclass of it that redefine the virtual method logMessage.
 @sa mafLoggerConsole mafLoggerFile mafMessageHandler
 */
class MAFCORESHARED_EXPORT mafLogger : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafLogger(const mafString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Set the mode for logging: mafLogModeAllMessages is the default.
    /** By changing the log mode you can filter messages to be logged to output stream.
    You can exclude some log types (Warnings, ...) to filter messages. By setting mafLogModeCustom, you can define your own filer in your subclass.*/
    void setLogMode(const mafLogMode mode);

    /// Return the current logging modality.
    mafLogMode logMode() const {return m_LogMode;}

    /// Enable or disable logging.
    void setEnabled(bool enabled = true) {m_Enabled = enabled;}

    /// Return the status of logging; enabled or no.
    bool enabled() const {return m_Enabled;}

    /// Method used to log the given message into the specific ouput stream.
    void logMessage(const mafMsgType type, const mafString &msg);

    /// Clear all the logged messages until now.
    virtual void clearLogHistory() = 0;

protected:
    /// Virtual method implementation of logMessage.
    /** You have to redefine this method in your sublass and put here your strategy to store the log message into the specific output stream media (file, db, buffer, console, ...).*/
    virtual void loggedMessage(const mafMsgType type, const mafString &msg) = 0;

private:
    mafLogMode m_LogMode; ///< Active logging mode type, see mafLogMode typedef.
    bool m_Enabled; ///< Flag to indicate if logging is enabled or no.
};

}

#endif // MAFLOGGER_H
