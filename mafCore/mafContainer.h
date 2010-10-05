/*
 *  mafContainer.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCONTAINER_H
#define MAFCONTAINER_H

// Includes list
#include "mafContainerInterface.h"
#include <typeinfo>

namespace mafCore {

// Class forwarding list

/**
Class name: mafContainer
This class defines the base concrete class container for the MAF3 dataset values.
*/
template<typename T>
class mafContainer : public mafContainerInterface {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafContainerInterface);
    typedef void (T::*mafExternalDataDestructorPointer)();

public:
    /// Object constructor.
    mafContainer();

    /// Object destructor.
    ~mafContainer();

    /// Redefined operator to have access to the external data owned.
    operator T *() const;

    /// Redefined assignment operator to initialize external data reference.
    void operator=(T*val);

    /// Redefined operator to have access to the external data owned.
    T *operator->();

    /// Return the pointer to the data value stored into the container.
    T *externalData();

    /// External data should assign the pointer to its custom method to destroy
    void setDestructionFunction(mafExternalDataDestructorPointer destruc_function);

private:
    /// Update the string containing the data type related to the external wrapped data.
    void updateExternalDataType();

    T *m_ExternalData; ///< Pointer to the data value.
    mafExternalDataDestructorPointer m_ExternalDestructor; ///< Pointer to external destructor
};

#include "mafContainer.txx"

} // namespace mafCore

#endif // MAFCONTAINER_H
