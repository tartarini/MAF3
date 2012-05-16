/*
 *  mafExternalDataCodecVolumeTest.cpp
 *  mafSerializationTest
 *
 *  Created by Yubo Tao on 25/02/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafVMEManager.h>
#include <mafVolume.h>
#include <mafMementoVolume.h>
#include <mafExternalDataCodecVolume.h>
#include <mafResourcesRegistration.h>

#ifdef WIN32
    #define SERIALIZATION_LIBRARY_NAME "mafSerialization.dll"
#else
    #ifdef __APPLE__
        #define SERIALIZATION_LIBRARY_NAME "mafSerialization.dylib"
    #else
        #define SERIALIZATION_LIBRARY_NAME "mafSerialization.so"
    #endif
#endif

using namespace mafCore;
using namespace mafResources;
using namespace mafSerialization;
using namespace mafPluginOutOfCore;
using namespace mafEventBus;

/**
 Class name: mafExternalDataCodecVolumeTest
 This class implements the test suite for mafExternalCodecVolume.
 */

//! <title>
//mafExternalDataCodecVolumeTest
//! </title>
//! <description>
//mafExternalDataCodecVolumeTest is a codec to Encode/Decode mafVolume into a raw memory dump using multiresolution.
//! </description>

class mafExternalDataCodecVolumeTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafExternalDataCodecVolume allocation test case.
    void allocationTest();

    /// test the encode method using the Gray volume (unsigned short).
    void encodeGrayVolumeTest();
    
    /// test the decode method using the Gray volume (unsigned short).
    void decodeGrayVolumeTest();
    
    /// test the update method using the Gray volume (unsigned short).
    void updateGrayVolumeTest();

    /// test the encode method using the RGB volume (unsigned char).
    void encodeRGBVolumeTest();
    
    /// test the decode method using the RGB volume (unsigned char).
    void decodeRGBVolumeTest();
    
    /// test the update method using the RGB volume (unsigned char).
    void updateRGBVolumeTest();

    /// test the encode method using the Float volume (float).
    void encodeFloatVolumeTest();
    
    /// test the decode method using the Float volume (float).
    void decodeFloatVolumeTest();
    
    /// test the update method using the Float volume (float).
    void updateFloatVolumeTest();

private:
    /// initialize the Gray volume (unsigned short).
    void initGrayVolume();

    /// initialize the RGB volume (unsigned char).
    void initRGBVolume();

    /// initialize the Float volume (float).
    void initFloatVolume();

    /// encode the Float volume data.
    void encodeFloatVolume();

    /// verify the meta information of two volumes are equal.
    void isVolumeMetaDataEqual(mafVolume *volumeA, mafVolume *volumeB, int level, int startPos[3], int dimensions[3]);

    /// verify the volume data of the Gray volume.
    void verifyGrayVolumeData();

    /// verify the volume data of the RGB volume.
    void verifyRGBVolumeData();

    /// verify the volume data of the Float volume.
    void verifyFloatVolumeData();

private:
    mafVolume *m_GrayVolume;                    ///< Test var (Gray, unsigned short).
    mafVolume *m_DecodedGrayVolume;             ///< Test var, the decoded volume from m_GrayFileName.
    mafVolume *m_RGBVolume;                     ///< Test var (RGB, unsigned char).
    mafVolume *m_DecodedRGBVolume;              ///< Test var, the decoded volume from m_RGBFileName.
    mafVolume *m_FloatVolume;                   ///< Test var (float).
    mafVolume *m_DecodedFloatVolume;            ///< Test var, the decoded volume from m_FloatFileName.
    mafExternalDataCodecVolume *m_CodecVolume;  ///< Test var.
    QString m_GrayFileName;                     ///< The file name of m_GrayVolume.
    QString m_RGBFileName;                      ///< The file name of m_RGBVolume.
    QString m_FloatFileName;                    ///< The file name of m_FloatVolume.
    QLibrary *m_SerializationHandle;
};

void mafExternalDataCodecVolumeTest::initTestCase() {
    mafEventBusManager::instance();
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    
    // create before the instance of the Serialization manager, which will register signals.
    m_SerializationHandle = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
    QVERIFY(m_SerializationHandle != NULL);

    // register external codec
    QString encodeType = "VOLUME_LOD";
    QString codec = "mafSerialization::mafExternalDataCodecVolume";
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);

    // create mafExternalDataCodecVolume var
    m_CodecVolume = mafNEW(mafPluginOutOfCore::mafExternalDataCodecVolume);

    // initialize the volumes
    initGrayVolume();
    initRGBVolume();
    initFloatVolume();
}

