/*
 *  mafCodecRawBinary.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafCodecRawBinary_H
#define mafCodecRawBinary_H

// Includes list
#include "mafCodecRaw.h"

namespace mafSerialization {

/**
 Class name: mafCodecRawBinary
 Codec to Encode/Decode maf objects into a raw memory dump.
 @sa mafCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafCodecRawBinary : public mafCodecRaw {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCodecRaw);

public:
    /// Object constructor.
    mafCodecRawBinary(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafCore::mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafCore::mafMemento *decode();
protected:
    /// Object destructor.
    /* virtual */ ~mafCodecRawBinary();

private:
    /// marshalling of QVariant data type in order to write the correct type.
    void marshall(const QVariant &value );

    /// unmarshalling of QVariant data type in order to write the correct type.
    QVariant demarshall( QString typeName, int multiplicity);

    QDataStream m_DataStreamRead;
    QDataStream m_DataStreamWrite;
};

} // mafSerialization

#endif // MAFCODECRAWBINARY_H
