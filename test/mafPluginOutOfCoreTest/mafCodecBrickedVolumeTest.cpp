/*
 *  mafCodecBrickedVolumeTest.cpp
 *  mafSerializationTest
 *
 *  Created by Yubo Tao on 30/12/10.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCodecBrickedVolume.h>
#include <mafObject.h>
#include <mafMementoObject.h>

using namespace mafCore;
using namespace mafSerialization;

/**
 Class name: mafCodecBrickedVolumeTest
 This class implements the test suite for mafCodecBrickedVolume.
 */

//! <title>
//mafCodecBrickedVolume
//! </title>
//! <description>
//mafCodecBrickedVolume is a codec to Encode/Decode maf volume objects into a raw bricked memory dump using multiresolution.
//! </description>

class mafCodecBrickedVolumeTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafCodecBrickedVolume allocation test case.
    void mafCodecBrickedVolumeAllocationTest();

    /// test the encode method.
    void encodeTest();
    
    /// test the decode method using the Gray memento.
    void decodeGrayMementoTest();

    /// test the decode method using the RGB memento.
    void decodeRGBMementoTest();

    /// test the decode method using the Float Equal memento.
    void decodeFloatEqualMementoTest();

    /// test the decode method using the Float Large memento.
    //void decodeFloatLargeMementoTest();
    
    /// test the update method using the Gray memento.
    void updateGrayMementoTest();

    /// test the update method using the RGB memento.
    void updateRGBMementoTest();

    /// test the update method using the Float Equal memento.
    void updateFloatEqualMementoTest();

    /// test the update method using the Float Large memento.
    //void updateFloatLargeMementoTest();

private:
    /// find the memento property item given the name
    mafMementoPropertyItem findPropertyItem(mafMemento *menento, QString name);

    /// verify the serialization of the Gray Memento
    void verifyGrayMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);
    
    /// verify the serialization of the RGB Memento
    void verifyRGBMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    /// verify the serialization of the float equal Memento
    void verifyFloatEqualMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    /// verify the serialization of the float large Memento
    void verifyFloatLargeMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    mafCodecBrickedVolume *m_CodecBrickedVolume;///< Test var
    int m_GrayDimensions[3];                    ///< The dimensions of the gray volume
    int m_RGBDimensions[3];                     ///< The dimension of the RGB volume
    int m_FloatEqualDimensions[3];              ///< The dimension of the float volume with equal values
    int m_FloatLargeDimensions[3];              ///< The dimension of the float large volume 
    QString m_GrayFileName;                     ///< The file name of m_GrayMemento
    QString m_RGBFileName;                      ///< The file name of m_RGBMemento
    QString m_FloatEqualFileName;               ///< The file name of m_FloatEqualMemento
    QString m_FloatLargeFileName;               ///< The file name of m_FloatLargeMemento
    mafMemento *m_GrayMemento;                  ///< Test Object (Gray unsigned short)
    mafMemento *m_RGBMemento;                   ///< Test Object (RGB unsigned char)
    mafMemento *m_FloatEqualMemento;            ///< Test Object (Gray float for testing bricks with equal values)
    mafMemento *m_FloatLargeMemento;            ///< Test Object (Gray float for testing large volumes)
    mafMemento *m_LoadedGrayMemento;            ///< The loaded memento from m_GrayFileName
    mafMemento *m_LoadedRGBMemento;             ///< The loaded memento from m_RGBFileName
    mafMemento *m_LoadedFloatEqualMemento;      ///< The loaded memento from m_FloatEqualFileName
    mafMemento *m_LoadedFloatLargeMemento;      ///< The loaded memento from m_FloatLargeFileName
};

