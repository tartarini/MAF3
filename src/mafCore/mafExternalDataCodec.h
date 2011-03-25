/*
 *  mafExternalDataCodec.h
 *  mafCore
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEXTERNALDATACODEC_H
#define MAFEXTERNALDATACODEC_H

// Includes list
#include "mafContainerInterface.h"
#include "mafObjectBase.h"

namespace mafCore {

/**
 Class name: mafExternalDataCodec
 Base class that define the strategy for coding and decoding external data types.
 */
class MAFCORESHARED_EXPORT mafExternalDataCodec : public mafObjectBase {
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafExternalDataCodec(const QString &code_location = "");

    /// Object constructor.
    mafExternalDataCodec(mafContainerInterface *data, const QString &code_location = "");

    /// Encode external data into a QDataStream.
    virtual char *encode(bool binary = true) = 0;

    /// Decode external data from the QDataStream.
    virtual void decode(const char *input_string, bool binary = true) = 0;

    /// Allow to set the reference to the external data type. The memory deallocation is in charge at the caller.
    void setExternalData(mafContainerInterface *data);

    /// Allow to retrieve external data. The memory deallocation is in charge at the caller.
    mafContainerInterface *externalData();

    /// Set the dimension of input_string
    void setStringSize(unsigned int size);

    /// Return the dimension of input_string
    unsigned int stringSize();

protected:
    /// Object destructor.
    /* virtual */~mafExternalDataCodec();

    mafContainerInterface *m_ExternalData; ///< External data type do be coded/encoded into/from file.
    unsigned int m_StringSize; ///< Size of the input/output string.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafExternalDataCodec::setExternalData(mafContainerInterface *data) {
    m_ExternalData = data;
}

inline mafContainerInterface *mafExternalDataCodec::externalData() {
    return m_ExternalData;
}

inline void mafExternalDataCodec::setStringSize(unsigned int size) {
    m_StringSize = size;
}

inline unsigned int mafExternalDataCodec::stringSize() {
    return m_StringSize;
}

} // namespace mafCore

#endif // MAFEXTERNALDATACODEC_H
