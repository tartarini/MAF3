/*
 *  mafCodecRaw.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 14/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafCodecRaw_H
#define mafCodecRaw_H

// Includes list
#include "mafCodec.h"

namespace mafSerialization {

/**
 Class name: mafCodecRaw
 This is the base class for the MAF3 to encode/decode maf objects
 into a raww memory dump.
 @sa mafCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafCodecRaw : public mafCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCodec);

public:
    /// Object constructor.
    mafCodecRaw(const QString code_location = "");

    /// Encode the memento into the output type.
    virtual void encode(mafCore::mafMemento *memento) = 0;

    /// Decode the output type into the memento.
    virtual mafCore::mafMemento *decode() = 0;

protected:
    /// Object destructor.
    /* virtual */ ~mafCodecRaw();

private:
    /// marshalling of QVariant data type in order to write the correct type.
    virtual void marshall(const QVariant &value ) = 0;

    /// unmarshalling of QVariant data type in order to write the correct type.
    virtual QVariant demarshall( QString typeName, int multiplicity) = 0;

};

} // mafSerialization

#endif // mafCodecRaw_H
