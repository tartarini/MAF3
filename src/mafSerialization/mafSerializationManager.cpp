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
#include <mafEventBusManager.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafSerialization;

mafSerializationManager* mafSerializationManager::instance() {
    static mafSerializationManager instanceSerializationManager;
    return &instanceSerializationManager;
}

mafSerializationManager::mafSerializationManager(const QString code_location) : mafObjectBase(code_location), m_CurrentExternalDataCodec(NULL) {
    mafSerializationRegistration::registerSerializationObjects();

    initializeConnections();

    plugCodec("XML","mafSerialization::mafCodecXML");
    plugCodec("RAW","mafSerialization::mafCodecRawBinary");
    plugCodec("VOLUME_LOD","mafSerialization::mafCodecRawVolume");
    plugCodec("VOLUME_BRICKED_LOD","mafSerialization::mafCodecBrickedVolume");
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

    // Register API signals.
    mafRegisterLocalSignal("maf.local.serialization.plugCodec", this, "plugCodecInModule(const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.plugSerializer", this, "plugSerializerInModule(const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.save", this, "save(mafCore::mafMemento *, const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.load", this, "load(const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.export", this, "exportData(mafCore::mafProxyInterface *, const QString &, const QString &)");
    mafRegisterLocalSignal("maf.local.serialization.import", this, "importData(const QString &, const QString &)");

    // Register private callbacks.
    mafRegisterLocalCallback("maf.local.serialization.plugCodec", this, "plugCodec(const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.plugSerializer", this, "plugSerializer(const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.save", this, "saveMemento(mafCore::mafMemento *, const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.load", this, "loadMemento(const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.export", this, "exportExternalData(mafCore::mafProxyInterface *, const QString &, const QString &)");
    mafRegisterLocalCallback("maf.local.serialization.import", this, "importExternalData(const QString &, const QString &)");
}

void mafSerializationManager::saveMemento(mafMemento *memento, const QString &url, const QString &encode_type) {
    REQUIRE(memento != NULL);

    QString codecType;
    if(m_CodecHash.contains(encode_type)) {
        codecType = m_CodecHash.value(encode_type);
    } else {
        qWarning("%s", mafTr("Codec not found for %1 type; will be used the RAW default.").arg(encode_type).toAscii().data());
        codecType = m_CodecHash.value("RAW");
    }
    
    // Pass the memento to the codec and ask him to encode the given data.
    mafCodec *codec = (mafCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    QUrl u(url);
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

mafMemento *mafSerializationManager::loadMemento(const QString &url, const QString &encode_type) {
    if(!m_CodecHash.contains(encode_type)) {
        qCritical("%s", mafTr("No codec associated with the '%1' encoding type.").arg(encode_type).toAscii().data());
        return NULL;
    }

    QString codecType = m_CodecHash[encode_type];
    mafCodec *codec = (mafCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    QUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
        return NULL;
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

    if(m == NULL) {
        qCritical("%s", mafTr("Impossible load memento.").toAscii().data());
        mafDEL(codec);
        mafDEL(ser);
        return NULL;
    }
    
    // Finally close the connection.
    ser->closeDevice();

    mafDEL(codec);
    mafDEL(ser);
    return  m;
}

void mafSerializationManager::exportExternalData(mafCore::mafProxyInterface *externalData, const QString &url, const QString &encode_type) {
    REQUIRE(externalData != NULL);

    QString codecType;
    if(m_CodecHash.contains(encode_type)) {
        codecType = m_CodecHash.value(encode_type);
    } else {
        qWarning("%s", mafTr("Codec not found for %1 type; will be used the RAW default.").arg(encode_type).toAscii().data());
        codecType = m_CodecHash.value("RAW");
    }


    // Pass the memento to the codec and ask him to encode the given data.
    mafExternalDataCodec *codec = (mafExternalDataCodec *)mafNEWFromString(codecType);

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    QUrl u(url);
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

mafCore::mafProxyInterface * mafSerializationManager::importExternalData(const QString &url, const QString &encode_type) {
    if(!m_CodecHash.contains(encode_type)) {
        qCritical("%s", mafTr("No codec associated with the '%1' encoding type.").arg(encode_type).toAscii().data());
        return NULL;
    }

    // Check the protocol of the given 'url' and create the corresponding mafSerialize class
    QUrl u(url);
    u =  QUrl::fromUserInput(url);
    if (!u.isValid()) {
        qCritical("%s", mafTr("Invalid URL: %1").arg(u.toString()).toAscii().data());
        return NULL;
    }

    QString s = u.scheme();
    QString serializer_type = m_SerializerHash[s];

    qDebug() << u;
    
    // Create the instance of correct serializer
    mafSerializer *ser = (mafSerializer *)mafNEWFromString(serializer_type);
    // Give it the data URL
    ser->setDataURL(u.toString());
    // ... and open the input connection.
    ser->openDevice(mafSerializerOpenModeIn);

    unsigned int size;
    char * inputString;
    QDataStream stream(ser->ioDevice());
    stream.readBytes(inputString, size);

    // Finally close the connection.
    ser->closeDevice();
    mafDEL(ser);

    QString codecType = m_CodecHash[encode_type];
    mafDEL(m_CurrentExternalDataCodec);
    m_CurrentExternalDataCodec = (mafExternalDataCodec *)mafNEWFromString(codecType);
    
    m_CurrentExternalDataCodec->setStringSize(size);
    m_CurrentExternalDataCodec->decode(inputString);

    return m_CurrentExternalDataCodec->externalData();
}

void mafSerializationManager::plugCodec(const QString &encoding_type, const QString &codecType) {
    m_CodecHash.insertMulti(encoding_type, codecType);
}

void mafSerializationManager::shutdown() {
    mafDEL(m_CurrentExternalDataCodec);
}