void mafCodecBrickedVolumeTest::initTestCase() {
    QVariantList list;
    mafMementoPropertyItem item;
    item.m_Multiplicity       = 1;
    m_GrayDimensions[0]       = 71;
    m_GrayDimensions[1]       = 64;
    m_GrayDimensions[2]       = 58;
    m_RGBDimensions[0]        = 71;
    m_RGBDimensions[1]        = 64;
    m_RGBDimensions[2]        = 58;
    m_FloatEqualDimensions[0] = 71;
    m_FloatEqualDimensions[1] = 64;
    m_FloatEqualDimensions[2] = 58;
    m_FloatLargeDimensions[0] = 1024;
    m_FloatLargeDimensions[1] = 1024;
    m_FloatLargeDimensions[2] = 1024;
    m_GrayFileName            = "./Gray716458.lod";
    m_RGBFileName             = "./RGB716458.lod";
    m_FloatEqualFileName      = "./Equal716458.lod";
    m_FloatLargeFileName      = "./FloatBrick1024.lod";
    m_LoadedGrayMemento       = 0;
    m_LoadedRGBMemento        = 0;
    m_LoadedFloatEqualMemento = 0;
    m_LoadedFloatLargeMemento = 0;

    // init the test variable
    m_CodecBrickedVolume = mafNEW(mafSerialization::mafCodecBrickedVolume);

    
    // 1.init the gray unsigned short int volume data
    m_GrayMemento = new mafMemento();
    mafMementoPropertyList *propList = m_GrayMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_Bricked_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)301210;
    propList->append(item);
    // dataHash
    item.m_Name  = "dataHash";
    item.m_Value = m_GrayMemento->objectHash();
    propList->append(item);
    // dataType
    item.m_Name  = "dataType";
    item.m_Value = (int)mafUnsignedShort;
    propList->append(item);
    // bitsStored
    item.m_Name  = "bitsStored";
    item.m_Value = 12;
    propList->append(item);
    // bigEndian
    item.m_Name  = "bigEndian";
    item.m_Value = false;
    propList->append(item);
    // componentNum
    item.m_Name  = "componentNum";
    item.m_Value = 1;
    propList->append(item);
    // dimensions
    list.clear();
    list.append(m_GrayDimensions[0]);
    list.append(m_GrayDimensions[1]);
    list.append(m_GrayDimensions[2]);
    item.m_Name  = "dimensions";
    item.m_Value = list;
    propList->append(item);
    // brickSize
    item.m_Name  = "brickSize";
    item.m_Value = 16;
    propList->append(item);
    // spacing
    list.clear();
    list.append(0.1f);
    list.append(0.11f);
    list.append(0.13f);
    item.m_Name  = "spacing";
    item.m_Value = list;
    propList->append(item);
    // posMatrix
    list.clear();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double value = (i == j);
            list.append(value);
        }
    }
    item.m_Name  = "posMatrix";
    item.m_Value = list;
    propList->append(item);
    // orginalDimensions
    list.clear();
    list.append(m_GrayDimensions[0]);
    list.append(m_GrayDimensions[1]);
    list.append(m_GrayDimensions[2]);
    item.m_Name  = "originalDimensions";
    item.m_Value = list;
    propList->append(item);
    // startPositions
    list.clear();
    list.append(0);
    list.append(0);
    list.append(0);
    item.m_Name  = "startPositions";
    item.m_Value = list;
    propList->append(item);
    // multiresolution
    item.m_Name  = "multiresolution";
    item.m_Value = true;
    propList->append(item);
    // memoryLimit
    item.m_Name  = "memoryLimit";
    item.m_Value = 64;   // 64k for test, 1M for default
    propList->append(item);
    // dataValue
    unsigned short *data = new unsigned short[m_GrayDimensions[0] * m_GrayDimensions[1] * m_GrayDimensions[2]];
    for (int z = 0; z < m_GrayDimensions[2]; ++z) {
        for (int y = 0; y < m_GrayDimensions[1]; ++y) {
            for (int x = 0; x < m_GrayDimensions[0]; ++x) {
                int index = (z * m_GrayDimensions[1] + y) * m_GrayDimensions[0] + x;
                data[index] = index % 4096;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data);
    propList->append(item);

    // 2.init the RGB unsigned char volume data
    m_RGBMemento = new mafMemento();
    propList = m_RGBMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_Bricked_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)301210;
    propList->append(item);
    // dataHash
    item.m_Name  = "dataHash";
    item.m_Value = m_RGBMemento->objectHash();
    propList->append(item);
    // dataType
    item.m_Name  = "dataType";
    item.m_Value = (int)mafUnsignedChar;
    propList->append(item);
    // bitsStored
    item.m_Name  = "bitsStored";
    item.m_Value = 8;
    propList->append(item);
    // bigEndian
    item.m_Name  = "bigEndian";
    item.m_Value = false;
    propList->append(item);
    // componentNum
    item.m_Name  = "componentNum";
    item.m_Value = 3;
    propList->append(item);
    // dimensions
    list.clear();
    list.append(m_RGBDimensions[0]);
    list.append(m_RGBDimensions[1]);
    list.append(m_RGBDimensions[2]);
    item.m_Name  = "dimensions";
    item.m_Value = list;
    propList->append(item);
    // brickSize
    item.m_Name  = "brickSize";
    item.m_Value = 16;
    propList->append(item);
    // spacing
    list.clear();
    list.append(0.1f);
    list.append(0.11f);
    list.append(0.13f);
    item.m_Name  = "spacing";
    item.m_Value = list;
    propList->append(item);
    // posMatrix
    list.clear();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double value = (i == j);
            list.append(value);
        }
    }
    item.m_Name  = "posMatrix";
    item.m_Value = list;
    propList->append(item);
    // orginalDimensions
    list.clear();
    list.append(m_RGBDimensions[0]);
    list.append(m_RGBDimensions[1]);
    list.append(m_RGBDimensions[2]);
    item.m_Name  = "originalDimensions";
    item.m_Value = list;
    propList->append(item);
    // startPositions
    list.clear();
    list.append(0);
    list.append(0);
    list.append(0);
    item.m_Name  = "startPositions";
    item.m_Value = list;
    propList->append(item);
    // multiresolution
    item.m_Name  = "multiresolution";
    item.m_Value = true;
    propList->append(item);
    // memoryLimit
    item.m_Name  = "memoryLimit";
    item.m_Value = 64;   // 64k for test, 1M for default
    propList->append(item);
    // dataValue
    unsigned char *data2 = new unsigned char[m_RGBDimensions[0] * m_RGBDimensions[1] * m_RGBDimensions[2] * 3];
    for (int z = 0; z < m_RGBDimensions[2]; ++z) {
        for (int y = 0; y < m_RGBDimensions[1]; ++y) {
            for (int x = 0; x < m_RGBDimensions[0]; ++x) {
                int index = ((z * m_RGBDimensions[1] + y) * m_RGBDimensions[0] + x) * 3;
                data2[index + 0] = z;
                data2[index + 1] = y;
                data2[index + 2] = x;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data2);
    propList->append(item);

    // 3.init the gray float volume data with equal values
    m_FloatEqualMemento = new mafMemento();    
    propList = m_FloatEqualMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_Bricked_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)301210;
    propList->append(item);
    // dataHash
    item.m_Name  = "dataHash";
    item.m_Value = m_FloatEqualMemento->objectHash();
    propList->append(item);
    // dataType
    item.m_Name  = "dataType";
    item.m_Value = (int)mafFloat;
    propList->append(item);
    // bitsStored
    item.m_Name  = "bitsStored";
    item.m_Value = 32;
    propList->append(item);
    // bigEndian
    item.m_Name  = "bigEndian";
    item.m_Value = false;
    propList->append(item);
    // componentNum
    item.m_Name  = "componentNum";
    item.m_Value = 1;
    propList->append(item);
    // dimensions
    list.clear();
    list.append(m_FloatEqualDimensions[0]);
    list.append(m_FloatEqualDimensions[1]);
    list.append(m_FloatEqualDimensions[2]);
    item.m_Name  = "dimensions";
    item.m_Value = list;
    propList->append(item);
    // brickSize
    item.m_Name  = "brickSize";
    item.m_Value = 16;
    propList->append(item);
    // spacing
    list.clear();
    list.append(0.1f);
    list.append(0.11f);
    list.append(0.13f);
    item.m_Name  = "spacing";
    item.m_Value = list;
    propList->append(item);
    // posMatrix
    list.clear();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double value = (i == j);
            list.append(value);
        }
    }
    item.m_Name  = "posMatrix";
    item.m_Value = list;
    propList->append(item);
    // orginalDimensions
    list.clear();
    list.append(m_FloatEqualDimensions[0]);
    list.append(m_FloatEqualDimensions[1]);
    list.append(m_FloatEqualDimensions[2]);
    item.m_Name  = "originalDimensions";
    item.m_Value = list;
    propList->append(item);
    // startPositions
    list.clear();
    list.append(0);
    list.append(0);
    list.append(0);
    item.m_Name  = "startPositions";
    item.m_Value = list;
    propList->append(item);
    // multiresolution
    item.m_Name  = "multiresolution";
    item.m_Value = true;
    propList->append(item);
    // memoryLimit
    item.m_Name  = "memoryLimit";
    item.m_Value = 64;   // 64k for test, 1M for default
    propList->append(item);
    // dataValue
    float *data3 = new float[m_FloatEqualDimensions[0] * m_FloatEqualDimensions[1] * m_FloatEqualDimensions[2]];
    for (int z = 0; z < m_FloatEqualDimensions[2]; ++z) {
        for (int y = 0; y < m_FloatEqualDimensions[1]; ++y) {
            for (int x = 0; x < m_FloatEqualDimensions[0]; ++x) {
                int index = (z * m_FloatEqualDimensions[1] + y) * m_FloatEqualDimensions[0] + x;
                if (z < 32)
                    data3[index] = 10.25f;
                else if (z >= 64)
                    data3[index] = 0;
                else 
                    data3[index] = index;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data3);
    propList->append(item);

    // 4.init the gray float large volume data
    m_FloatLargeMemento = new mafMemento();
    propList = m_FloatLargeMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_Bricked_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)040411;
    propList->append(item);
    // dataHash
    item.m_Name  = "dataHash";
    item.m_Value = m_FloatLargeMemento->objectHash();
    propList->append(item);
    // dataType
    item.m_Name  = "dataType";
    item.m_Value = (int)mafFloat;
    propList->append(item);
    // bitsStored
    item.m_Name  = "bitsStored";
    item.m_Value = 32;
    propList->append(item);
    // bigEndian
    item.m_Name  = "bigEndian";
    item.m_Value = false;
    propList->append(item);
    // componentNum
    item.m_Name  = "componentNum";
    item.m_Value = 1;
    propList->append(item);
    // dimensions
    list.clear();
    list.append(m_FloatLargeDimensions[0]);
    list.append(m_FloatLargeDimensions[1]);
    list.append(m_FloatLargeDimensions[2]);
    item.m_Name  = "dimensions";
    item.m_Value = list;
    propList->append(item);
    // brickSize
    item.m_Name  = "brickSize";
    item.m_Value = 64;
    propList->append(item);
    // spacing
    list.clear();
    list.append(0.1f);
    list.append(0.1f);
    list.append(0.1f);
    item.m_Name  = "spacing";
    item.m_Value = list;
    propList->append(item);
    // posMatrix
    list.clear();
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            double value = (i == j);
            list.append(value);
        }
    }
    item.m_Name  = "posMatrix";
    item.m_Value = list;
    propList->append(item);
    // orginalDimensions
    list.clear();
    list.append(m_FloatLargeDimensions[0]);
    list.append(m_FloatLargeDimensions[1]);
    list.append(m_FloatLargeDimensions[2]);
    item.m_Name  = "originalDimensions";
    item.m_Value = list;
    propList->append(item);
    // startPositions
    list.clear();
    list.append(0);
    list.append(0);
    list.append(0);
    item.m_Name  = "startPositions";
    item.m_Value = list;
    propList->append(item);
    // multiresolution
    item.m_Name  = "multiresolution";
    item.m_Value = true;
    propList->append(item);
    // memoryLimit
    item.m_Name  = "memoryLimit";
    item.m_Value = 16 * 1024;   // 16M
    propList->append(item);
    // dataValue
    float *data4 = 0;
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data4);
    propList->append(item);
}

