/*
 *  mafExternalDataCodecBrickedVolumeTest.cpp
 *  mafSerializationTest
 *
 *  Created by Yubo Tao on 27/02/12.
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
#include <mafExternalDataCodecBrickedVolume.h>
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
 Class name: mafExternalDataCodecBrickedVolumeTest
 This class implements the test suite for mafExternalCodecBrickedVolume.
 */

//! <title>
//mafExternalDataCodecBrickedVolumeTest
//! </title>
//! <description>
//mafExternalDataCodecBrickedVolumeTest is a codec to Encode/Decode mafVolume into a raw bricked memory dump using multiresolution.
//! </description>

class mafExternalDataCodecBrickedVolumeTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafExternalDataCodecBrickedVolume allocation test case.
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

    /// test the encode method using the Float Equal volume (float).
    void encodeFloatEqualVolumeTest();
    
    /// test the decode method using the Float Equal volume (float).
    void decodeFloatEqualVolumeTest();
    
    /// test the update method using the Float Equal volume (float).
    void updateFloatEqualVolumeTest();

    /// test the encode method using the Float Large volume (float).
    void encodeFloatLargeVolumeTest();

    /// test the decode method using the Float Large volume (float).
    void decodeFloatLargeVolumeTest();

    /// test the update method using the Float Large volume (float).
    void updateFloatLargeVolumeTest();

private:
    /// initialize the Gray volume (unsigned short).
    void initGrayVolume();

    /// initialize the RGB volume (unsigned char).
    void initRGBVolume();

    /// initialize the Float Equal volume (float).
    void initFloatEqualVolume();

    /// initialize the Float Large volume (float).
    void initFloatLargeVolume();

    /// encode the Float Large volume data.
    void encodeFloatLargeVolume();

    /// verify the meta information of two volumes are equal.
    void isVolumeMetaDataEqual(mafVolume *volumeA, mafVolume *volumeB, int level, int startPos[3], int dimensions[3]);

    /// verify the volume data of the Gray volume.
    void verifyGrayVolumeData();

    /// verify the volume data of the RGB volume.
    void verifyRGBVolumeData();

    /// verify the volume data of the Float Equal volume.
    void verifyFloatEqualVolumeData();

    /// verify the volume data of the Float Large volume.
    void verifyFloatLargeVolumeData();

private:
    mafVolume *m_GrayVolume;                            ///< Test var (Gray, unsigned short).
    mafVolume *m_DecodedGrayVolume;                     ///< Test var, the decoded volume from m_GrayFileName.
    mafVolume *m_RGBVolume;                             ///< Test var (RGB, unsigned char).
    mafVolume *m_DecodedRGBVolume;                      ///< Test var, the decoded volume from m_RGBFileName.
    mafVolume *m_FloatEqualVolume;                      ///< Test var (Equal, float).
    mafVolume *m_DecodedFloatEqualVolume;               ///< Test var, the decoded volume from m_FloatEqualFileName.
    mafVolume *m_FloatLargeVolume;                      ///< Test var (Large, float).
    mafVolume *m_DecodedFloatLargeVolume;               ///< Test var, the decoded volume from m_FloatLargeFileName.
    mafExternalDataCodecBrickedVolume *m_CodecVolume;   ///< Test var.
    QString m_GrayFileName;                             ///< The file name of m_GrayVolume.
    QString m_RGBFileName;                              ///< The file name of m_RGBVolume.
    QString m_FloatEqualFileName;                       ///< The file name of m_FloatEqualVolume.
    QString m_FloatLargeFileName;                       ///< The file name of m_FloatLargeVolume.
    QLibrary *m_SerializationHandle;
};