void mafExternalDataCodecVolumeTest::cleanupTestCase() {
    QFile::remove(m_GrayFileName);
    QFile::remove("/" + m_GrayVolume->fileName());
    QFile::remove(m_RGBFileName);
    QFile::remove("/" + m_RGBVolume->fileName());
    QFile::remove(m_FloatFileName);
    QFile::remove("/" + m_FloatVolume->fileName());
    mafDEL(m_CodecVolume);
    mafDEL(m_GrayVolume);
    mafDEL(m_DecodedGrayVolume);
    mafDEL(m_RGBVolume);
    mafDEL(m_DecodedRGBVolume);
    mafDEL(m_FloatVolume);
    mafDEL(m_DecodedFloatVolume);
    mafShutdownModule(m_SerializationHandle);
    mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafExternalDataCodecVolumeTest::allocationTest() {
    QVERIFY(m_GrayVolume);
    QVERIFY(m_DecodedGrayVolume);
    QVERIFY(m_RGBVolume);
    QVERIFY(m_DecodedRGBVolume);
    QVERIFY(m_FloatVolume);
    QVERIFY(m_DecodedFloatVolume);
    QVERIFY(m_CodecVolume);
}

void mafExternalDataCodecVolumeTest::encodeGrayVolumeTest() {
    mafMementoVolume *memento = (mafMementoVolume*)m_GrayVolume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_GrayFileName));
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // meta data file
    QVERIFY(QFile::exists(m_GrayFileName));
    QFileInfo metaFileInfo(m_GrayFileName);
    QVERIFY(metaFileInfo.size() > 0);

    // volume data file
    QVERIFY(QFile::exists("/" + m_GrayVolume->fileName()));
    QFileInfo dataFileInfo("/" + m_GrayVolume->fileName());
    QVERIFY(dataFileInfo.size() > 0);

    mafDEL(memento);
}
    
void mafExternalDataCodecVolumeTest::decodeGrayVolumeTest() {
    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_GrayFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    // verify memento
    QVERIFY(memento);
    m_DecodedGrayVolume->setMemento(memento);
    mafDEL(memento);

    // verify the meta information
    int dimensions[3] = { 0, 0, 0 };
    isVolumeMetaDataEqual(m_DecodedGrayVolume, m_GrayVolume, m_GrayVolume->level(), m_GrayVolume->startPositions(), dimensions);
}

void mafExternalDataCodecVolumeTest::updateGrayVolumeTest() {
    int  startPos[3], dimensions[3];
    int *originalDims = m_GrayVolume->originalDimensions();

    // test case 1: lazy initialization
    m_DecodedGrayVolume->updateDataValue();
    
    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = 0;
        dimensions[i] = originalDims[i] >> 1;
    }
    isVolumeMetaDataEqual(m_DecodedGrayVolume, m_GrayVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyGrayVolumeData();

    // test case 2: update the start positions and dimensions, and extract volume data from the loaded volume data in level 2
    startPos[0] = 3;
    startPos[1] = 6;
    startPos[2] = 9;
    dimensions[0] = originalDims[0] - 20;
    dimensions[1] = originalDims[1] - 15;
    dimensions[2] = originalDims[2] - 10;

    m_DecodedGrayVolume->setUpdatedStartPos(startPos);
    m_DecodedGrayVolume->setUpdatedDimensions(dimensions);
    m_DecodedGrayVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    isVolumeMetaDataEqual(m_DecodedGrayVolume, m_GrayVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyGrayVolumeData();

    // test case 3: update the start positions only, and load the volume data from the disk in level 2
    startPos[0] = 4;
    startPos[1] = 7;
    startPos[2] = 9;
    m_DecodedGrayVolume->setUpdatedStartPos(startPos);
    m_DecodedGrayVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i)
        startPos[i] = startPos[i] >> 1;
    m_GrayVolume->setStartPositions(startPos);
    isVolumeMetaDataEqual(m_DecodedGrayVolume, m_GrayVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyGrayVolumeData();

    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1 
    startPos[0] = 10;
    startPos[1] = 15;
    startPos[2] = 20;
    dimensions[0] = originalDims[0] - 40;
    dimensions[1] = originalDims[1] - 35;
    dimensions[2] = originalDims[2] - 30;

    m_DecodedGrayVolume->setUpdatedStartPos(startPos);
    m_DecodedGrayVolume->setUpdatedDimensions(dimensions);
    m_DecodedGrayVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedGrayVolume, m_GrayVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyGrayVolumeData();

    // test case 5: update the dimensions only, and extract the volume data from the loaded volume data in level 1 
    dimensions[0] = originalDims[0] - 40;
    dimensions[1] = originalDims[1] - 30;
    dimensions[2] = originalDims[2] - 40;
    m_DecodedGrayVolume->setUpdatedDimensions(dimensions);
    m_DecodedGrayVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedGrayVolume, m_GrayVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyGrayVolumeData();
}

void mafExternalDataCodecVolumeTest::encodeRGBVolumeTest() {
    mafMementoVolume *memento = (mafMementoVolume*)m_RGBVolume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_RGBFileName));
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // meta data file
    QVERIFY(QFile::exists(m_RGBFileName));
    QFileInfo metaFileInfo(m_RGBFileName);
    QVERIFY(metaFileInfo.size() > 0);

    // volume data file
    QVERIFY(QFile::exists("/" + m_RGBVolume->fileName()));
    QFileInfo dataFileInfo("/" + m_RGBVolume->fileName());
    QVERIFY(dataFileInfo.size() > 0);

    mafDEL(memento);
}

