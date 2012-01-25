/*
 *  mafQtClassFactory.h
 *  mafCore
 *
 *  Created by Roberto Mucci on 25/01/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFQTCLASSFACTORY_H
#define MAFQTCLASSFACTORY_H

#include "mafQtObjectFactoryInterface.h"

namespace mafCore {

/**
 Class name: mafQtClassFactory
 This class represent the allocator class for the Qt objects.
 @sa mafObjectFactoryInterface mafObjectFactory
 */
template<class F> class mafQtClassFactory : public mafQtObjectFactoryInterface {
public:
    /// Allocator for Qt objects.
    QObject* make() const;
};

template<class F> QObject* mafQtClassFactory<F>::make() const {
    return new F();
}

} //mafCore

#endif // MAFQTCLASSFACTORY_H
