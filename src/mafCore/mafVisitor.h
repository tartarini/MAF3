/*
 *  mafVisitor.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 16/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFVISITOR_H
#define MAFVISITOR_H

// Includes list
#include "mafObjectBase.h"

namespace mafCore {


/**
 Class name: mafVisitor
 This is the base class which define an operation to be performed on the elements of an object structure. 
 mafVisitor lets you define a new operation without changing the classes of the elements on which it operates.
 */
class MAFCORESHARED_EXPORT mafVisitor : public mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor
    mafVisitor(const QString code_location = "");

    /// Execute the defined operation on visited object passes as argument.
    virtual void visit(mafObjectBase *object) = 0;
    
protected:
    /// Object destructor.
    /* virtual */ ~mafVisitor();
};

}

#endif  // MAFVISITOR_H