void mafCodecBrickedVolumeTest::cleanupTestCase() {
    mafDEL(m_CodecBrickedVolume);
    // release the volume data in m_GrayMemento
    mafMementoPropertyList *propList = m_GrayMemento->mementoPropertyList();
    mafMementoPropertyList::iterator iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            unsigned short *data = (unsigned short *)iter->m_Value.value<void *>();
            delete []data;
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafDEL(m_GrayMemento);
    QFile::remove(m_GrayFileName);
    
    // release the volume data in m_RGBMemento
    propList = m_RGBMemento->mementoPropertyList();
    iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            unsigned char *data = (unsigned char *)iter->m_Value.value<void *>();
            delete []data;
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafDEL(m_RGBMemento);
    QFile::remove(m_RGBFileName);

    // release the volume data in m_FloatEqualMemento
    propList = m_FloatEqualMemento->mementoPropertyList();
    iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            float *data = (float *)iter->m_Value.value<void *>();
            delete []data;
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafDEL(m_FloatEqualMemento);
    QFile::remove(m_FloatEqualFileName);

    // release m_FloatLargeMemento
    propList = m_FloatLargeMemento->mementoPropertyList();
    propList->clear();
    mafDEL(m_FloatLargeMemento);

    // release the volume data in m_LoadedGrayMemento
    if (m_LoadedGrayMemento) {
        propList = m_LoadedGrayMemento->mementoPropertyList();
        iter = propList->begin();
        while (iter != propList->end()) {
            if (iter->m_Name == "dataValue") {
                unsigned short *data = (unsigned short *)iter->m_Value.value<void *>();
                delete []data;
            } else if (iter->m_Name == "fileName") {
                QString fileName = iter->m_Value.toString();
                QFile::remove(fileName + ".bpi");
                QFile::remove(fileName + ".bev");
                QFile::remove(fileName + ".bv");
            }
            iter = propList->erase(iter);
        }
        propList->clear();
        mafDEL(m_LoadedGrayMemento);
    }

    // release the volume data in m_LoadedRGBMemento
    if (m_LoadedRGBMemento) {
        propList = m_LoadedRGBMemento->mementoPropertyList();
        iter = propList->begin();
        while (iter != propList->end()) {
            if (iter->m_Name == "dataValue") {
                unsigned char *data = (unsigned char *)iter->m_Value.value<void *>();
                delete []data;
            } else if (iter->m_Name == "fileName") {
                QString fileName = iter->m_Value.toString();
                QFile::remove(fileName + ".bpi");
                QFile::remove(fileName + ".bev");
                QFile::remove(fileName + ".bv");
            }
            iter = propList->erase(iter);
        }
        propList->clear();
        mafDEL(m_LoadedRGBMemento);
    }

    // release the volume data in m_LoadedFloatEqualMemento
    if (m_LoadedFloatEqualMemento) {
        propList = m_LoadedFloatEqualMemento->mementoPropertyList();
        iter = propList->begin();
        while (iter != propList->end()) {
            if (iter->m_Name == "dataValue") {
                float *data = (float *)iter->m_Value.value<void *>();
                delete []data;
            } else if (iter->m_Name == "fileName") {
                QString fileName = iter->m_Value.toString();
                QFile::remove(fileName + ".bpi");
                QFile::remove(fileName + ".bev");
                QFile::remove(fileName + ".bv");
            }
            iter = propList->erase(iter);
        }
        propList->clear();
        mafDEL(m_LoadedFloatEqualMemento);
    }

    // release the volume data in m_LoadedFloatLargeMemento
    if (m_LoadedFloatLargeMemento) {
        propList = m_LoadedFloatLargeMemento->mementoPropertyList();
        iter = propList->begin();
        while (iter != propList->end()) {
            if (iter->m_Name == "dataValue") {
                float *data = (float *)iter->m_Value.value<void *>();
                delete []data;
            }
            iter = propList->erase(iter);
        }
        propList->clear();
        mafDEL(m_LoadedFloatLargeMemento);
    }
}

