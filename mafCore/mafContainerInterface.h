/*
 *  mafContainerInterface.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCONTAINERINTERFACE_H
#define MAFCONTAINERINTERFACE_H

// Includes list
#include "mafCore_global.h"

namespace mafCore {

// Class forwarding list

/**
Class name: mafContainerInterface
This class defines the interface class for MAF3 data values container.
*/
class MAFCORESHARED_EXPORT mafContainerInterface {
public:
    /// Object constructor.
    mafContainerInterface();
    /// Object Constructor
    mafContainerInterface(const mafContainerInterface &dw);

    /// Object destructor.
    /*virtual*/ ~mafContainerInterface() {}

    /// Return the data type for the external data wrapped.
    mafString externalDataType();

    /// Return the codec type for the external data wrapped.
    mafString externalCodecType();

    /// Set the data type for the external wrapped data.
    void setExternalDataType(const mafString data_type);

    /// Set the data type for the external wrapped data.
    void setExternalCodecType(const mafString codec_type);

    /// Check if the object is equal to that passed as argument.
    virtual bool isEqual(mafContainerInterface *container);

protected:
    mafString m_ExternalDataType; ///< Store the data type for the external wrapped data.
    mafString m_ExternalCodecType; ///< Store the codec type for the external wrapped data.
};

inline void mafContainerInterface::setExternalDataType(const mafString data_type) {
    m_ExternalDataType = data_type;
}

inline void mafContainerInterface::setExternalCodecType(const mafString codec_type) {
    m_ExternalCodecType = codec_type;
}

inline mafString mafContainerInterface::externalDataType() {
    return m_ExternalDataType;
}

inline mafString mafContainerInterface::externalCodecType() {
    return m_ExternalCodecType;
}

/// Define the pointer to the mafContainerInterface needed to be registered with Q_DECLARE_METATYPE macro.
typedef mafContainerInterface * mafContainerInterfacePointer;

} // namespace mafCore

// Type registered below can be substituted (or integrated) with this one:
Q_DECLARE_METATYPE(mafCore::mafContainerInterfacePointer);
Q_DECLARE_METATYPE(mafCore::mafContainerInterface);  // Handle with care!! A copy is done. Better use declaration above.

#endif // MAFCONTAINERINTERFACE_H
