/*
 *  mafLoggerConsole.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGGERCONSOLE_H
#define MAFLOGGERCONSOLE_H

// Includes list
#include "mafLogger.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafLoggerConsole
 This class defines the MAF3 default logging class that will shows messages to console output.
 @sa maf Logger mafLoggerFile mafLoggerBuffer mafMessageHandler
 */
class MAFCORESHARED_EXPORT mafLoggerConsole : public mafLogger {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafLogger);

public:
    /// Object constructor.
    mafLoggerConsole(const mafString code_location = "");

    /// Clear all the logged messages until now.
    /*virtual*/ void clearLogHistory() {}

protected:
    /// Object destructor.
    /* virtual */ ~mafLoggerConsole();
    /// Method used to log the given message to the console.
    /*virtual*/ void loggedMessage(const mafMsgType type, const mafString &msg);
};

}

#endif // MAFLOGGERCONSOLE_H