void mafCodecBrickedVolumeTest::mafCodecBrickedVolumeAllocationTest() {
    QVERIFY(m_CodecBrickedVolume);
}

void mafCodecBrickedVolumeTest::encodeTest() {
    // test case 1: the gray unsigned short volume data
    // open the gray file in writing modality
    QFile file(m_GrayFileName);
    bool success = file.open(QIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecBrickedVolume->encode(m_GrayMemento);

    // close the gray file
    file.close();

    QVERIFY(QFile::exists(m_GrayFileName));

    // test case 2: the RGB unsigned char volume data
    // open the RGB file in writing modality
    file.setFileName(m_RGBFileName);
    success = file.open(QIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecBrickedVolume->encode(m_RGBMemento);

    // close the RGB file
    file.close();

    QVERIFY(QFile::exists(m_RGBFileName));

    // test case 3: the gray float volume data with equal values
    // open the float file in writing modality
    file.setFileName(m_FloatEqualFileName);
    success = file.open(QIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecBrickedVolume->encode(m_FloatEqualMemento);

    // close the float file
    file.close();

    QVERIFY(QFile::exists(m_FloatEqualFileName));

    // test case 4: the gray float large volume data
    //// open the float file in writing modality
    //file.setFileName(m_FloatLargeFileName);
    //success = file.open(QIODevice::WriteOnly);
    //QVERIFY(success);

    //// assign the IODevice
    //m_CodecBrickedVolume->setDevice(&file);

    //// give the memento to the codec
    //m_CodecBrickedVolume->encode(m_FloatLargeMemento);

    //// close the float file
    //file.close();

    //QVERIFY(QFile::exists(m_FloatLargeFileName));
}

void mafCodecBrickedVolumeTest::decodeGrayMementoTest() {
    // open the gray file in reading modality
    QFile file(m_GrayFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // decode the information 
    m_LoadedGrayMemento = m_CodecBrickedVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { 0, 0, 0 };
    QVERIFY(m_LoadedGrayMemento != 0);
    verifyGrayMemento(m_LoadedGrayMemento, -1, startPos, dimensions);
}

void mafCodecBrickedVolumeTest::decodeRGBMementoTest() {
    // open the RGB file in reading modality
    QFile file(m_RGBFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // decode the information 
    m_LoadedRGBMemento = m_CodecBrickedVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { 0, 0, 0 };
    QVERIFY(m_LoadedRGBMemento != 0);
    verifyRGBMemento(m_LoadedRGBMemento, -1, startPos, dimensions);
}

void mafCodecBrickedVolumeTest::decodeFloatEqualMementoTest() {
    // open the float file in reading modality
    QFile file(m_FloatEqualFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // decode the information 
    m_LoadedFloatEqualMemento = m_CodecBrickedVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { 0, 0, 0 };
    QVERIFY(m_LoadedFloatEqualMemento != 0);
    verifyFloatEqualMemento(m_LoadedFloatEqualMemento, -1, startPos, dimensions);
}

//void mafCodecBrickedVolumeTest::decodeFloatLargeMementoTest() {
//    // open the float file in reading modality
//    QFile file(m_FloatLargeFileName);
//    bool success = file.open(QIODevice::ReadOnly);
//    QVERIFY(success);
//
//    // assign the IODevice
//    m_CodecBrickedVolume->setDevice(&file);
//
//    // decode the information 
//    m_LoadedFloatLargeMemento = m_CodecBrickedVolume->decode();
//
//    // close the gray file
//    file.close();
//
//    // verify the information
//    int startPos[3]   = { 0, 0, 0 };
//    int dimensions[3] = { 0, 0, 0 };
//    QVERIFY(m_LoadedFloatLargeMemento != 0);
//    verifyFloatLargeMemento(m_LoadedFloatLargeMemento, -1, startPos, dimensions);
//}

void mafCodecBrickedVolumeTest::updateGrayMementoTest() {
    QVariantList list;
    mafMementoPropertyItem item;
    int startPos[3], dimensions[3];

    // open the gray file in reading modality
    QFile file(m_GrayFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // property list
    mafMementoPropertyList *propList = m_LoadedGrayMemento->mementoPropertyList();

    // test case 1: lazy initialization
    startPos[0] = 0;
    startPos[1] = 0;
    startPos[2] = 0;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_GrayDimensions[0];
    dimensions[1] = m_GrayDimensions[1];
    dimensions[2] = m_GrayDimensions[2];
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedGrayMemento);
    
    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    verifyGrayMemento(m_LoadedGrayMemento, 2, startPos, dimensions);

    // test case 2: update the start positions and dimensions, and extract volume data from the loaded volume data in level 2
    startPos[0] = 3;
    startPos[1] = 6;
    startPos[2] = 9;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_GrayDimensions[0] - 20;
    dimensions[1] = m_GrayDimensions[1] - 15;
    dimensions[2] = m_GrayDimensions[2] - 10;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedGrayMemento);
    
    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    verifyGrayMemento(m_LoadedGrayMemento, 2, startPos, dimensions);

    // test case 3: update the start positions only, and load the volume data from the disk in level 2
    startPos[0] = 4;
    startPos[1] = 7;
    startPos[2] = 9;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedGrayMemento);

    // verify the information
    for (int i = 0; i < 3; ++i)
        startPos[i] = startPos[i] >> 1;
    verifyGrayMemento(m_LoadedGrayMemento, 2, startPos, dimensions);

    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1 
    startPos[0] = 10;
    startPos[1] = 15;
    startPos[2] = 20;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_GrayDimensions[0] - 40;
    dimensions[1] = m_GrayDimensions[1] - 35;
    dimensions[2] = m_GrayDimensions[2] - 30;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedGrayMemento);

    // verify the information
    verifyGrayMemento(m_LoadedGrayMemento, 1, startPos, dimensions);

    // test case 5: update the dimensions only, and extract the volume data from the loaded volume data in level 1 
    dimensions[0] = m_GrayDimensions[0] - 40;
    dimensions[1] = m_GrayDimensions[1] - 30;
    dimensions[2] = m_GrayDimensions[2] - 40;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedGrayMemento);

    // verify the information
    verifyGrayMemento(m_LoadedGrayMemento, 1, startPos, dimensions);

    // close the gray file
    file.close();
}

void mafCodecBrickedVolumeTest::updateRGBMementoTest() {
    QVariantList list;
    mafMementoPropertyItem item;
    int startPos[3], dimensions[3];

    // open the RGB file in reading modality
    QFile file(m_RGBFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // property list
    mafMementoPropertyList *propList = m_LoadedRGBMemento->mementoPropertyList();

    // test case 1: lazy initialization
    startPos[0] = 0;
    startPos[1] = 0;
    startPos[2] = 0;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_RGBDimensions[0];
    dimensions[1] = m_RGBDimensions[1];
    dimensions[2] = m_RGBDimensions[2];
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedRGBMemento);

    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 2;
        dimensions[i] = dimensions[i] >> 2;
    }
    verifyRGBMemento(m_LoadedRGBMemento, 3, startPos, dimensions);

    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
    startPos[0] = 3;
    startPos[1] = 6;
    startPos[2] = 9;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_RGBDimensions[0] - 15;
    dimensions[1] = m_RGBDimensions[1] - 13;
    dimensions[2] = m_RGBDimensions[2] - 11;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedRGBMemento);

    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    verifyRGBMemento(m_LoadedRGBMemento, 2, startPos, dimensions);

    // test case 3: update the start positions only, and load the volume data from the disk in level 2
    startPos[0] = 4;
    startPos[1] = 7;
    startPos[2] = 9;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedRGBMemento);

    // verify the information
    for (int i = 0; i < 3; ++i)
        startPos[i] = startPos[i] >> 1;
    verifyRGBMemento(m_LoadedRGBMemento, 2, startPos, dimensions);

    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1 
    startPos[0] = 10;
    startPos[1] = 15;
    startPos[2] = 20;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_RGBDimensions[0] - 40;
    dimensions[1] = m_RGBDimensions[1] - 35;
    dimensions[2] = m_RGBDimensions[2] - 35;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedRGBMemento);

    // verify the information
    verifyRGBMemento(m_LoadedRGBMemento, 1, startPos, dimensions);

    // test case 5: update the dimensions only, and extract the volume data from the loaded volume data in level 1
    dimensions[0] = m_RGBDimensions[0] - 40;
    dimensions[1] = m_RGBDimensions[1] - 38;
    dimensions[2] = m_RGBDimensions[2] - 35;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedRGBMemento);

    // verify the information
    verifyRGBMemento(m_LoadedRGBMemento, 1, startPos, dimensions);

    // close the RGB file
    file.close();
}

