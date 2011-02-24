/*
 *  mafMementoDataSet.cpp
 *  mafResources
 *
 *  Created by Roberto Mucci on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTODATASET_H
#define MAFMEMENTODATASET_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafMemento.h>
#include <mafContainerInterface.h>

namespace mafResources {

// Class forwarding list

/**
Class name: mafMementoDataSet
This class defines the MAF3 memento class for storing a mafDataSet state.
 This is used to restore a previous stored DataSet state (undo mechanism or serialization porpouses).
 @sa mafCore::mafMemento mafResources::mafMementoResource
*/
class MAFRESOURCESSHARED_EXPORT mafMementoDataSet : public mafCore::mafMemento {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoDataSet(const mafString code_location = "");

    /// object overloaded constructor.
    mafMementoDataSet(const mafCore::mafObject *obj, mafPoseMatrix *matrix, mafCore::mafContainerInterface *dataValue, bool binary = true, const mafString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafMementoDataSet();

private:
    friend class mafDataSet;
};

} // namespace mafResources

#endif // MAFMEMENTODATASET_H
