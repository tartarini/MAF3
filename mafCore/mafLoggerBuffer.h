/*
 *  mafLoggerBuffer.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 17/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFLOGGERBUFFER_H
#define MAFLOGGERBUFFER_H

// Includes list
#include "mafLogger.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafLoggerBuffer
 This class defines the MAF3 logging class that will store messages into a buffer that can be queryed later or linked to a UI Widget.
 @sa mafLogger mafLoggerFile mafLoggerConsole mafMessageHandler
 */
class MAFCORESHARED_EXPORT mafLoggerBuffer : public mafLogger {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafLogger);

public:
    /// Object constructor.
    mafLoggerBuffer(const mafString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// return the buffer containing all the logged messages.
    mafString &bufferLog() {return m_BufferLog;}

    /// Clear all the logged messages until now.
    /*virtual*/ void clearLogHistory() {m_BufferLog.clear();}

protected:
    /// Method used to log the given message to the buffer.
    /*virtual*/ void loggedMessage(const mafMsgType type, const mafString &msg);

private:
    mafString m_BufferLog; ///< String containing all the logged messages for a specific session.
};

}

#endif // MAFLOGGERBUFFER_H
