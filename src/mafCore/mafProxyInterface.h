/*
 *  mafProxyInterface.h
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
Class name: mafProxyInterface
This class defines the interface class for MAF3 data values container.
*/
class MAFCORESHARED_EXPORT mafProxyInterface {
public:
    /// Object constructor.
    mafProxyInterface();
    /// Object Constructor
    mafProxyInterface(const mafProxyInterface &dw);

    /// Object destructor.
    /*virtual*/ ~mafProxyInterface() {}

    /// Return the data type for the external data wrapped.
    QString externalDataType();

    /// Return the codec type for the external data wrapped.
    QString externalCodecType();

    /// Set the data type for the external wrapped data.
    void setExternalDataType(const QString data_type);

    /// Set the data type for the external wrapped data.
    void setExternalCodecType(const QString codec_type);

    /// Check if the object is equal to that passed as argument.
    virtual bool isEqual(mafProxyInterface *container);

protected:
    QString m_ExternalDataType; ///< Store the data type for the external wrapped data.
    QString m_ExternalCodecType; ///< Store the codec type for the external wrapped data.
};

inline void mafProxyInterface::setExternalDataType(const QString data_type) {
    m_ExternalDataType = data_type;
}

inline void mafProxyInterface::setExternalCodecType(const QString codec_type) {
    m_ExternalCodecType = codec_type;
}

inline QString mafProxyInterface::externalDataType() {
    return m_ExternalDataType;
}

inline QString mafProxyInterface::externalCodecType() {
    return m_ExternalCodecType;
}

/// Define the pointer to the mafProxyInterface needed to be registered with Q_DECLARE_METATYPE macro.
typedef mafProxyInterface * mafProxyInterfacePointer;

} // namespace mafCore

// Type registered below can be substituted (or integrated) with this one:
Q_DECLARE_METATYPE(mafCore::mafProxyInterfacePointer);
Q_DECLARE_METATYPE(mafCore::mafProxyInterface);  // Handle with care!! A copy is done. Better use declaration above.

#endif // MAFCONTAINERINTERFACE_H
