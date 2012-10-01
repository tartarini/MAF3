/*
 *  mafUndoStackCommand.h
 *  mafResources
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFUNDOSTACKCOMMAND_H
#define MAFUNDOSTACKCOMMAND_H

// Include File List
#include "mafResourcesDefinitions.h"
#include <mafCommand.h>

namespace mafResources {

// Class forwarding list
class mafOperation;

/**
   Class name: mafUndoStackCommand.
   Class which define a mafCommand for launch Undo or Redo methods of an operation.
*/
class MAFRESOURCESSHARED_EXPORT mafUndoStackCommand : public mafCore::mafCommand {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafCommand);

 public:
    /// Object constructor.
    mafUndoStackCommand(const QString code_location = "");

     /// Object constructor.
     mafUndoStackCommand(mafOperation *rec, QString action, const QString code_location = "");

     ///  Execute receiver function
     virtual void execute();

protected:
     ///  Execute receiver function
     virtual ~mafUndoStackCommand();
};

} //namespace mafResource

#endif // MAFUNDOSTACKCOMMAND_H