void mafCodecBrickedVolumeTest::updateFloatEqualMementoTest() {
    QVariantList list;
    mafMementoPropertyItem item;
    int startPos[3], dimensions[3];

    // open the float file in reading modality
    QFile file(m_FloatEqualFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // property list
    mafMementoPropertyList *propList = m_LoadedFloatEqualMemento->mementoPropertyList();

    // test case 1: lazy initialization
    startPos[0] = 0;
    startPos[1] = 0;
    startPos[2] = 0;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_FloatEqualDimensions[0];
    dimensions[1] = m_FloatEqualDimensions[1];
    dimensions[2] = m_FloatEqualDimensions[2];
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedFloatEqualMemento);

    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 2;
        dimensions[i] = dimensions[i] >> 2;
    }
    verifyFloatEqualMemento(m_LoadedFloatEqualMemento, 3, startPos, dimensions);

    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
    startPos[0] = 3;
    startPos[1] = 6;
    startPos[2] = 9;
    list.clear();
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name  = "updatedStartPositions";
    item.m_Value = list;
    propList->append(item);

    dimensions[0] = m_FloatEqualDimensions[0] - 18;
    dimensions[1] = m_FloatEqualDimensions[1] - 15;
    dimensions[2] = m_FloatEqualDimensions[2] - 12;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedFloatEqualMemento);

    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    verifyFloatEqualMemento(m_LoadedFloatEqualMemento, 2, startPos, dimensions);

    // test case 3: update the dimensions only, and load the volume data from the disk in level 1
    dimensions[0] = m_FloatEqualDimensions[0] - 40;
    dimensions[1] = m_FloatEqualDimensions[1] - 39;
    dimensions[2] = m_FloatEqualDimensions[2] - 38;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedFloatEqualMemento);

    // verify the information
    for (int i = 0; i < 3; ++i)
        startPos[i] = startPos[i] << 1;
    verifyFloatEqualMemento(m_LoadedFloatEqualMemento, 1, startPos, dimensions);
    
    // close the float file
    file.close();
}

