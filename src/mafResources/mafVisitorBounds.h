/*
 *  mafVisitorBounds.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 10/02/12.
 *  Copyright 2012 B3C. All rights reserved.
 *  
 *  See License at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFVISITORBOUNDS_H
#define MAFVISITORBOUNDS_H

// Includes list
#include "mafResourcesDefinitions.h"
#include <mafVisitor.h>
#include <mafBounds.h>

namespace mafResources {

/**
 Class name: mafVisitorBounds
 This is the base class which define an operation to be performed on the elements of an object structure. 
 mafVisitorBounds lets you define a new operation without changing the classes of the elements on which it operates.
 */
class MAFRESOURCESSHARED_EXPORT mafVisitorBounds : public mafCore::mafVisitor {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitor);

public:
    /// Object constructor
    mafVisitorBounds(const QString code_location = "");

    /// Execute the defined operation on visited object passes as argument.
    /*virtual*/ void visit(mafCore::mafObjectBase *object);
	
	/// Return the calculated bounds.
	mafCore::mafBounds *bounds() const;
    
protected:
    /// Object destructor.
    /* virtual */ ~mafVisitorBounds();

private:
	mafCore::mafBounds *m_Bounds; ///< Store the calculated bounds.
};

inline mafCore::mafBounds *mafVisitorBounds::bounds() const {
	return m_Bounds;
}

} //end namespace mafResources

#endif  // MAFVISITORBOUNDS_H