void mafExternalDataCodecBrickedVolumeTest::initTestCase() {
    mafEventBusManager::instance();
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();

    // create before the instance of the Serialization manager, which will register signals.
    m_SerializationHandle = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
    QVERIFY(m_SerializationHandle != NULL);
    
    mafRegisterObject(mafPluginOutOfCore::mafExternalDataCodecBrickedVolume);

    // register external codec
    QString encodeType = "VOLUME_BRICKED_LOD";
    QString codec = "mafPluginOutOfCore::mafExternalDataCodecBrickedVolume";
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);

    // create mafExternalDataCodecVolume var
    m_CodecVolume = mafNEW(mafPluginOutOfCore::mafExternalDataCodecBrickedVolume);

    // initialize the volumes
    initGrayVolume();
    initRGBVolume();
    initFloatEqualVolume();
    initFloatLargeVolume();
}

void mafExternalDataCodecBrickedVolumeTest::cleanupTestCase() {
    QFile::remove(m_GrayFileName);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    QString tmp1 = tmp;
    tmp1.append("/" + m_GrayVolume->fileName());
    
    QFile::remove(tmp);
    QFile::remove(tmp + ".bpi");
    QFile::remove(tmp + ".bev");
    QFile::remove(tmp + ".bv");
    
    QFile::remove(m_RGBFileName);
    
    QString tmp2 = tmp;
    tmp2.append("/" + m_RGBVolume->fileName());
    
    QFile::remove(tmp2);
    QFile::remove(tmp2 + ".bpi");
    QFile::remove(tmp2 + ".bev");
    QFile::remove(tmp2 + ".bv");
    
    QFile::remove(m_FloatEqualFileName);
    
    QString tmp3 = tmp;
    tmp3.append("/" + m_FloatEqualVolume->fileName());
    
    QFile::remove(tmp3);
    QFile::remove(tmp3 + ".bpi");
    QFile::remove(tmp3 + ".bev");
    QFile::remove(tmp3 + ".bv");
    
    QFile::remove(m_FloatLargeFileName);
    
    QString tmp4 = tmp;
    tmp4.append("/" + m_FloatLargeVolume->fileName());
    
    QFile::remove(tmp4);
    QFile::remove(tmp4 + ".bpi");
    QFile::remove(tmp4 + ".bev");
    QFile::remove(tmp4 + ".bv");
    
    mafDEL(m_CodecVolume);
    mafDEL(m_GrayVolume);
    mafDEL(m_DecodedGrayVolume);
    mafDEL(m_RGBVolume);
    mafDEL(m_DecodedRGBVolume);
    mafDEL(m_FloatEqualVolume);
    mafDEL(m_DecodedFloatEqualVolume);
    mafDEL(m_FloatLargeVolume);
    mafDEL(m_DecodedFloatLargeVolume);
    mafShutdownModule(m_SerializationHandle);
    mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafExternalDataCodecBrickedVolumeTest::allocationTest() {
    QVERIFY(m_GrayVolume);
    QVERIFY(m_DecodedGrayVolume);
    QVERIFY(m_RGBVolume);
    QVERIFY(m_DecodedRGBVolume);
    QVERIFY(m_FloatEqualVolume);
    QVERIFY(m_DecodedFloatEqualVolume);
    QVERIFY(m_FloatLargeVolume);
    QVERIFY(m_DecodedFloatLargeVolume);
    QVERIFY(m_CodecVolume);
}

void mafExternalDataCodecBrickedVolumeTest::encodeGrayVolumeTest() {
    mafMementoVolume *memento = (mafMementoVolume*)m_GrayVolume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_GrayFileName));
    qDebug() << m_GrayFileName;
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // meta data file
    QVERIFY(QFile::exists(m_GrayFileName));
    QFileInfo metaFileInfo(m_GrayFileName);
    QVERIFY(metaFileInfo.size() > 0);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/" + m_GrayVolume->fileName());
    
    // volume data bpi file
    QVERIFY(QFile::exists(tmp + ".bpi"));
    QFileInfo dataBPIFileInfo(tmp + ".bpi");
    QVERIFY(dataBPIFileInfo.size() > 0);

    // volume data bev file
    QVERIFY(QFile::exists(tmp + ".bev"));
    QFileInfo dataBEVFileInfo(tmp + ".bev");

    // volume data bv file
    QVERIFY(QFile::exists(tmp + ".bv"));
    QFileInfo dataBVFileInfo(tmp + ".bv");
    QVERIFY(dataBVFileInfo.size() > 0);

    mafDEL(memento);
}
    
