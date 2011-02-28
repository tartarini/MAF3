/*
 *  mafSerializationManager.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSerializationManager.h"
#include "mafSerializationRegistration.h"
#include "mafSerializer.h"
#include "mafCodec.h"
#include <mafExternalDataCodec.h>
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafSerialization;

mafSerializationManager* mafSerializationManager::instance() {
    static mafSerializationManager instanceSerializationManager;
    return &instanceSerializationManager;
}

mafSerializationManager::mafSerializationManager(const QString code_location) : mafObjectBase(code_location) {
    mafSerializationRegistration::registerSerializationObjects();

    initializeConnections();

    plugCodec("*","RAW","mafSerialization::mafCodecRawBinary");
    plugCodec("*","VOLUME_LOD","mafSerialization::mafCodecRawVolume");
    plugCodec("*","VOLUME_BRICKED_LOD","mafSerialization::mafCodecBrickedVolume");
    plugSerializer("file", "mafSerialization::mafSerializerFileSystem");
}

mafSerializationManager::~mafSerializationManager() {
}

void mafSerializationManager::initializeConnections() {
    // Create the IDs required to Save/Load MAF3 data.
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.serialization.plugCodec");
    provider->createNewId("maf.local.serialization.plugSerializer");
    provider->createNewId("maf.local.serialization.save");
    provider->createNewId("maf.local.serialization.load");
    provider->createNewId("maf.local.serialization.export");
    provider->createNewId("maf.local.serialization.import");
    provider->createNewId("maf.local.serialization.saveExternalData");
    provider->createNewId("maf.local.serialization.loadExternalData");
    m_MementoLoadedId = "maf.local.serialization.mementoLoaded";
    provider->createNewId(m_MementoLoadedId);
    m_ExtDataImportedId = "maf.local.serialization.extDataImported";
    provider->createNewId(m_ExtDataImportedId);

    // Register API signals.
    mafRegisterLocalSignal("maf.local.serialization.plugCodec", this, "plugCodecInModule(const QString &, const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.plugSerializer", this, "plugSerializerInModule(const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.save", this, "save(mafCore::mafMemento *, const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.load", this, "load(const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.export", this, "exportData(mafCore::mafContainerInterface *, const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.import", this, "importData(const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.saveExternalData", this, "saveExtData(char *, const QString &, int)");
    mafRegisterLocalSignal("maf.local.serialization.loadExternalData", this, "loadExtData(const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.mementoLoaded", this, "mementoLoaded(mafCore::mafMemento *)");
    mafRegisterLocalSignal("maf.local.serialization.extDataImported", this, "extDataImported(mafCore::mafContainerInterface *)");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.serialization.plugCodec", this, "plugCodec(const QString &, const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.plugSerializer", this, "plugSerializer(const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.save", this, "saveMemento(mafCore::mafMemento *, const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.load", this, "loadMemento(const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.export", this, "exportExternalData(mafCore::mafContainerInterface *, const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.import", this, "importExternalData(const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.saveExternalData", this, "saveExternalData(char *, const QString &, int)");
    mafRegisterLocalCallback("maf.local.serialization.loadExternalData", this, "loadExternalData(const QString &)");
}

void mafSerializationManager::saveMemento(mafMemento *memento, const QString &url, const QString &encode_type) {
    REQUIRE(memento != NULL);

    // Create an instance of the codec type corresponding to the object's memento
    mafEncodingList encoding_list = encodingTypeList(memento);
    bool encoding_not_found = true;
    QString encodingTypeItem = "";
    QString codecType = "";
    if(!encode_type.isEmpty()) {
        foreach(encodingTypeItem, encoding_list) {
            if(encodingTypeItem == encode_type) {
                encoding_not_found = false;
                codecType = m_CodecHash[encodingTypeItem];
                break;
            }
        }
    }

    if(encoding_not_found) {
        if(encode_type != "RAW") {
            qWarning("%s", mafTr("Codec not found for %1 type; will be used the RAW default.").arg(encode_type).toAscii().data());
        }
        codecType = m_CodecHash["RAW"];
    }

    // Pass the memento to the codec and ask him to encode the given data.
    mafCodec *codec = (mafCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    mafUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
        return;
    }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the output connection.
    ser->openDevice(mafSerializerOpenModeOut);

    // Give the device to the codec
    codec->setDevice(ser->ioDevice());
    // ... and encode data into the stream
    codec->encode(memento);

    // Finally close the connection.
    ser->closeDevice();

    mafDEL(codec);
    mafDEL(ser);
}

void mafSerializationManager::loadMemento(const QString &url, const QString &encode_type) {
    if(!m_CodecHash.contains(encode_type)) {
        qCritical("%s", mafTr("No codec associated with the '%1' encoding type.").arg(encode_type).toAscii().data());
        return;
    }

    QString codecType = m_CodecHash[encode_type];
    mafCodec *codec = (mafCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    mafUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
        return;
    }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the input connection.
    ser->openDevice(mafSerializerOpenModeIn);

    // Give the device to the codec
    codec->setDevice(ser->ioDevice());

    mafMemento *m = codec->decode();

    // Finally close the connection.
    ser->closeDevice();

    mafDEL(codec);
    mafDEL(ser);

    // Notify through the event bus that the memento has beed loaded by using the ID: "maf.local.serialization.mementoLoaded"
    mafEventArgumentsList list;
    list.append(mafEventArgument(mafCore::mafMemento *, m));
    mafEventBusManager::instance()->notifyEvent(m_MementoLoadedId, mafEventTypeLocal, &list);
}

void mafSerializationManager::exportExternalData(mafCore::mafContainerInterface *externalData, const QString &url, const QString &encode_type) {
    REQUIRE(externalData != NULL);

    // Create an instance of the codec type corresponding to the object's memento
    mafEncodingList encoding_list = m_EncodingHash.values(externalData->externalDataType());
    bool encoding_not_found = true;
    QString encodingTypeItem = "";
    QString codecType = "";
    if(!encode_type.isEmpty()) {
        foreach(encodingTypeItem, encoding_list) {
            if(encodingTypeItem == encode_type) {
                encoding_not_found = false;
                codecType = m_CodecHash[encodingTypeItem];
                break;
            }
        }
    }

    if(encoding_not_found) {
        if(encode_type != "RAW") {
            qWarning("%s", mafTr("Codec not found for %1 type; will be used the RAW default.").arg(encode_type).toAscii().data());
        }
        codecType = m_CodecHash["RAW"];
    }

    // Pass the memento to the codec and ask him to encode the given data.
    mafExternalDataCodec *codec = (mafExternalDataCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    mafUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
         return;
     }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the output connection.
    ser->openDevice(mafSerializerOpenModeOut);

    QDataStream stream(ser->ioDevice());

    // ... and encode data into the string
    codec->setExternalData(externalData);
    char *outputString = codec->encode();

    //write binary data
    stream.writeBytes(outputString, codec->stringSize());

    // Finally close the connection.
    ser->closeDevice();

    mafDEL(codec);
    mafDEL(ser);
}

void mafSerializationManager::saveExternalData(char *externalDataString, const QString &url, int stringSize) {
    REQUIRE(externalDataString != NULL);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    mafUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
         return;
     }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the output connection.
    ser->openDevice(mafSerializerOpenModeOut);

    QDataStream stream(ser->ioDevice());

    //write binary data
    stream.writeBytes(externalDataString, stringSize);

    // Finally close the connection.
    ser->closeDevice();

    mafDEL(ser);
}

void mafSerializationManager::importExternalData(const QString &url, const QString &encode_type) {
    if(!m_CodecHash.contains(encode_type)) {
        qCritical("%s", mafTr("No codec associated with the '%1' encoding type.").arg(encode_type).toAscii().data());
        return;
    }

    QString codecType = m_CodecHash[encode_type];
    mafExternalDataCodec *codec = (mafExternalDataCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    mafUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
         return;
     }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the input connection.
    ser->openDevice(mafSerializerOpenModeIn);

    unsigned int size;
    char * inputString;;
    QDataStream stream(ser->ioDevice());
    stream.readBytes(inputString, size);

    // Finally close the connection.
    ser->closeDevice();
    mafDEL(ser);

    // Give the device to the codec
    codec->setStringSize(size);
    codec->decode(inputString);

    // Notify through the event bus that the Externaldata has beed imported by using the ID: "maf.local.serialization.extDataImported"
    mafEventArgumentsList list;
    list.append(mafEventArgument(mafCore::mafContainerInterface *, codec->externalData()));
    mafEventBusManager::instance()->notifyEvent(m_ExtDataImportedId, mafEventTypeLocal, &list);
    mafDEL(codec);
}

QByteArray mafSerializationManager::loadExternalData(const QString &url) {

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    mafUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
         return 0;
     }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the input connection.
    ser->openDevice(mafSerializerOpenModeIn);

    unsigned int size;
    char * inputString;;
    QDataStream stream(ser->ioDevice());
    stream.readBytes(inputString, size);

    // Finally close the connection.
    ser->closeDevice();
    mafDEL(ser);

    QByteArray stringArray;
    stringArray = QByteArray::fromRawData(inputString, size);
    return stringArray;
}

void mafSerializationManager::plugCodec(const QString &object_type,const QString &encoding_type, const QString &codecType) {
    m_EncodingHash.insertMulti(object_type, encoding_type);
    m_CodecHash.insertMulti(encoding_type, codecType);
}

mafEncodingList mafSerializationManager::encodingTypeList(const mafMemento *memento) {
    QString key = memento->objectClassType();
    return m_EncodingHash.values(key);
}

void mafSerializationManager::shutdown() {
}
