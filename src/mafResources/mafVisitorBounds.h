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
	mafResources::mafBounds *bounds() const;
    
    /// modify the criteria of calculate the bounds using the absolute position.
    void setAbsolutePoseFlag(bool flag);

    /// retrieve absolute pose flag.
    bool getAbsolutePoseFlag();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafVisitorBounds();

private:
	mafResources::mafBounds *m_Bounds; ///< Store the calculated bounds.
    bool m_AbsolutePoseFlag;
};

inline mafResources::mafBounds *mafVisitorBounds::bounds() const {
	return m_Bounds;
}
    
inline void mafVisitorBounds::setAbsolutePoseFlag(bool flag) {
    m_AbsolutePoseFlag = flag;
}

inline bool mafVisitorBounds::getAbsolutePoseFlag() {
    return m_AbsolutePoseFlag;
}
    

} //end namespace mafResources

#endif  // MAFVISITORBOUNDS_H
