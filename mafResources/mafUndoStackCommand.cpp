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

#include "mafUndoStackCommand.h"
#include "mafOperation.h"

using namespace mafResources;

mafUndoStackCommand::~mafUndoStackCommand() {
}

mafUndoStackCommand::mafUndoStackCommand(mafOperation *rec, QString action): m_Receiver(rec), m_Action(action) {
}

void mafUndoStackCommand::execute() {
    m_Receiver->metaObject()->invokeMethod(m_Receiver, m_Action.toAscii());
}