void mafExternalDataCodecVolumeTest::decodeRGBVolumeTest() {
    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_RGBFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    // verify memento
    QVERIFY(memento);
    m_DecodedRGBVolume->setMemento(memento);
    mafDEL(memento);

    // verify the meta information
    int dimensions[3] = { 0, 0, 0 };
    isVolumeMetaDataEqual(m_DecodedRGBVolume, m_RGBVolume, m_RGBVolume->level(), m_RGBVolume->startPositions(), dimensions);
}

void mafExternalDataCodecVolumeTest::updateRGBVolumeTest() {
    int  startPos[3], dimensions[3];
    int *originalDims = m_RGBVolume->originalDimensions();

    // test case 1: lazy initialization
    m_DecodedRGBVolume->updateDataValue();
    
    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = 0;
        dimensions[i] = originalDims[i] >> 2;
    }
    isVolumeMetaDataEqual(m_DecodedRGBVolume, m_RGBVolume, 3, startPos, dimensions);
    // verify the volume data
    verifyRGBVolumeData();

    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
    startPos[0] = 3;
    startPos[1] = 6;
    startPos[2] = 9;
    dimensions[0] = originalDims[0] - 15;
    dimensions[1] = originalDims[1] - 13;
    dimensions[2] = originalDims[2] - 11;

    m_DecodedRGBVolume->setUpdatedStartPos(startPos);
    m_DecodedRGBVolume->setUpdatedDimensions(dimensions);
    m_DecodedRGBVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    isVolumeMetaDataEqual(m_DecodedRGBVolume, m_RGBVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyRGBVolumeData();

    // test case 3: update the start positions only, and load the volume data from the disk in level 2
    startPos[0] = 4;
    startPos[1] = 7;
    startPos[2] = 9;
    m_DecodedRGBVolume->setUpdatedStartPos(startPos);
    m_DecodedRGBVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i)
        startPos[i] = startPos[i] >> 1;
    isVolumeMetaDataEqual(m_DecodedRGBVolume, m_RGBVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyRGBVolumeData();

    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1 
    startPos[0] = 10;
    startPos[1] = 15;
    startPos[2] = 20;
    dimensions[0] = originalDims[0] - 40;
    dimensions[1] = originalDims[1] - 35;
    dimensions[2] = originalDims[2] - 35;

    m_DecodedRGBVolume->setUpdatedStartPos(startPos);
    m_DecodedRGBVolume->setUpdatedDimensions(dimensions);
    m_DecodedRGBVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedRGBVolume, m_RGBVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyRGBVolumeData();

    // test case 5: update the dimensions only, and extract the volume data from the loaded volume data in level 1
    dimensions[0] = originalDims[0] - 40;
    dimensions[1] = originalDims[1] - 38;
    dimensions[2] = originalDims[2] - 35;
    m_DecodedRGBVolume->setUpdatedDimensions(dimensions);
    m_DecodedRGBVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedRGBVolume, m_RGBVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyRGBVolumeData();
}

