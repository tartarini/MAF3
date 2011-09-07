/*
 *  mafCommand.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCOMMAND_H
#define MAFCOMMAND_H

// Includes list
#include "mafObjectBase.h"

namespace mafCore {

// Class forwarding list

/**
Class name: mafCommand
This class defines a pure interface for the command pattern, used for example in undo stack mechanism
of operations.
*/
class MAFCORESHARED_EXPORT mafCommand : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafCommand(const QString code_location = "");

    /// define the single API interface of all he classes that inherits from it
    virtual void execute() = 0;

Q_SIGNALS:
    /// Signal used to trigger the start of the given command.
    void executeCommand();

protected:
    /// Object destructor.
    /*virtual*/ ~mafCommand();
};

} //namespace mafCore

#endif // MAFCOMMAND_H
