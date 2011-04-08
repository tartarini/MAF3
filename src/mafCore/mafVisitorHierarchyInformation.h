/*
 *  mafVisitorHierarchyInformation.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 16/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISITORHIERARCHYINFORMATION_H
#define MAFVISITORHIERARCHYINFORMATION_H


// Includes list
#include "mafVisitor.h"

namespace mafCore {

/**
 Class name: mafVisitorHierarchyInformation
 This class define an operation to be performed on the elements of an object structure.
 mafVisitorHierarchyInformation lets you define an operation that return a list of mafObjectBase.
 @sa mafVisitor
 */
class MAFCORESHARED_EXPORT mafVisitorHierarchyInformation : public mafVisitor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitor);

public:
    /// object constructor
    mafVisitorHierarchyInformation(const QString code_location = "");

    /// Return the Objects List
    mafObjectsList *objectsList();

protected:
    /// Object destructor.
    /* virtual */ ~mafVisitorHierarchyInformation();

    mafObjectsList *m_ObjectsList; ///< contains the list of found objects
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafObjectsList *mafVisitorHierarchyInformation::objectsList() {
    return m_ObjectsList;
}

} //mafCore

#endif // MAFVISITORHIERARCHYINFORMATION_H
