/*
 *  mafSerializationDefinitions.h
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSERIALIZATIONDEFINITIONS_H
#define MAFSERIALIZATIONDEFINITIONS_H

// Includes list
#include <mafCoreDefinitions.h>
#include <mafObject.h>
#include <mafMemento.h>
#include <mafObjectFactory.h>
#include <mafIdProvider.h>

// Qt includes
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QIODevice>
#include <QUrl>

namespace mafSerialization {

#define mafUrl QUrl
#define mafXmlStreamWriter QXmlStreamWriter
#define mafDomNamedNodeMap QDomNamedNodeMap
#define mafDomNodeList QDomNodeList
#define mafDomDocument QDomDocument
#define mafDomElement QDomElement
#define mafDomNode QDomNode

#define mafIODevice QIODevice

// Class forwarding list
class mafCodec;

typedef mafHash<mafString, mafString> mafEncodingHash; ///< Hash table that associate the object type name (mafObject, mafResource,...) and the the output data type (RAW, VTK, STL...)
typedef mafHash<mafString, mafString> mafCodecHash; ///< Hash table that associate the output data type (RAW, VTK, STL...) and the corresponding codec type (mafCodecRaw...)
typedef mafHash<mafString, mafString> mafSerializerHash; ///< Hash table that associate the URL schema with the corresponding serializer class (Eg. file, mafSerialization::mafSerializerFileSystem)

typedef mafList<mafString> mafEncodingList; ///< List of encoding types (RAW, STL ...).

///< Enum that identify the mafSerializer open modality.
typedef enum {
    mafSerializerOpenModeIn,
    mafSerializerOpenModeOut,
    mafSerializerOpenModeBidirectional
} mafSerializerOpenMode;


} // mafSerialization

#endif // MAFSERIALIZATIONDEFINITIONS_H
