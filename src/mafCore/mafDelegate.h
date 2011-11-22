/*
 *  mafDelegate.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDELEGATE_H
#define MAFDELEGATE_H

// Includes list
#include "mafCore_global.h"

namespace mafCore {

// Class forwarding list

/**
 Class name: mafDelegate
 This defines the base class for objects used to customize behaviour of high level classes.
 All methods are implemented as public slots so to be able to call the method from outside the class.
*/
class MAFCORESHARED_EXPORT mafDelegate : public QObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafDelegate(const QString code_location = "");
    
    /// Object destructor.
    ~mafDelegate();
    
    /// Check if a method has been defined into the delegate class.
    bool isMethodDefined(QString signature);
    
    /// Return true or false depending if the delegate class wants that the caller execute its own code or skip it.
    virtual bool shouldExecuteLocalCode();
};

} //namespace mafCore

#endif // MAFDELEGATE_H