void mafExternalDataCodecVolumeTest::encodeFloatVolumeTest() {
    mafMementoVolume *memento = (mafMementoVolume*)m_FloatVolume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_FloatFileName));
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // automatic generate the volume data file
    encodeFloatVolume();

    // meta data file
    QVERIFY(QFile::exists(m_FloatFileName));
    QFileInfo metaFileInfo(m_FloatFileName);
    QVERIFY(metaFileInfo.size() > 0);

    // volume data file
    QVERIFY(QFile::exists("/" + m_FloatVolume->fileName()));
    QFileInfo dataFileInfo("/" + m_FloatVolume->fileName());
    QVERIFY(dataFileInfo.size() > 0);

    mafDEL(memento);
}

void mafExternalDataCodecVolumeTest::decodeFloatVolumeTest() {
    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_FloatFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    // verify memento
    QVERIFY(memento);
    m_DecodedFloatVolume->setMemento(memento);
    mafDEL(memento);

    // verify the meta information
    int dimensions[3] = { 0, 0, 0 };
    isVolumeMetaDataEqual(m_DecodedFloatVolume, m_FloatVolume, m_FloatVolume->level(), m_FloatVolume->startPositions(), dimensions);
}

void mafExternalDataCodecVolumeTest::updateFloatVolumeTest() {
    //if (!QFile::exists(m_DecodedFloatVolume->fileName())) // no data
    //    return;

    int  startPos[3], dimensions[3];
    int *originalDims = m_FloatVolume->originalDimensions();

    // test case 1: lazy initialization
    m_DecodedFloatVolume->updateDataValue();
    
    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = 0;
        dimensions[i] = originalDims[i] >> 3;
    }
    isVolumeMetaDataEqual(m_DecodedFloatVolume, m_FloatVolume, 4, startPos, dimensions);
    // verify the volume data
    verifyFloatVolumeData();

    // update the memory limit in the float memento for test case 2 - 4
    m_DecodedFloatVolume->setMemoryLimit(512 * 1024);

    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
    startPos[0] = 125;
    startPos[1] = 250;
    startPos[2] = 375;
    dimensions[0] = originalDims[0] - 125;
    dimensions[1] = originalDims[1] - 250;
    dimensions[2] = originalDims[2] - 375;

    m_DecodedFloatVolume->setUpdatedStartPos(startPos);
    m_DecodedFloatVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    isVolumeMetaDataEqual(m_DecodedFloatVolume, m_FloatVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyFloatVolumeData();

    // test case 3: update the dimensions only, and extract the volume data from the loaded volume data in level 2
    dimensions[0] = originalDims[0] - 375;
    dimensions[1] = originalDims[1] - 500;
    dimensions[2] = originalDims[2] - 625;
    m_DecodedFloatVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i)
        dimensions[i] = dimensions[i] >> 1;
    isVolumeMetaDataEqual(m_DecodedFloatVolume, m_FloatVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyFloatVolumeData();

    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1
    startPos[0] = 250;
    startPos[1] = 375;
    startPos[2] = 500;
    dimensions[0] = originalDims[0] - 400;
    dimensions[1] = originalDims[1] - 525;
    dimensions[2] = originalDims[2] - 650;

    m_DecodedFloatVolume->setUpdatedStartPos(startPos);
    m_DecodedFloatVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedFloatVolume, m_FloatVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyFloatVolumeData();
}

void mafExternalDataCodecVolumeTest::initGrayVolume() {
    // create the Gray volume (unsigned short)
    m_GrayVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedGrayVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    m_GrayFileName      = "/Gray716458.lod";   // Meta-data file name

    int dimensions[3] = { 71, 64, 58 };
    float spacing[3]  = { 0.1f, 0.11f, 0.13f };
    m_GrayVolume->setDataType(mafPluginOutOfCore::mafVolUnsignedShort);
    m_GrayVolume->setBitsStored(12);
    m_GrayVolume->setBigEndian(false);
    m_GrayVolume->setComponentNum(1);
    m_GrayVolume->setOriginalDimensions(dimensions);
    m_GrayVolume->setSpacing(spacing);
    m_GrayVolume->setDimensions(dimensions);
    m_GrayVolume->setLevelNum(2);                   // level number for 64k memory limit
    m_GrayVolume->setMemoryLimit(64);               // 64k for test, 1M for default
    m_DecodedGrayVolume->setMemoryLimit(64);
    m_GrayVolume->setFileName("Gary716458.raw");    // External data file name
    // dataValue
    ushort *data = new ushort[dimensions[2] * dimensions[1] * dimensions[0]];
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                int index = (z * dimensions[1] + y ) * dimensions[0] + x;
                data[index] = index % 4096;
            }
        }
    }
    m_GrayVolume->setDataValue(data);
}

