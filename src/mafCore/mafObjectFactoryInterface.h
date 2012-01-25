/*
 *  mafObjectFactoryInterface.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFOBJECTFACTORYINTERFACE_H
#define MAFOBJECTFACTORYINTERFACE_H

#include "mafObjectBase.h"


namespace mafCore {

/**
 Class name: mafObjectFactoryInterface
 This class represent the abstract interface for factory allocator of the MAF3 objects.
 @sa mafClassFactory mafObjectFactory
 */
class MAFCORESHARED_EXPORT mafObjectFactoryInterface {
public:
    /// Object destructor.
    virtual ~mafObjectFactoryInterface();

    /// Allocator for Qt objects.
    virtual mafObjectBase* make(const QString codeLocation = "") const = 0;
};

} //mafCore

#endif // MAFOBJECTFACTORYINTERFACE_H
