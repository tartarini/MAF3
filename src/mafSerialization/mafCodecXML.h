/*
 *  mafCodecXML.h
 *  mafSerialization
 *
 *  Created by Daniele Giunchi and Roberto Mucci on 19/01/10.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFCODECXML_H
#define MAFCODECXML_H

// Includes list
#include "mafCodec.h"

namespace mafSerialization {

/**
 Class name: mafCodecXML
 Codec to Encode/Decode maf objects into XML structure.
 @sa mafCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafCodecXML : public mafCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCodec);

public:
    /// Object constructor.
    mafCodecXML(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafCore::mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafCore::mafMemento *decode();

protected:
    /// Object destructor.
    /* virtual */ ~mafCodecXML();

private:
    /// marshalling of QVariant data type in order to write the correct type.
    void marshall(const QVariant &value );

    /// unmarshalling of QVariant data type in order to write the correct type.
    QVariant demarshall( QXmlStreamReader *xmlStream );

    QXmlStreamWriter m_XMLStreamWriter; ///
    QXmlStreamReader m_XMLStreamReader; 

    QDomDocument m_XMLDocument;
    QDomNode m_CurrentNode;

    int m_MementoLevel; 
    bool m_Valid;
};


} // mafSerialization

#endif //MAFCODECXML_H
