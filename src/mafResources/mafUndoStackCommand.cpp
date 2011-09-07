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

mafUndoStackCommand::mafUndoStackCommand(const QString code_location) : mafCommand(code_location) {
}

mafUndoStackCommand::mafUndoStackCommand(mafOperation *rec, QString action, const QString code_location) : mafCommand(code_location) {
    QString action_sig = CALLBACK_SIGNATURE;
    action_sig.append(action);

    bool result = connect(this, SIGNAL(executeCommand()), rec, action_sig.toAscii(), Qt::DirectConnection);
}

mafUndoStackCommand::~mafUndoStackCommand() {
}

void mafUndoStackCommand::execute() {
    Q_EMIT executeCommand();
}