//void mafCodecBrickedVolumeTest::updateFloatLargeMementoTest() {
//    QVariantList list;
//    mafMementoPropertyItem item;
//    int startPos[3], dimensions[3];
//
//    // open the float large volume file in reading modality
//    QFile file(m_FloatLargeFileName);
//    bool success = file.open(QIODevice::ReadOnly);
//    QVERIFY(success);
//
//    // assign the IODevice
//    m_CodecBrickedVolume->setDevice(&file);
//
//    // property list
//    mafMementoPropertyList *propList = m_LoadedFloatLargeMemento->mementoPropertyList();
//
//    // test case 1: lazy initialization
//    startPos[0] = 0;
//    startPos[1] = 0;
//    startPos[2] = 0;
//    list.clear();
//    list.append(startPos[0]);
//    list.append(startPos[1]);
//    list.append(startPos[2]);
//    item.m_Name  = "updatedStartPositions";
//    item.m_Value = list;
//    propList->append(item);
//
//    dimensions[0] = m_FloatLargeDimensions[0];
//    dimensions[1] = m_FloatLargeDimensions[1];
//    dimensions[2] = m_FloatLargeDimensions[2];
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecBrickedVolume->update(m_LoadedFloatLargeMemento);
//
//    // verify the information
//    for (int i = 0; i < 3; ++i) {
//        startPos[i]   = startPos[i] >> 3;
//        dimensions[i] = dimensions[i] >> 3;
//    }
//    verifyFloatLargeMemento(m_LoadedFloatLargeMemento, 4, startPos, dimensions);
//
//    // update the memory limit in the float large volume memento for test case 2 - 4
//    for (mafMementoPropertyList::iterator iter = propList->begin(); iter != propList->end(); ++iter) {
//        if (iter->m_Name == "memoryLimit")
//            iter->m_Value = 512 * 1024; // 512M
//    }
//
//    // test case 2: update the start positions and dimensions, and load the volume data from the disk in level 2
//    startPos[0] = 125;
//    startPos[1] = 250;
//    startPos[2] = 375;
//    list.clear();
//    list.append(startPos[0]);
//    list.append(startPos[1]);
//    list.append(startPos[2]);
//    item.m_Name  = "updatedStartPositions";
//    item.m_Value = list;
//    propList->append(item);
//
//    dimensions[0] = m_FloatLargeDimensions[0] - 125;
//    dimensions[1] = m_FloatLargeDimensions[1] - 250;
//    dimensions[2] = m_FloatLargeDimensions[2] - 375;
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecBrickedVolume->update(m_LoadedFloatLargeMemento);
//
//    // verify the information
//    for (int i = 0; i < 3; ++i) {
//        startPos[i]   = startPos[i] >> 1;
//        dimensions[i] = dimensions[i] >> 1;
//    }
//    verifyFloatLargeMemento(m_LoadedFloatLargeMemento, 2, startPos, dimensions);
//
//    // test case 3: update the dimensions only, and extract the volume data from the loaded volume data in level 2
//    dimensions[0] = m_FloatLargeDimensions[0] - 375;
//    dimensions[1] = m_FloatLargeDimensions[1] - 500;
//    dimensions[2] = m_FloatLargeDimensions[2] - 625;
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecBrickedVolume->update(m_LoadedFloatLargeMemento);
//
//    // verify the information
//    for (int i = 0; i < 3; ++i) {
//        dimensions[i] = dimensions[i] >> 1;
//    }
//    verifyFloatLargeMemento(m_LoadedFloatLargeMemento, 2, startPos, dimensions);
//
//    // test case 4: update the start positions and dimensions, and load the volume data from the disk in level 1
//    startPos[0] = 250;
//    startPos[1] = 375;
//    startPos[2] = 500;
//    list.clear();
//    list.append(startPos[0]);
//    list.append(startPos[1]);
//    list.append(startPos[2]);
//    item.m_Name  = "updatedStartPositions";
//    item.m_Value = list;
//    propList->append(item);
//
//    dimensions[0] = m_FloatLargeDimensions[0] - 400;
//    dimensions[1] = m_FloatLargeDimensions[1] - 525;
//    dimensions[2] = m_FloatLargeDimensions[2] - 650;
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecBrickedVolume->update(m_LoadedFloatLargeMemento);
//
//    // verify the information
//    verifyFloatLargeMemento(m_LoadedFloatLargeMemento, 1, startPos, dimensions);
//
//    // close the float large volume file
//    file.close();
//}

