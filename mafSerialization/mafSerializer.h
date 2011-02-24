/*
 *  mafSerializer.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZER_H
#define MAFSERIALIZER_H

// Includes list
#include "mafSerialization_global.h"

namespace mafSerialization {

/**
 Class name: mafSerializer
 Base class that define the interface for MAF3 serializer.
 @sa mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafSerializer : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafSerializer(const mafString code_location = "");

    /// Set the URL to allow the serializer to write/read the DataStream.
    /** Data URL MUST contain the schema associated with the URL itselves.
    For local files the schema if 'file', for web URL the schema can be for example 'http', 'https' or 'ftp'.*/
    void setDataURL(const mafString &url);

    /// Return the URL of the file containing the data.
    const mafUrl dataURL() const;

    /// Initialize the IO device and make it ready to operate.
    virtual void openDevice(mafSerializerOpenMode mode) = 0;

    /// Close the IO device and flush all the information into the media.
    virtual void closeDevice() = 0;

    /// Return the mafIODevice to be connected to the active Codec.
    mafIODevice *ioDevice();

protected:
    /// Object destructor
    /* virtual */ ~mafSerializer();
    mafIODevice *m_IODevice; ///< IO device used to operate on the specific media defined by the strategy used.

private:
    mafUrl m_DataURL; ///< URL that will be written/read the data.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline void mafSerializer::setDataURL(const mafString &url) {
    m_DataURL.setUrl(url);
}

inline const mafUrl mafSerializer::dataURL() const {
    return m_DataURL;
}

inline mafIODevice *mafSerializer::ioDevice() {
    return m_IODevice;
}

} // mafSerialization

#endif // MAFSERIALIZER_H
