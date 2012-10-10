/*
 *  mafMessageHandler.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMESSAGEHANDLER_H
#define MAFMESSAGEHANDLER_H

// Includes list
#include "mafCore_global.h"

namespace mafCore {

// Class forwarding list
class mafLogger;

/**
 Class name: mafMessageHandler
 This singleton provides the dispatching of logging messages.
 The message if forwarded to the current active logger only if it is enabled and the message type has not been filtered.
 For example: if the logger is in the mode of enable logging only for Critical messages, a message is forwarded to it only if
 it is critical and so on.
 @sa mafLogger, mafLoggerConsole, mafLoggerBuffer, mafLoggerFile
 */
class MAFCORESHARED_EXPORT mafMessageHandler {
public:
    /// Return an instance of the Message Handler.
    static mafMessageHandler *instance();

    /// Destroy the singleton instance. To be called at the end of the application lifetime.
    void shutdown();

    /// Allows to reinstall the message handler
    void installMessageHandler();

    /// Set the active logging system to manage logging messages.
    void setActiveLogger(mafLogger *logger);

    /// Return the current active logging system.
    mafLogger *activeLogger();

    /// Allows to assign a secondary logger in which redirect the logs (NULL argument will remove the previously assigned logger).
    void setSecondaryLogger(mafLogger *logger);

    /// Return the secondary logger (if any).
    mafLogger *secondaryLogger();

    // Allow to turn On/Off the TestSuite Debug logging modality which puts as prefix a fixed string defined in mafCoreDefinitions.h
    void testSuiteLogMode(bool on = true);

    /// Set the default logger as active one
    void setDeafultLoggerAsActive();

private:
    /// Object constructor.
    mafMessageHandler();

    /// Object destructor.
    ~mafMessageHandler();
    
    mutable QReadWriteLock *m_Lock; ///< Lock variable for thread safe access to object.
    mafLogMode m_LastLogMode; ///< Store the last log mode active for the current logger.
    QStack<mafMsgHandlingFunction> m_OldMsgHandlerStack; ///< Stack of Message handler callback pointers.
    mafLogger *m_ActiveLogger; ///< Current active logging system to which will be forwarded all the incoming messages.
    mafLogger *m_DefaultLogger; ///< mafLoggerConsole is the default logger.
    mafLogger *m_SecondaryLogger; ///< Secondary logger to which forward log messages.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafLogger *mafMessageHandler::secondaryLogger() {
    return m_SecondaryLogger;
}

} // namespace mafCore

#endif // MAFMESSAGEHANDLER_H