void mafExternalDataCodecVolumeTest::initRGBVolume() {
    // create the RGB volume (unsigned char)
    m_RGBVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedRGBVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    m_RGBFileName      = "/RGB716458.lod";   // Meta-data file name

    int dimensions[3] = { 71, 64, 58 };
    float spacing[3]  = { 0.1f, 0.11f, 0.13f };
    m_RGBVolume->setDataType(mafPluginOutOfCore::mafVolUnsignedChar);
    m_RGBVolume->setBitsStored(8);
    m_RGBVolume->setBigEndian(false);
    m_RGBVolume->setComponentNum(3);
    m_RGBVolume->setOriginalDimensions(dimensions);
    m_RGBVolume->setSpacing(spacing);
    m_RGBVolume->setDimensions(dimensions);
    m_RGBVolume->setLevelNum(3);                // level number for 64k memory limit
    m_RGBVolume->setMemoryLimit(64);            // 64k for test, 1M for default
    m_DecodedRGBVolume->setMemoryLimit(64);
    m_RGBVolume->setFileName("RGB716458.raw");  // External data file name
    // dataValue
    unsigned char *data = new unsigned char[dimensions[0] * dimensions[1] * dimensions[2] * 3];
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                int index = ((z * dimensions[1] + y ) * dimensions[0] + x) * 3;
                data[index + 0] = z;
                data[index + 1] = y;
                data[index + 2] = x;
            }
        }
    }
    m_RGBVolume->setDataValue(data);
}

void mafExternalDataCodecVolumeTest::initFloatVolume() {
    // create the Float volume (float)
    m_FloatVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedFloatVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    m_FloatFileName      = "/Float1024.lod";   // Meta-data file name

    int dimensions[3] = { 1024, 1024, 1024 };
    float spacing[3]  = { 0.1f, 0.1f, 0.1f };
    m_FloatVolume->setDataType(mafPluginOutOfCore::mafVolFloat);
    m_FloatVolume->setBitsStored(32);
    m_FloatVolume->setBigEndian(false);
    m_FloatVolume->setComponentNum(1);
    m_FloatVolume->setOriginalDimensions(dimensions);
    m_FloatVolume->setSpacing(spacing);
    m_FloatVolume->setDimensions(dimensions);
    m_FloatVolume->setLevelNum(4);                      // level number for 16M memory limit
    m_FloatVolume->setMemoryLimit(16 * 1024);           // 16M
    m_DecodedFloatVolume->setMemoryLimit(16 * 1024); 
    m_FloatVolume->setFileName("float1024x1024x1024.raw");    // External data file name
    // no dataValue, use encodeFloatVolume to save the volume data directly
}

void mafExternalDataCodecVolumeTest::encodeFloatVolume()
{
    QString url = "/" + m_FloatVolume->fileName();
    QFile file(url);

    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << ("Not able to open file " + url);
        return;
    }

    int scale = 1;
    int levels = m_FloatVolume->levelNum();
    int *dimensions = m_FloatVolume->dimensions();
    float *data = new float[dimensions[0] * dimensions[1]];
    
    for (int level = 0; level < levels; ++level) {
        int xDim = dimensions[0] / scale;
        int yDim = dimensions[1] / scale;
        int zDim = dimensions[2] / scale;
        for (int z = 0; z < zDim; ++z) {
            for (int y = 0; y < yDim; ++y) {
                for (int x = 0; x < xDim; ++x) {
                    int index = ((z * dimensions[1] + y) * dimensions[0] + x) * scale;
                    data[y * xDim + x] = index / 100.f;
                }
            }
            file.write((const char*)data, sizeof(float) * xDim * yDim);
        }
        scale *= 2;
    }
    
    delete []data;
    file.close();
}

