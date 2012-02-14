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
#include "mafVisitor.h"
//#include "mafBounds.h"

namespace mafCore {


/**
 Class name: mafVisitorBounds
 This is the base class which define an operation to be performed on the elements of an object structure. 
 mafVisitorBounds lets you define a new operation without changing the classes of the elements on which it operates.
 */
class MAFCORESHARED_EXPORT mafVisitorBounds : public mafVisitor {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafVisitor);

public:
    /// Object destructor.
    /* virtual */ ~mafVisitorBounds();

    /// Object constructor
    mafVisitorBounds(const QString code_location = "");

    /// Execute the defined operation on visited object passes as argument.
    /*virtual*/ void visit(mafObjectBase *object);
	
	/// Return the calculated bounds.
	//mafBounds bounds() const;
	
private:
	//mafBounds m_Bounds; ///< Store the calculated bounds.
};
/*
inline mafBounds mafVisitorBounds::bounds() const {
	return m_Bounds;
}*/

}

#endif  // MAFVISITORBOUNDS_H
