/*
 *  mafProxy.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCONTAINER_H
#define MAFCONTAINER_H

// Includes list
#include "mafProxyInterface.h"
#include <typeinfo>

namespace mafCore {

// Class forwarding list

/**
Class name: mafProxy
This class defines the base concrete class container for the MAF3 dataset values.
*/
template<typename T>
class mafProxy : public mafProxyInterface {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafProxyInterface);
    typedef void (T::*mafExternalDataDestructorPointer)();
    typedef QString (*mafClassTypeNameFunctionPointer)(T*);    

public:
    /// Object constructor.
    mafProxy();

    /// Object destructor.
    ~mafProxy();

    /// Redefined operator to have access to the external data owned.
    operator T *() const;

    /// Redefined assignment operator to initialize external data reference.
    void operator=(T*val);

    /// Redefined operator to have access to the external data owned.
    T *operator->();

    /// Return the pointer to the data value stored into the container.
    T *externalData();

    /// External data should assign the pointer to its custom method to destroy
    void setDestructionFunction(mafExternalDataDestructorPointer destructFunction);

    /// Check if the object is equal to that passed as argument.
    /*virtual*/ bool isEqual(mafProxyInterface *container);
    
    /// Optional function pointer used to extract class type from assigned data.
    void setClassTypeNameFunction(mafClassTypeNameFunctionPointer classTypeNameFunction);


private:
    /// Update the string containing the data type related to the external wrapped data.
    void updateExternalDataType();

    T *m_ExternalData; ///< Pointer to the data value.
    mafExternalDataDestructorPointer m_ExternalDestructor; ///< Pointer to external destructor
    mafClassTypeNameFunctionPointer  m_ClassTypeNameFunction; ///< Pointer to function for extract object type name.
};

#include "mafProxy.txx"

} // namespace mafCore

#endif // MAFCONTAINER_H