void mafExternalDataCodecBrickedVolumeTest::decodeGrayVolumeTest() {
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

void mafExternalDataCodecBrickedVolumeTest::updateGrayVolumeTest() {
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

void mafExternalDataCodecBrickedVolumeTest::encodeRGBVolumeTest() {
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
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/" + m_RGBVolume->fileName());

    // volume data bpi file
    QVERIFY(QFile::exists(tmp + ".bpi"));
    QFileInfo dataBPIFileInfo(tmp + ".bpi");
    QVERIFY(dataBPIFileInfo.size() > 0);

    // volume data bev file
    QVERIFY(QFile::exists(tmp + ".bev"));
    QFileInfo dataBEVFileInfo(tmp + ".bev");

    // volume data bv file
    QVERIFY(QFile::exists(tmp + ".bv"));
    QFileInfo dataBVFileInfo(tmp + ".bv");
    QVERIFY(dataBVFileInfo.size() > 0);

    mafDEL(memento);
}

void mafExternalDataCodecBrickedVolumeTest::decodeRGBVolumeTest() {
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

void mafExternalDataCodecBrickedVolumeTest::updateRGBVolumeTest() {
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

void mafExternalDataCodecBrickedVolumeTest::encodeFloatEqualVolumeTest() {
    mafMementoVolume *memento = (mafMementoVolume*)m_FloatEqualVolume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_FloatEqualFileName));
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // meta data file
    QVERIFY(QFile::exists(m_FloatEqualFileName));
    QFileInfo metaFileInfo(m_FloatEqualFileName);
    QVERIFY(metaFileInfo.size() > 0);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/" + m_FloatEqualVolume->fileName());

    // volume data bpi file
    QVERIFY(QFile::exists(tmp + ".bpi"));
    QFileInfo dataBPIFileInfo(tmp + ".bpi");
    QVERIFY(dataBPIFileInfo.size() > 0);

    // volume data bev file
    QVERIFY(QFile::exists(tmp + ".bev"));
    QFileInfo dataBEVFileInfo(tmp + ".bev");
    QVERIFY(dataBEVFileInfo.size() > 0);

    // volume data bv file
    QVERIFY(QFile::exists(tmp + ".bv"));
    QFileInfo dataBVFileInfo(tmp + ".bv");
    QVERIFY(dataBVFileInfo.size() > 0);

    mafDEL(memento);
}

void mafExternalDataCodecBrickedVolumeTest::decodeFloatEqualVolumeTest() {
    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_FloatEqualFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    // verify memento
    QVERIFY(memento);
    m_DecodedFloatEqualVolume->setMemento(memento);
    mafDEL(memento);

    // verify the meta information
    int dimensions[3] = { 0, 0, 0 };
    isVolumeMetaDataEqual(m_DecodedFloatEqualVolume, m_FloatEqualVolume, m_FloatEqualVolume->level(), m_FloatEqualVolume->startPositions(), dimensions);
}

void mafExternalDataCodecBrickedVolumeTest::updateFloatEqualVolumeTest() {
    int  startPos[3], dimensions[3];
    int *originalDims = m_FloatEqualVolume->originalDimensions();

    // test case 1: lazy initialization
    m_DecodedFloatEqualVolume->updateDataValue();
    
    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = 0;
        dimensions[i] = originalDims[i] >> 2;
    }
    isVolumeMetaDataEqual(m_DecodedFloatEqualVolume, m_FloatEqualVolume, 3, startPos, dimensions);
    // verify the volume data
    verifyFloatEqualVolumeData();

    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
    startPos[0] = 3;
    startPos[1] = 6;
    startPos[2] = 9;
    dimensions[0] = originalDims[0] - 18;
    dimensions[1] = originalDims[1] - 15;
    dimensions[2] = originalDims[2] - 12;

    m_DecodedFloatEqualVolume->setUpdatedStartPos(startPos);
    m_DecodedFloatEqualVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatEqualVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    isVolumeMetaDataEqual(m_DecodedFloatEqualVolume, m_FloatEqualVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyFloatEqualVolumeData();

    // test case 3: update the dimensions only, and load the volume data from the disk in level 1
    startPos[0] = 1;
    startPos[1] = 3;
    startPos[2] = 4;
    dimensions[0] = originalDims[0] - 40;
    dimensions[1] = originalDims[1] - 39;
    dimensions[2] = originalDims[2] - 38;

    m_DecodedFloatEqualVolume->setUpdatedStartPos(startPos);
    m_DecodedFloatEqualVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatEqualVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedFloatEqualVolume, m_FloatEqualVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyFloatEqualVolumeData();
}

void mafExternalDataCodecBrickedVolumeTest::encodeFloatLargeVolumeTest() {
    mafMementoVolume *memento = (mafMementoVolume*)m_FloatLargeVolume->createMemento();
    QVERIFY(memento);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafMemento *, memento));
    argList.append(mafEventArgument(QString, m_FloatLargeFileName));
    argList.append(mafEventArgument(QString, memento->encodeType()));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal, &argList);

    // automatic generate the volume data file
    encodeFloatLargeVolume();

    // meta data file
    QVERIFY(QFile::exists(m_FloatLargeFileName));
    QFileInfo metaFileInfo(m_FloatLargeFileName);
    QVERIFY(metaFileInfo.size() > 0);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/" + m_FloatLargeVolume->fileName());

    // volume data bpi file
    QVERIFY(QFile::exists(tmp + ".bpi"));
    QFileInfo dataBPIFileInfo(tmp + ".bpi");
    QVERIFY(dataBPIFileInfo.size() > 0);

    // volume data bev file
    QVERIFY(QFile::exists(tmp + ".bev"));
    QFileInfo dataBEVFileInfo(tmp + ".bev");

    // volume data bv file
    QVERIFY(QFile::exists(tmp + ".bv"));
    QFileInfo dataBVFileInfo(tmp + ".bv");
    QVERIFY(dataBVFileInfo.size() > 0);

    mafDEL(memento);
}

