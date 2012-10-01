/*
 *  mafSerializationRegistrations.cpp
 *  mafSerialization
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafSerializationRegistration.h"

#include "mafSerializationManager.h"

using namespace mafSerialization;

void mafSerializationRegistration::registerSerializationObjects() {
        mafRegisterObject(mafSerialization::mafCodecRawBinary);
        mafRegisterObject(mafSerialization::mafCodecRawASCII);
        mafRegisterObject(mafSerialization::mafCodecVolume);
        mafRegisterObject(mafSerialization::mafCodecBrickedVolume);
        mafRegisterObject(mafSerialization::mafCodecXML);
        mafRegisterObject(mafSerialization::mafSerializerFileSystem);

        //mafRegisterObject(mafSerialization::mafCSVReader);
}

void mafSerializationRegistration::unregisterSerializationObjects() {
        mafUnregisterObject(mafSerialization::mafCodecRawBinary);
        mafUnregisterObject(mafSerialization::mafCodecRawASCII);
        mafUnregisterObject(mafSerialization::mafCodecVolume);
        mafUnregisterObject(mafSerialization::mafCodecBrickedVolume);
        mafUnregisterObject(mafSerialization::mafCodecXML);
        mafUnregisterObject(mafSerialization::mafSerializerFileSystem);
}

void mafSerialization::initializeModule() {
    mafSerialization::mafSerializationRegistration::registerSerializationObjects();
    mafSerialization::mafSerializationManager::instance();
}

void mafSerialization::shutdownModule() {
    mafSerialization::mafSerializationRegistration::unregisterSerializationObjects();
    mafSerialization::mafSerializationManager::instance()->shutdown();
}
