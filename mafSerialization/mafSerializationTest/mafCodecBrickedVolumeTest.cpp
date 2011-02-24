/*
 *  mafCodecBrickedVolumeTest.cpp
 *  mafSerializationTest
 *
 *  Created by Yubo Tao on 30/12/10.
 *  Copyright 2010 University of Bedfordshire. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
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

private slots:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafCodecBrickedVolume allocation test case.
    void mafCodecBrickedVolumeAllocationTest();

    /// test the encode method.
    void encodeTest();
    
    /// test the decode method.
    void decodeTest();
    
    /// test the update method.
    void updateTest();

private:
    /// find the memento property item given the name
    mafMementoPropertyItem findPropertyItem(mafMemento *menento, mafString name);

    /// verify the serialization of the Gray Memento
    void verifyGrayMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);
    
    /// verify the serialization of the RGB Memento
    void verifyRGBMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    /// verify the serialization of the float Memento
    void verifyFloatMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    mafCodecBrickedVolume *m_CodecBrickedVolume;///< Test var
    int m_Dimensions[3];                        ///< The dimensions of the test volumes
    mafMemento *m_GrayMemento;                   ///< Test Object (Gray unsigned short).
    mafMemento *m_RGBMemento;                    ///< Test Object (RGB unsigned char).
    mafMemento *m_FloatMemento;                  ///< Test Object (Gray float for testing bricks with equal values)
    mafString  m_GrayFileName;                  ///< The file name of m_GrayMemento
    mafString  m_RGBFileName;                   ///< The file name of m_RGBMemento
    mafString  m_FloatFileName;                 ///< The file name of m_FloatMemento
    mafMemento *m_LoadedGrayMemento;            ///< The loaded memento from m_GrayFileName
    mafMemento *m_LoadedRGBMemento;             ///< The loaded memento from m_RGBFileName
    mafMemento *m_LoadedFloatMemento;           ///< The loaded memento from m_FloatFileName
};

void mafCodecBrickedVolumeTest::initTestCase() {
    mafList<mafVariant> list;
    mafMementoPropertyItem item;
    item.m_Multiplicity = 1;
    m_Dimensions[0] = 71;
    m_Dimensions[1] = 64;
    m_Dimensions[2] = 58;
    m_GrayFileName = "./Gray716458.lod";
    m_RGBFileName  = "./RGB716458.lod";
    m_FloatFileName= "./Equal716458.lod";
    m_LoadedGrayMemento = 0;
    m_LoadedRGBMemento  = 0;
    m_LoadedFloatMemento= 0;

    // init the test variable
    m_CodecBrickedVolume = mafNEW(mafSerialization::mafCodecBrickedVolume);

    m_GrayMemento = new mafMemento();
    // init the volume data (single component, unsigned short)
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
    list.append(m_Dimensions[0]);
    list.append(m_Dimensions[1]);
    list.append(m_Dimensions[2]);
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
    list.append(m_Dimensions[0]);
    list.append(m_Dimensions[1]);
    list.append(m_Dimensions[2]);
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
    unsigned short *data = new unsigned short[m_Dimensions[0] * m_Dimensions[1] * m_Dimensions[2]];
    for (int z = 0; z < m_Dimensions[2]; ++z) {
        for (int y = 0; y < m_Dimensions[1]; ++y) {
            for (int x = 0; x < m_Dimensions[0]; ++x) {
                int index = (z * m_Dimensions[1] + y ) * m_Dimensions[0] + x;
                data[index] = index % 4096;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data);
    propList->append(item);

    m_RGBMemento = new mafMemento();
    // init the volume data (three component, unsigned char)
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
    list.append(m_Dimensions[0]);
    list.append(m_Dimensions[1]);
    list.append(m_Dimensions[2]);
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
    list.append(m_Dimensions[0]);
    list.append(m_Dimensions[1]);
    list.append(m_Dimensions[2]);
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
    unsigned char *data2 = new unsigned char[m_Dimensions[0] * m_Dimensions[1] * m_Dimensions[2] * 3];
    for (int z = 0; z < m_Dimensions[2]; ++z) {
        for (int y = 0; y < m_Dimensions[1]; ++y) {
            for (int x = 0; x < m_Dimensions[0]; ++x) {
                int index = ((z * m_Dimensions[1] + y ) * m_Dimensions[0] + x) * 3;
                data2[index + 0] = z;
                data2[index + 1] = y;
                data2[index + 2] = x;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data2);
    propList->append(item);

    m_FloatMemento = new mafMemento();
    // init the volume data (one component, float) with equal values
    propList = m_FloatMemento->mementoPropertyList();
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
    item.m_Value = m_FloatMemento->objectHash();
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
    list.append(m_Dimensions[0]);
    list.append(m_Dimensions[1]);
    list.append(m_Dimensions[2]);
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
    list.append(m_Dimensions[0]);
    list.append(m_Dimensions[1]);
    list.append(m_Dimensions[2]);
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
    float *data3 = new float[m_Dimensions[0] * m_Dimensions[1] * m_Dimensions[2]];
    for (int z = 0; z < m_Dimensions[2]; ++z) {
        for (int y = 0; y < m_Dimensions[1]; ++y) {
            for (int x = 0; x < m_Dimensions[0]; ++x) {
                int index = (z * m_Dimensions[1] + y ) * m_Dimensions[0] + x;
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
    mafFile::remove(m_GrayFileName);
    
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
    mafFile::remove(m_RGBFileName);

    // release the volume data in m_FloatMemento
    propList = m_FloatMemento->mementoPropertyList();
    iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            float *data = (float *)iter->m_Value.value<void *>();
            delete []data;
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafFile::remove(m_FloatFileName);

    // release the volume data in m_LoadedGrayMemento
    propList = m_LoadedGrayMemento->mementoPropertyList();
    iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            unsigned short *data = (unsigned short *)iter->m_Value.value<void *>();
            delete []data;
        } else if (iter->m_Name == "fileName") {
            mafString fileName = iter->m_Value.toString();
            mafFile::remove(fileName + ".bpi");
            mafFile::remove(fileName + ".bev");
            mafFile::remove(fileName + ".bv");
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafDEL(m_LoadedGrayMemento);

    // release the volume data in m_LoadedRGBMemento
    propList = m_LoadedRGBMemento->mementoPropertyList();
    iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            unsigned char *data = (unsigned char *)iter->m_Value.value<void *>();
            delete []data;
        } else if (iter->m_Name == "fileName") {
            mafString fileName = iter->m_Value.toString();
            mafFile::remove(fileName + ".bpi");
            mafFile::remove(fileName + ".bev");
            mafFile::remove(fileName + ".bv");
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafDEL(m_LoadedRGBMemento);

    // release the volume data in m_LoadedFloatMemento
    propList = m_LoadedFloatMemento->mementoPropertyList();
    iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "dataValue") {
            float *data = (float *)iter->m_Value.value<void *>();
            delete []data;
        } else if (iter->m_Name == "fileName") {
            mafString fileName = iter->m_Value.toString();
            mafFile::remove(fileName + ".bpi");
            mafFile::remove(fileName + ".bev");
            mafFile::remove(fileName + ".bv");
        }
        iter = propList->erase(iter);
    }
    propList->clear();
    mafDEL(m_LoadedFloatMemento);

    mafDEL(m_GrayMemento);
    mafDEL(m_RGBMemento);
    mafDEL(m_FloatMemento);
}

void mafCodecBrickedVolumeTest::mafCodecBrickedVolumeAllocationTest() {
    QVERIFY(m_CodecBrickedVolume != NULL);
}

void mafCodecBrickedVolumeTest::encodeTest() {
    // test case 1
    // open the gray file in writing modality
    mafFile file(m_GrayFileName);
    bool success = file.open(mafIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecBrickedVolume->encode(m_GrayMemento);

    // close the gray file
    file.close();

    QVERIFY(mafFile::exists(m_GrayFileName));

    // test case 2
    // open the RGB file in writing modality
    file.setFileName(m_RGBFileName);
    success = file.open(mafIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecBrickedVolume->encode(m_RGBMemento);

    // close the RGB file
    file.close();

    QVERIFY(mafFile::exists(m_RGBFileName));

    // test case 3
    // open the float file in writing modality
    file.setFileName(m_FloatFileName);
    success = file.open(mafIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecBrickedVolume->encode(m_FloatMemento);

    // close the float file
    file.close();

    QVERIFY(mafFile::exists(m_FloatFileName));
}

void mafCodecBrickedVolumeTest::decodeTest() {
    // test case 1
    // open the gray file in reading modality
    mafFile file(m_GrayFileName);
    bool success = file.open(mafIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // decode the information 
    m_LoadedGrayMemento = m_CodecBrickedVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    QVERIFY(m_LoadedGrayMemento != 0);
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { m_Dimensions[0] >> 1, m_Dimensions[1] >> 1, m_Dimensions[2] >> 1 };
    verifyGrayMemento(m_LoadedGrayMemento, 2, startPos, dimensions);

    // test case 2
    // open the RGB file in reading modality
    file.setFileName(m_RGBFileName);
    success = file.open(mafIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // decode the information 
    m_LoadedRGBMemento = m_CodecBrickedVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    QVERIFY(m_LoadedRGBMemento != 0);
    dimensions[0] = m_Dimensions[0] >> 2;
    dimensions[1] = m_Dimensions[1] >> 2;
    dimensions[2] = m_Dimensions[2] >> 2;
    verifyRGBMemento(m_LoadedRGBMemento, 3, startPos, dimensions);

    // test case 3
    // open the float file in reading modality
    file.setFileName(m_FloatFileName);
    success = file.open(mafIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // decode the information 
    m_LoadedFloatMemento = m_CodecBrickedVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    QVERIFY(m_LoadedFloatMemento != 0);
    dimensions[0] = m_Dimensions[0] >> 2;
    dimensions[1] = m_Dimensions[1] >> 2;
    dimensions[2] = m_Dimensions[2] >> 2;
    verifyFloatMemento(m_LoadedFloatMemento, 3, startPos, dimensions);
}

void mafCodecBrickedVolumeTest::updateTest() {
    mafList<mafVariant> list;
    mafMementoPropertyItem item;
    mafMementoPropertyList *propList = 0;
    int startPos[3], dimensions[3];

    // open the gray file in reading modality
    mafFile file(m_GrayFileName);
    bool success = file.open(mafIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // test case 1: update the start positions and dimensions, and extract volume data from the loaded volume data in level 2
    propList = m_LoadedGrayMemento->mementoPropertyList();
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

    dimensions[0] = m_Dimensions[0] - 20;
    dimensions[1] = m_Dimensions[1] - 15;
    dimensions[2] = m_Dimensions[2] - 10;
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

    // test case 2: update the start positions only, and load the volume data from the disk in level 2
    propList = m_LoadedGrayMemento->mementoPropertyList();
    startPos[0]   = 4;
    startPos[1]   = 7;
    startPos[2]   = 9;
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

    // test case 3: update the start positions and dimensions, and load the volume data from the disk in level 1 
    propList = m_LoadedGrayMemento->mementoPropertyList();
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

    dimensions[0] = m_Dimensions[0] - 40;
    dimensions[1] = m_Dimensions[1] - 35;
    dimensions[2] = m_Dimensions[2] - 30;
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

    // test case 4: update the dimensions only, and extract the volume data from the loaded volume data in level 1 
    propList = m_LoadedGrayMemento->mementoPropertyList();
    dimensions[0] = m_Dimensions[0] - 40;
    dimensions[1] = m_Dimensions[1] - 30;
    dimensions[2] = m_Dimensions[2] - 40;
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

    // open the RGB file in reading modality
    file.setFileName(m_RGBFileName);
    success = file.open(mafIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // test case 5: update the start positions and dimensions, and load the volume data from the disk in level 2
    propList = m_LoadedRGBMemento->mementoPropertyList();
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

    dimensions[0] = m_Dimensions[0] - 15;
    dimensions[1] = m_Dimensions[1] - 13;
    dimensions[2] = m_Dimensions[2] - 11;
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

    // test case 6: update the start positions only, and load the volume data from the disk in level 2
    propList = m_LoadedRGBMemento->mementoPropertyList();
    startPos[0]   = 4;
    startPos[1]   = 7;
    startPos[2]   = 9;
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

    // test case 7: update the start positions and dimensions, and load the volume data from the disk in level 1 
    propList = m_LoadedRGBMemento->mementoPropertyList();
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

    dimensions[0] = m_Dimensions[0] - 40;
    dimensions[1] = m_Dimensions[1] - 35;
    dimensions[2] = m_Dimensions[2] - 35;
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

    // test case 8: update the dimensions only, and extract the volume data from the loaded volume data in level 1
    propList = m_LoadedRGBMemento->mementoPropertyList();
    dimensions[0] = m_Dimensions[0] - 40;
    dimensions[1] = m_Dimensions[1] - 38;
    dimensions[2] = m_Dimensions[2] - 35;
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

    // open the float file in reading modality
    file.setFileName(m_FloatFileName);
    success = file.open(mafIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecBrickedVolume->setDevice(&file);

    // test case 9: update the start positions and dimensions, and load the volume data from the disk in level 2
    propList = m_LoadedFloatMemento->mementoPropertyList();
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

    dimensions[0] = m_Dimensions[0] - 18;
    dimensions[1] = m_Dimensions[1] - 15;
    dimensions[2] = m_Dimensions[2] - 12;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedFloatMemento);

    // verify the information
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i] >> 1;
        dimensions[i] = dimensions[i] >> 1;
    }
    verifyFloatMemento(m_LoadedFloatMemento, 2, startPos, dimensions);

    // test case 10: update the dimensions only, and extract the volume data from the loaded volume data in level 1
    propList = m_LoadedFloatMemento->mementoPropertyList();
    dimensions[0] = m_Dimensions[0] - 40;
    dimensions[1] = m_Dimensions[1] - 39;
    dimensions[2] = m_Dimensions[2] - 38;
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name  = "updatedDimensions";
    item.m_Value = list;
    propList->append(item);

    m_CodecBrickedVolume->update(m_LoadedFloatMemento);

    // verify the information
    for (int i = 0; i < 3; ++i)
        startPos[i] = startPos[i] << 1;
    verifyFloatMemento(m_LoadedFloatMemento, 1, startPos, dimensions);
    
    // close the float file
    file.close();
}

mafMementoPropertyItem mafCodecBrickedVolumeTest::findPropertyItem(mafMemento *menento, mafString name){
    mafMementoPropertyList *propList = menento->mementoPropertyList();
    foreach(mafMementoPropertyItem item, *propList) {
        if (item.m_Name == name)
            return item;
    }
    mafMementoPropertyItem item;
    return item;
}

void mafCodecBrickedVolumeTest::verifyGrayMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    foreach(mafMementoPropertyItem item, *propList) {
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
            mafList<mafVariant> list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)             
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            mafList<mafVariant> list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int scale = pow(2.f, level - 1);
            unsigned short *readData     = (unsigned short *)item.m_Value.value<void *>();
            unsigned short *comparedData = (unsigned short *)comparedItem.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                        int comparedIndex = (((startPos[2] + z) * m_Dimensions[1] + startPos[1] + y) * m_Dimensions[0] + startPos[0] + x) * scale;
                        QVERIFY(readData[readIndex] == comparedData[comparedIndex]);
                    }
                }
            }
        } else { 
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}

void mafCodecBrickedVolumeTest::verifyRGBMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    foreach(mafMementoPropertyItem item, *propList) {
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
            mafList<mafVariant> list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            mafList<mafVariant> list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int scale = pow(2.f, level - 1);
            unsigned char *readData     = (unsigned char *)item.m_Value.value<void *>();
            unsigned char *comparedData = (unsigned char *)comparedItem.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        int readIndex     = ((z * dimensions[1] + y) * dimensions[0] + x) * 3;
                        int comparedIndex = (((startPos[2] + z) * m_Dimensions[1] + startPos[1] + y) * m_Dimensions[0] + startPos[0] + x) * scale * 3;
                        QVERIFY(readData[readIndex + 0] == comparedData[comparedIndex + 0]);
                        QVERIFY(readData[readIndex + 1] == comparedData[comparedIndex + 1]);
                        QVERIFY(readData[readIndex + 2] == comparedData[comparedIndex + 2]);
                    }
                }
            }
        } else {
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}

void mafCodecBrickedVolumeTest::verifyFloatMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    foreach(mafMementoPropertyItem item, *propList) {
        mafMementoPropertyItem comparedItem = findPropertyItem(m_FloatMemento, item.m_Name);
        if (item.m_Name == "fileName") {
            continue;
        } else if (item.m_Name == "levels") {
            int levels = item.m_Value.toInt();
            QVERIFY(levels == 3);
        } else if (item.m_Name == "currentLevel") {
            int currentLevel = item.m_Value.toInt();
            QVERIFY(currentLevel == level);
        } else if (item.m_Name == "startPositions") {
            mafList<mafVariant> list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)             
                QVERIFY(list[i].toInt() == startPos[i]);
        } else if (item.m_Name == "dimensions") {
            mafList<mafVariant> list = item.m_Value.toList();
            QVERIFY(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                QVERIFY(list[i].toInt() == dimensions[i]);
        } else if (item.m_Name == "dataValue") {
            int scale = pow(2.f, level - 1);
            float *readData     = (float *)item.m_Value.value<void *>();
            float *comparedData = (float *)comparedItem.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                        int comparedIndex = (((startPos[2] + z) * m_Dimensions[1] + startPos[1] + y) * m_Dimensions[0] + startPos[0] + x) * scale;
                        QVERIFY(readData[readIndex] == comparedData[comparedIndex]);
                    }
                }
            }
        } else { 
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            mafString a = item.m_Value.toString();
            mafString b = comparedItem.m_Value.toString();
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}


MAF_REGISTER_TEST(mafCodecBrickedVolumeTest);
#include "mafCodecBrickedVolumeTest.moc"