void mafExternalDataCodecBrickedVolumeTest::decodeFloatLargeVolumeTest() {
    QString encodeType = "RAW";
    mafCore::mafMemento *memento = 0;

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_FloatLargeFileName));
    argList.append(mafEventArgument(QString, encodeType));
    QGenericReturnArgument retVal = mafEventReturnArgument(mafCore::mafMemento*, memento);
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal, &argList, &retVal);

    // verify memento
    QVERIFY(memento);
    m_DecodedFloatLargeVolume->setMemento(memento);
    mafDEL(memento);

    // verify the meta information
    int dimensions[3] = { 0, 0, 0 };
    isVolumeMetaDataEqual(m_DecodedFloatLargeVolume, m_FloatLargeVolume, m_FloatLargeVolume->level(), m_FloatLargeVolume->startPositions(), dimensions);
}

void mafExternalDataCodecBrickedVolumeTest::updateFloatLargeVolumeTest() {
    //if (!QFile::exists(m_DecodedFloatLargeVolume->fileName() + ".bpi") || 
    //    !QFile::exists(m_DecodedFloatLargeVolume->fileName() + ".bev") || 
    //    !QFile::exists(m_DecodedFloatLargeVolume->fileName() + ".bv")) // no data
    //    return;

    int  startPos[3], dimensions[3];
    int *originalDims = m_FloatLargeVolume->originalDimensions();

    // test case 1: lazy initialization
    m_DecodedFloatLargeVolume->updateDataValue();
    
    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = 0;
        dimensions[i] = originalDims[i] >> 3;
    }
    isVolumeMetaDataEqual(m_DecodedFloatLargeVolume, m_FloatLargeVolume, 4, startPos, dimensions);
    // verify the volume data
    verifyFloatLargeVolumeData();

    // update the memory limit in the float memento for test case 2 - 4
    m_DecodedFloatLargeVolume->setMemoryLimit(512 * 1024);

    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
    startPos[0] = 125;
    startPos[1] = 250;
    startPos[2] = 375;
    dimensions[0] = originalDims[0] - 125;
    dimensions[1] = originalDims[1] - 250;
    dimensions[2] = originalDims[2] - 375;

    m_DecodedFloatLargeVolume->setUpdatedStartPos(startPos);
    m_DecodedFloatLargeVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatLargeVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    isVolumeMetaDataEqual(m_DecodedFloatLargeVolume, m_FloatLargeVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyFloatLargeVolumeData();

    // test case 3: update the dimensions only, and extract the volume data from the loaded volume data in level 2
    dimensions[0] = originalDims[0] - 375;
    dimensions[1] = originalDims[1] - 500;
    dimensions[2] = originalDims[2] - 625;
    m_DecodedFloatLargeVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatLargeVolume->updateDataValue();

    // verify the meta information
    for (int i = 0; i < 3; ++i)
        dimensions[i] = dimensions[i] >> 1;
    isVolumeMetaDataEqual(m_DecodedFloatLargeVolume, m_FloatLargeVolume, 2, startPos, dimensions);
    // verify the volume data
    verifyFloatLargeVolumeData();

    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1
    startPos[0] = 250;
    startPos[1] = 375;
    startPos[2] = 500;
    dimensions[0] = originalDims[0] - 400;
    dimensions[1] = originalDims[1] - 525;
    dimensions[2] = originalDims[2] - 650;

    m_DecodedFloatLargeVolume->setUpdatedStartPos(startPos);
    m_DecodedFloatLargeVolume->setUpdatedDimensions(dimensions);
    m_DecodedFloatLargeVolume->updateDataValue();

    // verify the meta information
    isVolumeMetaDataEqual(m_DecodedFloatLargeVolume, m_FloatLargeVolume, 1, startPos, dimensions);
    // verify the volume data
    verifyFloatLargeVolumeData();
}

