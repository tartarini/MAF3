/*
 *  mafVisitorFindObjects.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 16/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVISITORFINDOBJECTS_H
#define MAFVISITORFINDOBJECTS_H


// Includes list
#include "mafVisitor.h"

namespace mafCore {

/**
 Class name: mafVisitorFindObjects
 This class define an operation to be performed on the elements of an object structure.
 mafVisitorFindObjects lets you define an operation that return a list of mafObjectBase.
 @sa mafVisitor
 */
class MAFCORESHARED_EXPORT mafVisitorFindObjects : public mafVisitor {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitor);

public:
    /// object constructor
    mafVisitorFindObjects(const mafString code_location = "");

    /// Return the Objects List
    mafObjectsList *objectsList();

protected:
    /// Object destructor.
    /* virtual */ ~mafVisitorFindObjects();

    mafObjectsList *m_ObjectsList; ///< contains the list of found objects
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafObjectsList *mafVisitorFindObjects::objectsList() {
    return m_ObjectsList;
}

} //mafCore

#endif // MAFVISITORFINDOBJECTS_H
