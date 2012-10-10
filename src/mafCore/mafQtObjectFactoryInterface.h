/*
 *  mafQtObjectFactoryInterface.h
 *  mafCore
 *
 *  Created by Roberto Mucci on 25/01/12.
 *  Copyright 2012 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFQTOBJECTFACTORYINTERFACE_H
#define MAFQTOBJECTFACTORYINTERFACE_H

#include "mafObjectBase.h"


namespace mafCore {

/**
 Class name: mafQtObjectFactoryInterface
 This class represent the abstract interface for factory allocator of the MAF3 objects.
 @sa mafClassFactory mafObjectFactory
 */
class MAFCORESHARED_EXPORT mafQtObjectFactoryInterface {
public:
    /// Object destructor.
    virtual ~mafQtObjectFactoryInterface();

    /// Allocator for Qt objects.
    virtual QObject* make() const = 0;
};

} //mafCore

#endif // MAFQTOBJECTFACTORYINTERFACE_H