void mafExternalDataCodecBrickedVolumeTest::initGrayVolume() {
    // create the Gray volume (unsigned short)
    m_GrayVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedGrayVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/Gray716458.lod");
    
    m_GrayFileName      = tmp;   // Meta-data file name

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
    m_GrayVolume->setBricking(true);                // also set external encode type VOLUME_BRICKED_LOD
    m_GrayVolume->setBrickSize(16);
    m_GrayVolume->setMemoryLimit(64);               // 64k for test, 1M for default
    m_DecodedGrayVolume->setMemoryLimit(64);
    m_GrayVolume->setFileName("Gary716458");        // External data file name
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

void mafExternalDataCodecBrickedVolumeTest::initRGBVolume() {
    // create the RGB volume (unsigned char)
    m_RGBVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedRGBVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/RGB716458.lod");
    
    m_RGBFileName      = tmp;   // Meta-data file name

    int dimensions[3] = { 71, 64, 58 };
    float spacing[3]  = { 0.1f, 0.11f, 0.13f };
    m_RGBVolume->setDataType(mafPluginOutOfCore::mafVolUnsignedChar);
    m_RGBVolume->setBitsStored(8);
    m_RGBVolume->setBigEndian(false);
    m_RGBVolume->setComponentNum(3);
    m_RGBVolume->setOriginalDimensions(dimensions);
    m_RGBVolume->setSpacing(spacing);
    m_RGBVolume->setDimensions(dimensions);
    m_RGBVolume->setLevelNum(3);                    // level number for 64k memory limit
    m_RGBVolume->setBricking(true);                 // also set external encode type VOLUME_BRICKED_LOD
    m_RGBVolume->setBrickSize(16);
    m_RGBVolume->setMemoryLimit(64);                // 64k for test, 1M for default
    m_DecodedRGBVolume->setMemoryLimit(64);
    m_RGBVolume->setFileName("RGB716458");          // External data file name
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

void mafExternalDataCodecBrickedVolumeTest::initFloatEqualVolume() {
    // create the Float Equal volume (float)
    m_FloatEqualVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedFloatEqualVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/Equal716458.lod");
    
    m_FloatEqualFileName      = tmp;   // Meta-data file name

    int dimensions[3] = { 71, 64, 58 };
    float spacing[3]  = { 0.1f, 0.11f, 0.13f };
    m_FloatEqualVolume->setDataType(mafPluginOutOfCore::mafVolFloat);
    m_FloatEqualVolume->setBitsStored(32);
    m_FloatEqualVolume->setBigEndian(false);
    m_FloatEqualVolume->setComponentNum(1);
    m_FloatEqualVolume->setOriginalDimensions(dimensions);
    m_FloatEqualVolume->setSpacing(spacing);
    m_FloatEqualVolume->setDimensions(dimensions);
    m_FloatEqualVolume->setLevelNum(3);                     // level number for 64k memory limit
    m_FloatEqualVolume->setBricking(true);                  // also set external encode type VOLUME_BRICKED_LOD
    m_FloatEqualVolume->setBrickSize(16);
    m_FloatEqualVolume->setMemoryLimit(64);                 // 64k for test, 1M for default
    m_DecodedFloatEqualVolume->setMemoryLimit(64);
    m_FloatEqualVolume->setFileName("Equal716458");         // External data file name
    // dataValue
    float *data = new float[dimensions[0] * dimensions[1] * dimensions[2]];
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                int index = ((z * dimensions[1] + y ) * dimensions[0] + x);
                if (z < 32)
                    data[index] = 10.25f;
                else if (z >= 64)
                    data[index] = 0;
                else 
                    data[index] = index;
            }
        }
    }
    m_FloatEqualVolume->setDataValue(data);
}

