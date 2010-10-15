/*
 *  mafMementoViewManager.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 13/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTOVIEWMANAGER_H
#define MAFMEMENTOVIEWMANAGER_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafMemento.h>

namespace mafResources {

// Class forwarding list
class mafViewManager;

/**
Class name: mafMementoViewManager
This class defines the mafViewManager memento class for storing created views state.
@sa mafCore::mafMemento mafResources::mafMementoResource
*/
class MAFRESOURCESSHARED_EXPORT mafMementoViewManager : public mafCore::mafMemento {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoViewManager(const mafString code_location = "");
    /// object constructor.
    mafMementoViewManager(const mafCore::mafObjectBase *obj, const mafResourceList *vList, const mafString code_location = "");

private:
    friend class mafViewManager;
};

} // namespace mafResources

#endif // MAFMEMENTOVIEWMANAGER_H
