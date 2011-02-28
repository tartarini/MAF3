/*
 *  mafUndoStackCommand.h
 *  mafResources
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009 B3C. All rights reserved.
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
 public:
    /// define the action (undo or redo) of an operation
    typedef void(mafOperation::*Action)();

     /// Object constructor.
     mafUndoStackCommand(mafOperation *rec, QString action);

     ///  Execute receiver function
     virtual void execute();

     ///  Execute receiver function
     virtual ~mafUndoStackCommand();

private:
    mafOperation *m_Receiver; ///< represent the operation with which call undo or redo
    QString m_Action; ///< Action to be executed by the command pattern.
};

} //namespace mafResource

#endif // MAFUNDOSTACKCOMMAND_H