void mafExternalDataCodecBrickedVolumeTest::initFloatLargeVolume() {
    // create the Float Large volume (float)
    m_FloatLargeVolume        = mafNEW(mafPluginOutOfCore::mafVolume);
    m_DecodedFloatLargeVolume = mafNEW(mafPluginOutOfCore::mafVolume);
    
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/FloatBrick1024.lod");

    m_FloatLargeFileName      = tmp;   // Meta-data file name

    int dimensions[3] = { 1024, 1024, 1024 };
    float spacing[3]  = { 0.1f, 0.1f, 0.1f };
    m_FloatLargeVolume->setDataType(mafPluginOutOfCore::mafVolFloat);
    m_FloatLargeVolume->setBitsStored(32);
    m_FloatLargeVolume->setBigEndian(false);
    m_FloatLargeVolume->setComponentNum(1);
    m_FloatLargeVolume->setOriginalDimensions(dimensions);
    m_FloatLargeVolume->setSpacing(spacing);
    m_FloatLargeVolume->setDimensions(dimensions);
    m_FloatLargeVolume->setLevelNum(4);                             // level number for 64k memory limit
    m_FloatLargeVolume->setBricking(true);                          // also set external encode type VOLUME_BRICKED_LOD
    m_FloatLargeVolume->setBrickSize(64);
    m_FloatLargeVolume->setMemoryLimit(16 * 1024);                  // 16M
    m_DecodedFloatLargeVolume->setMemoryLimit(16 * 1024);
    m_FloatLargeVolume->setFileName("floatBrick1024x1024x1024");    // External data file name
    // no dataValue, use encodeFloatVolume to save the volume data directly
}

