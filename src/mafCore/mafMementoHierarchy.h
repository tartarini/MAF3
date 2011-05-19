/*
 *  mafMementoHierarchy
 *  mafResources
 *
 *  Created by Roberto Mucci on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFMEMENTOHIERARCHY_H
#define MAFMEMENTOHIERARCHY_H

// Includes list
#include "mafCoreDefinitions.h"
#include "mafMemento.h"
#include "mafHierarchy.h"

namespace mafCore {

// Class forwarding list

/**
Class name: mafMementoHierarchy
This class defines the state which will be saved for mafHierarchy.
 This can be used to stroe/restore a tree of elements (undo mechanism or serialization porpouses).
 @sa mafCore::mafMemento
*/
class MAFCORESHARED_EXPORT mafMementoHierarchy : public mafMemento {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafMemento);

public:
    /// object constructor.
    mafMementoHierarchy(const QString code_location = "");
    /// object overloaded constructor.
    mafMementoHierarchy(const mafObject *obj, const QString code_location = "");

protected:
    /// Object destructor.
    /* virtual */ ~mafMementoHierarchy();

private:
    friend class mafHierarchy;
    
    mafHierarchyPointer m_Hierarchy;
    mafMemento *m_ParentMemento; ///< Memento used as parent memento.
    /// recursive function for hierarchy traversal.
    void traverseTree();
};

} //mafResources
#endif // MAFMEMENTOHIERARCHY_H
