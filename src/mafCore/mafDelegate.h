/*
 *  mafDelegate.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDELEGATE_H
#define MAFDELEGATE_H

// Includes list
#include "mafReferenceCounted.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafDelegate
 This defines the base class for objects used to customize behavior of high level classes.
 All methods are implemented as public slots so to be able to call the method from outside the class.
*/
class MAFCORESHARED_EXPORT mafDelegate : public mafReferenceCounted {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafReferenceCounted);

public:
    /// Object constructor.
    mafDelegate();
    
    /// Check if a method has been defined into the delegate class.
    bool isMethodDefined(QString signature);

    /// Execute delegated method.
    void executeMethod(QString signature, QGenericReturnArgument ret);
    
    /// Return true or false depending if the delegate class wants that the caller execute its own code or skip it.
    virtual bool shouldExecuteLocalCode();

protected:
    /// Object destructor.
    ~mafDelegate();
};
    
typedef mafDelegate * mafDelegatePointer;

} //namespace mafCore

Q_DECLARE_METATYPE(mafCore::mafDelegatePointer);

#endif // MAFDELEGATE_H