void mafExternalDataCodecBrickedVolumeTest::encodeFloatLargeVolume() {
    QString tmp = QDir::tempPath();
    tmp.append("/data");
    QDir data_dir(tmp);
    if(!data_dir.exists()) {
        data_dir.mkpath(tmp);
    }
    tmp.append("/" + m_FloatLargeVolume->fileName());
    
    QString url = tmp ;
    QFile bpiFile(url + ".bpi"), bevFile(url + ".bev"), bvFile(url + ".bv");
    if (!bpiFile.open(QIODevice::WriteOnly)) {
        qDebug() << ("Not able to open file " + url + ".bpi");
        return;
    }
    if (!bevFile.open(QIODevice::WriteOnly)) {
        qDebug() << ("Not able to open file " + url + ".bev");
        return;
    }
    if (!bvFile.open(QIODevice::WriteOnly)) {
        qDebug() << ("Not able to open file " + url + ".bv");
        return;
    }

    int scale = 1;
    int levels = m_FloatLargeVolume->levelNum();
    int brickSize = m_FloatLargeVolume->brickSize();
    int *dimensions = m_FloatLargeVolume->dimensions();
    float *data = new float[brickSize * brickSize * brickSize];

    qint64 brickPosition = 0;
    for (int level = 0; level < levels; ++level) {
        int xDim = dimensions[0] / scale;
        int yDim = dimensions[1] / scale;
        int zDim = dimensions[2] / scale;

        int xBrick = xDim / brickSize;
        int yBrick = yDim / brickSize;
        int zBrick = zDim / brickSize;
        for (int bz = 0; bz < zBrick; ++bz) {
            int zStart = bz * brickSize;
            for (int by = 0; by < yBrick; ++by) {
                int yStart = by * brickSize;
                for (int bx = 0; bx < xBrick; ++bx) {
                    int xStart = bx * brickSize;
                    for (int z = 0; z < brickSize; ++z) {
                        for (int y = 0; y < brickSize; ++y) {
                            for (int x = 0; x < brickSize; ++x) {
                                int indexBrick = (z * brickSize + y) * brickSize + x;
                                int indexData  = (((zStart + z) * dimensions[1] + (yStart + y)) * dimensions[0] + (xStart + x)) * scale;
                                data[indexBrick] = indexData / 100.f;
                            }
                        }
                    }
                    qint64 position = brickPosition << 1;
                    bpiFile.write((const char*)&position, sizeof(qint64));
                    bvFile.write((const char*)data, sizeof(float) * brickSize * brickSize * brickSize);
                    brickPosition += sizeof(float) * brickSize * brickSize * brickSize; 
                }
            }
        }
        scale *= 2;
    }
    
    delete []data;
    bpiFile.close();
    bevFile.close();
    bvFile.close();
}

void mafExternalDataCodecBrickedVolumeTest::isVolumeMetaDataEqual(mafVolume *volumeA, mafVolume *volumeB, int level, int startPos[3], int dimensions[3]) {
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
    QVERIFY(volumeA->brickSize() == volumeB->brickSize());
    QVERIFY(volumeA->codecType() == volumeB->codecType());
    QVERIFY(volumeA->levelNum()  == volumeB->levelNum());
    //QVERIFY(volumeA->fileName() == volumeB->fileName());
}

void mafExternalDataCodecBrickedVolumeTest::verifyGrayVolumeData() {
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

void mafExternalDataCodecBrickedVolumeTest::verifyRGBVolumeData() {
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

void mafExternalDataCodecBrickedVolumeTest::verifyFloatEqualVolumeData() {
    int  level         = m_DecodedFloatEqualVolume->level();
    int  scale         = pow(2.f, level - 1.f);
    int  *startPos     = m_DecodedFloatEqualVolume->startPositions();
    int  *dimensions   = m_DecodedFloatEqualVolume->dimensions();
    int  *originalDims = m_DecodedFloatEqualVolume->originalDimensions();
    float *readData     = (float *)m_DecodedFloatEqualVolume->dataValue();
    float *originalData = (float *)m_FloatEqualVolume->dataValue();

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

void mafExternalDataCodecBrickedVolumeTest::verifyFloatLargeVolumeData() {
    int   level         = m_DecodedFloatLargeVolume->level();
    int   scale         = pow(2.f, level - 1.f);
    int   *startPos     = m_DecodedFloatLargeVolume->startPositions();
    int   *dimensions   = m_DecodedFloatLargeVolume->dimensions();
    int   *originalDims = m_DecodedFloatLargeVolume->originalDimensions();
    float *readData     = (float *)m_DecodedFloatLargeVolume->dataValue();
    
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

MAF_REGISTER_TEST(mafExternalDataCodecBrickedVolumeTest);
#include "mafExternalDataCodecBrickedVolumeTest.moc"