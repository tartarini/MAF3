/*
 *  mafClassFactory.h
 *  mafCore
 *
 *  Created by Daniele Giunchi on 10/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCLASSFACTORY_H
#define MAFCLASSFACTORY_H

#include "mafObjectFactoryInterface.h"

namespace mafCore {

/**
 Class name: mafClassFactory
 This class represent the allocator class for the MAF3 objects.
 @sa mafObjectFactoryInterface mafObjectFactory
 */
template<class F> class mafClassFactory : public mafObjectFactoryInterface {
public:
    /// Allocator for MAF objects.
    mafObjectBase* make(const QString codeLocation = "") const;

};

template<class F> mafObjectBase* mafClassFactory<F>::make(const QString codeLocation) const {
    return new F(codeLocation); 
}

} //mafCore

#endif // MAFCLASSFACTORY_H