mafMementoPropertyItem mafCodecBrickedVolumeTest::findPropertyItem(mafMemento *menento, QString name){
    mafMementoPropertyList *propList = menento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        if (item.m_Name == name)
            return item;
    }
    mafMementoPropertyItem item;
    return item;
}

void mafCodecBrickedVolumeTest::verifyGrayMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        mafMementoPropertyItem comparedItem = findPropertyItem(m_GrayMemento, item.m_Name);
        if (item.m_Name == "fileName") {
            continue;
        } else if (item.m_Name == "levels") {
            int levels = item.m_Value.toInt();
            QVERIFY(levels == 2);
        } else if (item.m_Name == "currentLevel") {
            int currentLevel = item.m_Value.toInt();
            QVERIFY(currentLevel == level);
        } else if (item.m_Name == "startPositions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)             
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int scale = pow(2.f, level - 1);
            bool dataEqual = true;
            unsigned short *readData     = (unsigned short *)item.m_Value.value<void *>();
            unsigned short *comparedData = (unsigned short *)comparedItem.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                        int comparedIndex = (((startPos[2] + z) * m_GrayDimensions[1] + startPos[1] + y) * m_GrayDimensions[0] + startPos[0] + x) * scale;
                        //QVERIFY(readData[readIndex] == comparedData[comparedIndex]);
                        dataEqual &= (readData[readIndex] == comparedData[comparedIndex]);
                    }
                }
            }
            QVERIFY(dataEqual);
        } else { 
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}

