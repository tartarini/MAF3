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
    QString externalDataType();

    /// Return the codec type for the external data wrapped.
    QString externalCodecType();

    /// Set the data type for the external wrapped data.
    void setExternalDataType(const QString data_type);

    /// Set the data type for the external wrapped data.
    void setExternalCodecType(const QString codec_type);

    /// Check if the object is equal to that passed as argument.
    virtual bool isEqual(mafContainerInterface *container);

protected:
    QString m_ExternalDataType; ///< Store the data type for the external wrapped data.
    QString m_ExternalCodecType; ///< Store the codec type for the external wrapped data.
};

inline void mafContainerInterface::setExternalDataType(const QString data_type) {
    m_ExternalDataType = data_type;
}

inline void mafContainerInterface::setExternalCodecType(const QString codec_type) {
    m_ExternalCodecType = codec_type;
}

inline QString mafContainerInterface::externalDataType() {
    return m_ExternalDataType;
}

inline QString mafContainerInterface::externalCodecType() {
    return m_ExternalCodecType;
}

/// Define the pointer to the mafContainerInterface needed to be registered with Q_DECLARE_METATYPE macro.
typedef mafContainerInterface * mafContainerInterfacePointer;

} // namespace mafCore

// Type registered below can be substituted (or integrated) with this one:
Q_DECLARE_METATYPE(mafCore::mafContainerInterfacePointer);
Q_DECLARE_METATYPE(mafCore::mafContainerInterface);  // Handle with care!! A copy is done. Better use declaration above.

#endif // MAFCONTAINERINTERFACE_H
