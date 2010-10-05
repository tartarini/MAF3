/*
 *  mafMementoVME.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTOVME_H
#define MAFMEMENTOVME_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafMemento.h>

namespace mafResources {

// Class forwarding list
class mafDataPipe;
class mafVME;

/**
Class name: mafMementoVME
This class defines the MAF3 memento class for storing a mafVME state.
 This is used to restore a previous stored VME state (undo mechanism or serialization porpouses).
 @sa mafCore::mafMemento mafResources::mafMementoResource
*/
class MAFRESOURCESSHARED_EXPORT mafMementoVME : public mafCore::mafMemento {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoVME(const mafString code_location = "");
    /// object overloaded constructor.
    mafMementoVME(const mafCore::mafObject *obj, mafDataSetCollection *collection, mafDataPipe *pipe, bool binary = true, const mafString code_location = "");

private:
    friend class mafVME;
};

} // namespace mafResources

#endif // MAFMEMENTOVME_H