void mafExternalDataCodecVolumeTest::isVolumeMetaDataEqual(mafVolume *volumeA, mafVolume *volumeB, int level, int startPos[3], int dimensions[3]) {
    QVERIFY(volumeA->dataType()     == volumeB->dataType());
    QVERIFY(volumeA->bitsStored()   == volumeB->bitsStored());
    QVERIFY(volumeA->bigEndian()    == volumeB->bigEndian());
    QVERIFY(volumeA->componentNum() == volumeB->componentNum());
    for (int i = 0; i < 3; ++i) {
        QVERIFY(volumeA->originalDimensions()[i] == volumeB->originalDimensions()[i]);
        QVERIFY(volumeA->spacing()[i]            == volumeB->spacing()[i]);
        QVERIFY(volumeA->startPositions()[i]     == startPos[i]);
        QVERIFY(volumeA->dimensions()[i]         == dimensions[i]);
    }
    QVERIFY(volumeA->poseMatrix()->isEqual(*(volumeB->poseMatrix())));
    QVERIFY(volumeA->level()     == level);
    QVERIFY(volumeA->codecType() == volumeB->codecType());
    QVERIFY(volumeA->levelNum()  == volumeB->levelNum());
    //QVERIFY(volumeA->fileName() == volumeB->fileName());
}

void mafExternalDataCodecVolumeTest::verifyGrayVolumeData() {
    int   level         = m_DecodedGrayVolume->level();
    int   scale         = pow(2.f, level - 1.f);
    int   *startPos     = m_DecodedGrayVolume->startPositions();
    int   *dimensions   = m_DecodedGrayVolume->dimensions();
    int   *originalDims = m_DecodedGrayVolume->originalDimensions();
    ushort *readData     = (ushort *)m_DecodedGrayVolume->dataValue();
    ushort *originalData = (ushort *)m_GrayVolume->dataValue();

    bool dataEqual = true;
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                int comparedIndex = (((startPos[2] + z) * originalDims[1] + startPos[1] + y) * originalDims[0] + startPos[0] + x) * scale;
                dataEqual &= (readData[readIndex] == originalData[comparedIndex]);
            }
        }
    }
    QVERIFY(dataEqual);
}

void mafExternalDataCodecVolumeTest::verifyRGBVolumeData() {
    int  level         = m_DecodedRGBVolume->level();
    int  scale         = pow(2.f, level - 1.f);
    int  *startPos     = m_DecodedRGBVolume->startPositions();
    int  *dimensions   = m_DecodedRGBVolume->dimensions();
    int  *originalDims = m_DecodedRGBVolume->originalDimensions();
    uchar *readData     = (uchar *)m_DecodedRGBVolume->dataValue();
    uchar *originalData = (uchar *)m_RGBVolume->dataValue();

    bool dataEqual = true;
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                int readIndex     = ((z * dimensions[1] + y) * dimensions[0] + x) * 3;
                int comparedIndex = (((startPos[2] + z) * originalDims[1] + startPos[1] + y) * originalDims[0] + startPos[0] + x) * scale * 3;
                dataEqual &= (readData[readIndex + 0] == originalData[comparedIndex + 0]);
                dataEqual &= (readData[readIndex + 1] == originalData[comparedIndex + 1]);
                dataEqual &= (readData[readIndex + 2] == originalData[comparedIndex + 2]);
            }
        }
    }
    QVERIFY(dataEqual);
}

void mafExternalDataCodecVolumeTest::verifyFloatVolumeData() {
    int   level         = m_DecodedFloatVolume->level();
    int   scale         = pow(2.f, level - 1.f);
    int   *startPos     = m_DecodedFloatVolume->startPositions();
    int   *dimensions   = m_DecodedFloatVolume->dimensions();
    int   *originalDims = m_DecodedFloatVolume->originalDimensions();
    float *readData     = (float *)m_DecodedFloatVolume->dataValue();
    
    int  readIndex = 0;
    bool dataEqual = true;
    for (int z = 0; z < dimensions[2]; ++z) {
        int comparedIndex = (((startPos[2] + z) * originalDims[1] + startPos[1]) * originalDims[0] + startPos[0]) * scale;
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                //int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                //int comparedIndex = (((startPos[2] + z) * originalDims[1] + startPos[1] + y) * originalDims[0] + startPos[0] + x) * scale;
                float value         = readData[readIndex++];
                float comparedValue = comparedIndex * 0.01;
                dataEqual &= (value == comparedValue);
                comparedIndex += scale;
             }
             comparedIndex += (originalDims[0] - dimensions[0]) * scale;
         }
    }
    QVERIFY(dataEqual);
}

MAF_REGISTER_TEST(mafExternalDataCodecVolumeTest);
#include "mafExternalDataCodecVolumeTest.moc"