void mafCodecBrickedVolumeTest::verifyRGBMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        mafMementoPropertyItem comparedItem = findPropertyItem(m_RGBMemento, item.m_Name);
        if (item.m_Name == "fileName") {
            continue;
        } else if (item.m_Name == "levels") {
            int levels = item.m_Value.toInt();
            QVERIFY(levels == 3);
        } else if (item.m_Name == "currentLevel") {
            int currentLevel = item.m_Value.toInt();
            QVERIFY(currentLevel == level);
        } else if (item.m_Name == "startPositions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int scale = pow(2.f, level - 1);
            bool dataEqual = true;
            unsigned char *readData     = (unsigned char *)item.m_Value.value<void *>();
            unsigned char *comparedData = (unsigned char *)comparedItem.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        int readIndex     = ((z * dimensions[1] + y) * dimensions[0] + x) * 3;
                        int comparedIndex = (((startPos[2] + z) * m_RGBDimensions[1] + startPos[1] + y) * m_RGBDimensions[0] + startPos[0] + x) * scale * 3;
                        //QVERIFY(readData[readIndex + 0] == comparedData[comparedIndex + 0]);
                        //QVERIFY(readData[readIndex + 1] == comparedData[comparedIndex + 1]);
                        //QVERIFY(readData[readIndex + 2] == comparedData[comparedIndex + 2]);
                        dataEqual &= (readData[readIndex + 0] == comparedData[comparedIndex + 0]);
                        dataEqual &= (readData[readIndex + 1] == comparedData[comparedIndex + 1]);
                        dataEqual &= (readData[readIndex + 2] == comparedData[comparedIndex + 2]);
                    }
                }
            }
            QVERIFY(dataEqual);
        } else {
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}

void mafCodecBrickedVolumeTest::verifyFloatEqualMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        mafMementoPropertyItem comparedItem = findPropertyItem(m_FloatEqualMemento, item.m_Name);
        if (item.m_Name == "fileName") {
            continue;
        } else if (item.m_Name == "levels") {
            int levels = item.m_Value.toInt();
            QVERIFY(levels == 3);
        } else if (item.m_Name == "currentLevel") {
            int currentLevel = item.m_Value.toInt();
            QVERIFY(currentLevel == level);
        } else if (item.m_Name == "startPositions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)             
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int scale = pow(2.f, level - 1);
            bool dataEqual = true;
            float *readData     = (float *)item.m_Value.value<void *>();
            float *comparedData = (float *)comparedItem.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                        int comparedIndex = (((startPos[2] + z) * m_FloatEqualDimensions[1] + startPos[1] + y) * m_FloatEqualDimensions[0] + startPos[0] + x) * scale;
                        //QVERIFY(readData[readIndex] == comparedData[comparedIndex]);
                        dataEqual &= (readData[readIndex] == comparedData[comparedIndex]);
                    }
                }
            }
            QVERIFY(dataEqual);
        } else { 
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}

void mafCodecBrickedVolumeTest::verifyFloatLargeMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        mafMementoPropertyItem comparedItem = findPropertyItem(m_FloatLargeMemento, item.m_Name);
        if (item.m_Name == "fileName" || item.m_Name == "dataHash" || item.m_Name == "memoryLimit") {
            continue;
        } else if (item.m_Name == "levels") {
            int levels = item.m_Value.toInt();
            QVERIFY(levels == 4);
        } else if (item.m_Name == "currentLevel") {
            int currentLevel = item.m_Value.toInt();
            QVERIFY(currentLevel == level);
        } else if (item.m_Name == "startPositions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)             
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            QVariantList list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int    readIndex = 0;
            int    scale     = pow(2.f, level - 1);
            bool   dataEqual = true;
            float *readData  = (float *)item.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        //int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                        int comparedIndex = (((startPos[2] + z) * m_FloatLargeDimensions[1] + startPos[1] + y) * m_FloatLargeDimensions[0] + startPos[0] + x) * scale;
                        float value       = readData[readIndex++];
                        float comparedValue = comparedIndex * 0.01;
                        //QVERIFY(readData[readIndex] == comparedIndex / 100.f);
                        //QVERIFY(value == comparedValue);
                        dataEqual &= (value == comparedValue);
                    }
                }
            }
            QVERIFY(dataEqual);
        } else { 
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}


MAF_REGISTER_TEST(mafCodecBrickedVolumeTest);
#include "mafCodecBrickedVolumeTest.moc"
