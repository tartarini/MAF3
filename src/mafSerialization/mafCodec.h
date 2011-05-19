/*
 *  mafCodec.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCODEC_H
#define MAFCODEC_H

// Includes list
#include "mafSerialization_global.h"
#include <QPair>



namespace mafSerialization {

/**
 Class name: mafCodec
 Base class that define the interface for MAF3 codec.
 @sa mafCodecRawBinary, mafSerializationManager
 */
    class MAFSERIALIZATIONSHARED_EXPORT mafCodec : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    mafCodec(const QString code_location = "");

    /// Return the codec output type
    const QString encodingType();

    /// Encode the memento into the output type.
    virtual void encode(mafCore::mafMemento *memento) = 0;

    /// Decode the output type into the memento.
    virtual mafCore::mafMemento *decode() = 0;

    /// Set device used to interface the physical media.
    void setDevice(QIODevice *d);

protected:
    /// Recreate hierarchy of memento.
    void buildMementoTree();

    /// Object destructor.
    /* virtual */ ~mafCodec();

    QString m_EncodingType; ///< Encoding type for the codec.
    QIODevice *m_Device; ///< Media device.
    unsigned int m_LevelEncode; ///< Encoding level for compounded memento classes.
    unsigned int m_LevelDecode; ///< Decoding level for compounded memento classes.
    QMap<int, mafCore::mafMemento*> m_MementoMap; ///< Map of mafMemento and its hierarchy level.
    QList<QPair<unsigned int, mafCore::mafMemento*> > m_MementoList;
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline const QString mafCodec::encodingType() {
    return m_EncodingType;
}

inline void mafCodec::setDevice(QIODevice *d) {
    m_Device = d;
}

} // namespace mafSerialization

#endif // MAFCODEC_H
