/*
 *  mafMessageHandler.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
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
 This singletone provides the dispatching of logging messages.
 The message if forwarded to the current active logger only if it is enabled and the message type has not ben filtered.
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

    /// Allows to reinstall the mssage handler
    void installMessageHandler();

    /// Set the active logging syatem to manage logging messages.
    void setActiveLogger(mafLogger *logger);

    /// Return the current active logging system.
    mafLogger *activeLogger() {return m_ActiveLogger;}

    // Allow to turn On/Off the TestSuite Debug logging modality which puts as prefix a fixed string defined in mafCoreDefinitions.h
    void testSuiteLogMode(bool on = true);

    /// Set the default logger as active one
    void setDeafultLoggerAsActive();

private:
    /// Object constructor.
    mafMessageHandler();

    /// Object destructor.
    ~mafMessageHandler();

    mafLogMode m_LastLogMode; ///< Store the last log mode active for the current logger.
    mafMsgHandlingFunction m_OldMsgHandler; ///< Message handler callback pointer.
    mafLogger *m_ActiveLogger; ///< Current active logging system to which will be forwarded all the incoming messages.
    mafLogger *m_DefaultLogger; ///< mafLoggerConsole is the default logger.
};

}

#endif // MAFMESSAGEHANDLER_H
