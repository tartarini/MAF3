/*
 *  mafSmartObject.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 18/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *  
 *  See Licence at: http://tiny.cc/QXJ4D
 *  
 */

#ifndef MAFSMARTPOINTER_H
#define MAFSMARTPOINTER_H

// Includes list
#include "mafObjectBase.h"
#include <QPointer>

namespace mafCore {

// Class forwarding list

/**
 Class name: mafSmartObject
 This class represent simply a subclass of QPointer needed only for naming convention reason.
 */
template <class T>
class mafSmartPointer : public QPointer<T> {
public:
    /// mafSmartPointer constructor.
    mafSmartPointer(T *p);

    /// mafSmartPointer destructor.
    ~mafSmartPointer();

protected:
    
private:
    mafObjectBase *m_ObjectBase;
};

#include "mafSmartPointer.txx"

} //namespace mafCore



#endif  // MAFSMARTPOINTER_H
