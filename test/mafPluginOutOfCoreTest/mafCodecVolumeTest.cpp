/*
 *  mafCodecVolumeTest.cpp
 *  mafSerializationTest
 *
 *  Created by Yubo Tao on 28/12/10.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCodecVolume.h>
#include <mafObject.h>
#include <mafMementoObject.h>

using namespace mafCore;
using namespace mafSerialization;

/**
 Class name: mafCodecVolumeTest
 This class implements the test suite for mafCodecVolume.
 */

//! <title>
//mafCodecVolume
//! </title>
//! <description>
//mafCodecVolume is a codec to Encode/Decode maf volume objects into a raw memory dump using multiresolution.
//! </description>

class mafCodecVolumeTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafCodecVolume allocation test case.
    void mafCodecVolumeAllocationTest();

    /// test the encode method.
    void encodeTest();
    
    /// test the decode method using the Gray memento.
    void decodeGrayMementoTest();

    /// test the decode method using the RGB memento.
    void decodeRGBMementoTest();

    /// test the decode method using the Float memento.
    //void decodeFloatMementoTest();
    
    /// test the update method using the Gray memento.
    void updateGrayMementoTest();

    /// test the update method using the RGB memento.
    void updateRGBMementoTest();

    /// test the update method using the Float memento.
    //void updateFloatMementoTest();

private:
    /// find the memento property item given the name
    mafMementoPropertyItem findPropertyItem(mafMemento *menento, QString name);

    /// verify the serialization of the Gray Memento
    void verifyGrayMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);
    
    /// verify the serialization of the RGB Memento
    void verifyRGBMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    /// verify the serialization of the Float Memento
    void verifyFloatMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]);

    mafCodecVolume *m_CodecVolume;          ///< Test var
    int m_GrayDimensions[3];                ///< The dimensions of the Gray volume
    int m_RGBDimensions[3];                 ///< The dimension of the RGB volume
    int m_FloatDimensions[3];               ///< The dimension of the float volume
    QString m_GrayFileName;                 ///< The file name of m_GrayMemento
    QString m_RGBFileName;                  ///< The file name of m_RGBMenmento
    QString m_FloatFileName;                ///< The file name of m_FloatMenmento
    mafMemento *m_GrayMemento;              ///< Test volume object (Gray unsigned short)
    mafMemento *m_RGBMemento;               ///< Test volume object (RGB unsigned char)
    mafMemento *m_FloatMemento;             ///< Test volume object (float)
    mafMemento *m_LoadedGrayMemento;        ///< The loaded memento from m_GrayFileName
    mafMemento *m_LoadedRGBMemento;         ///< The loaded memento from m_RGBFileName
    mafMemento *m_LoadedFloatMemento;       ///< The loaded memento from m_FloatFileName
};

void mafCodecVolumeTest::initTestCase() {
    QVariantList list;
    mafMementoPropertyItem item;
    item.m_Multiplicity  = 1;
    m_GrayDimensions[0]  = 71;
    m_GrayDimensions[1]  = 64;
    m_GrayDimensions[2]  = 58;
    m_RGBDimensions[0]   = 71;
    m_RGBDimensions[1]   = 64;
    m_RGBDimensions[2]   = 58;
    m_FloatDimensions[0] = 1024;
    m_FloatDimensions[1] = 1024;
    m_FloatDimensions[2] = 1024;
    m_GrayFileName       = "./Gray716458.lod";
    m_RGBFileName        = "./RGB716458.lod";
    m_FloatFileName      = "./Float1024.lod";
    m_LoadedGrayMemento  = 0;
    m_LoadedRGBMemento   = 0;
    m_LoadedFloatMemento = 0;

    // init the test variable
    m_CodecVolume = mafNEW(mafSerialization::mafCodecVolume);

    // 1.Init the volume data (single component, unsigned short)
    m_GrayMemento = new mafMemento();
    mafMementoPropertyList *propList = m_GrayMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)281210;
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
                int index = (z * m_GrayDimensions[1] + y ) * m_GrayDimensions[0] + x;
                data[index] = index % 4096;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data);
    propList->append(item);

    // 2.Init the volume data (three component, unsigned char)
    m_RGBMemento = new mafMemento();
    propList = m_RGBMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)281210;
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
                int index = ((z * m_RGBDimensions[1] + y ) * m_RGBDimensions[0] + x) * 3;
                data2[index + 0] = z;
                data2[index + 1] = y;
                data2[index + 2] = x;
            }
        }
    }
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data2);
    propList->append(item);

    // 3.Init the volume data (one component, float)    
    m_FloatMemento = new mafMemento();
    propList = m_FloatMemento->mementoPropertyList();
    // codecType
    item.m_Name  = "codecType";
    item.m_Value = "RAW_LOD";
    propList->append(item);
    // mafDataSetTime
    item.m_Name  = "mafDataSetTime";
    item.m_Value = (double)010411;
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
    list.append(m_FloatDimensions[0]);
    list.append(m_FloatDimensions[1]);
    list.append(m_FloatDimensions[2]);
    item.m_Name  = "dimensions";
    item.m_Value = list;
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
    list.append(m_FloatDimensions[0]);
    list.append(m_FloatDimensions[1]);
    list.append(m_FloatDimensions[2]);
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
    float *data3 = 0;   // we could not generate a 4G volume in the memory
    item.m_Name  = "dataValue";
    item.m_Value = qVariantFromValue((void *)data3);
    propList->append(item);
}

void mafCodecVolumeTest::cleanupTestCase() {
    mafDEL(m_CodecVolume);
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

    // release m_FloatMemento
    propList = m_FloatMemento->mementoPropertyList();
    propList->clear();
    mafDEL(m_FloatMemento);

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
                QFile::remove(fileName);
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
                QFile::remove(fileName);
            }
            iter = propList->erase(iter);
        }
        propList->clear();
        mafDEL(m_LoadedRGBMemento);
    }

    // release the volume data in m_LoadedFloatMemento
    if (m_LoadedFloatMemento) {
        propList = m_LoadedFloatMemento->mementoPropertyList();
        iter = propList->begin();
        while (iter != propList->end()) {
            if (iter->m_Name == "dataValue") {
                float *data = (float *)iter->m_Value.value<void *>();
                delete []data;
            }
            iter = propList->erase(iter);
        }
        propList->clear();
        mafDEL(m_LoadedFloatMemento);
    }
}

void mafCodecVolumeTest::mafCodecVolumeAllocationTest() {
    QVERIFY(m_CodecVolume);
}

void mafCodecVolumeTest::encodeTest() {
    // test case 1
    // open the gray file in writing modality
    QFile file(m_GrayFileName);
    bool success = file.open(QIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecVolume->encode(m_GrayMemento);

    // close the gray file
    file.close();

    QVERIFY(QFile::exists(m_GrayFileName));

    // test case 2
    // open the RGB file in writing modality
    file.setFileName(m_RGBFileName);
    success = file.open(QIODevice::WriteOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecVolume->setDevice(&file);

    // give the memento to the codec
    m_CodecVolume->encode(m_RGBMemento);

    // close the RGB file
    file.close();

    QVERIFY(QFile::exists(m_RGBFileName));

    // test case 3
    //// open the Float file in writing modality
    //file.setFileName(m_FloatFileName);
    //success = file.open(QIODevice::WriteOnly);
    //QVERIFY(success);

    //// assign the IODevice
    //m_CodecVolume->setDevice(&file);

    //// give the memento to the codec
    //m_CodecVolume->encode(m_FloatMemento);

    //// close the Float file
    //file.close();

    //QVERIFY(QFile::exists(m_FloatFileName));
}

void mafCodecVolumeTest::decodeGrayMementoTest() {
    // open the gray file in reading modality
    QFile file(m_GrayFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecVolume->setDevice(&file);

    // decode the information 
    m_LoadedGrayMemento = m_CodecVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { 0, 0, 0 };
    QVERIFY(m_LoadedGrayMemento != 0);
    verifyGrayMemento(m_LoadedGrayMemento, -1, startPos, dimensions);
}

void mafCodecVolumeTest::decodeRGBMementoTest() {
    // open the RGB file in reading modality
    QFile file(m_RGBFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecVolume->setDevice(&file);

    // decode the information 
    m_LoadedRGBMemento = m_CodecVolume->decode();

    // close the gray file
    file.close();

    // verify the information
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { 0, 0, 0 };
    QVERIFY(m_LoadedGrayMemento != 0);
    verifyRGBMemento(m_LoadedRGBMemento, -1, startPos, dimensions);
}

//void mafCodecVolumeTest::decodeFloatMementoTest() {
//    // open the Float file in reading modality
//    QFile file(m_FloatFileName);
//    bool success = file.open(QIODevice::ReadOnly);
//    QVERIFY(success);
//
//    // assign the IODevice
//    m_CodecVolume->setDevice(&file);
//
//    // decode the information
//    m_LoadedFloatMemento = m_CodecVolume->decode();
//
//    // close the float file
//    file.close();
//
//    // verify the information
//    int startPos[3]   = { 0, 0, 0 };
//    int dimensions[3] = { 0, 0, 0 };
//    QVERIFY(m_LoadedFloatMemento != 0);
//    verifyFloatMemento(m_LoadedFloatMemento, -1, startPos, dimensions);
//}

void mafCodecVolumeTest::updateGrayMementoTest() {
    QVariantList list;
    mafMementoPropertyItem item;
    int startPos[3], dimensions[3];

    // open the gray file in reading modality
    QFile file(m_GrayFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecVolume->setDevice(&file);

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

    m_CodecVolume->update(m_LoadedGrayMemento);
    
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

    m_CodecVolume->update(m_LoadedGrayMemento);
    
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

    m_CodecVolume->update(m_LoadedGrayMemento);

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

    m_CodecVolume->update(m_LoadedGrayMemento);

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

    m_CodecVolume->update(m_LoadedGrayMemento);

    // verify the information
    verifyGrayMemento(m_LoadedGrayMemento, 1, startPos, dimensions);

    // close the gray file
    file.close();
}

void mafCodecVolumeTest::updateRGBMementoTest() {
    QVariantList list;
    mafMementoPropertyItem item;
    int startPos[3], dimensions[3];

    // open the RGB file in reading modality
    QFile file(m_RGBFileName);
    bool success = file.open(QIODevice::ReadOnly);
    QVERIFY(success);

    // assign the IODevice
    m_CodecVolume->setDevice(&file);

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

    m_CodecVolume->update(m_LoadedRGBMemento);

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

    m_CodecVolume->update(m_LoadedRGBMemento);

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

    m_CodecVolume->update(m_LoadedRGBMemento);

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

    m_CodecVolume->update(m_LoadedRGBMemento);

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

    m_CodecVolume->update(m_LoadedRGBMemento);

    // verify the information
    verifyRGBMemento(m_LoadedRGBMemento, 1, startPos, dimensions);

    // close the RGB file
    file.close();
}

//void mafCodecVolumeTest::updateFloatMementoTest() {
//    QVariantList list;
//    mafMementoPropertyItem item;
//    int startPos[3], dimensions[3];
//
//    // open the Float file in reading modality
//    QFile file(m_FloatFileName);
//    bool success = file.open(QIODevice::ReadOnly);
//    QVERIFY(success);
//
//    // assign the IODevice
//    m_CodecVolume->setDevice(&file);
//
//    // property list
//    mafMementoPropertyList *propList = m_LoadedFloatMemento->mementoPropertyList();
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
//    dimensions[0] = m_FloatDimensions[0];
//    dimensions[1] = m_FloatDimensions[1];
//    dimensions[2] = m_FloatDimensions[2];
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecVolume->update(m_LoadedFloatMemento);
//
//    // verify the information
//    for (int i = 0; i < 3; ++i) {
//        startPos[i]   = startPos[i] >> 3;
//        dimensions[i] = dimensions[i] >> 3;
//    }
//    verifyFloatMemento(m_LoadedFloatMemento, 4, startPos, dimensions);
//
//    // update the memory limit in the float memento for test case 2 - 4
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
//    dimensions[0] = m_FloatDimensions[0] - 125;
//    dimensions[1] = m_FloatDimensions[1] - 250;
//    dimensions[2] = m_FloatDimensions[2] - 375;
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecVolume->update(m_LoadedFloatMemento);
//
//    // verify the information
//    for (int i = 0; i < 3; ++i) {
//        startPos[i]   = startPos[i] >> 1;
//        dimensions[i] = dimensions[i] >> 1;
//    }
//    verifyFloatMemento(m_LoadedFloatMemento, 2, startPos, dimensions);
//
//    // test case 3: update the dimensions only, and extract the volume data from the loaded volume data in level 2
//    dimensions[0] = m_FloatDimensions[0] - 375;
//    dimensions[1] = m_FloatDimensions[1] - 500;
//    dimensions[2] = m_FloatDimensions[2] - 625;
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecVolume->update(m_LoadedFloatMemento);
//
//    // verify the information
//    for (int i = 0; i < 3; ++i) {
//        dimensions[i] = dimensions[i] >> 1;
//    }
//    verifyFloatMemento(m_LoadedFloatMemento, 2, startPos, dimensions);
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
//    dimensions[0] = m_FloatDimensions[0] - 400;
//    dimensions[1] = m_FloatDimensions[1] - 525;
//    dimensions[2] = m_FloatDimensions[2] - 650;
//    list.clear();
//    list.append(dimensions[0]);
//    list.append(dimensions[1]);
//    list.append(dimensions[2]);
//    item.m_Name  = "updatedDimensions";
//    item.m_Value = list;
//    propList->append(item);
//
//    m_CodecVolume->update(m_LoadedFloatMemento);
//
//    // verify the information
//    verifyFloatMemento(m_LoadedFloatMemento, 1, startPos, dimensions);
//
//    // close the Float file
//    file.close();
//}

mafMementoPropertyItem mafCodecVolumeTest::findPropertyItem(mafMemento *menento, QString name){
    mafMementoPropertyList *propList = menento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        if (item.m_Name == name)
            return item;
    }
    mafMementoPropertyItem item;
    return item;
}

void mafCodecVolumeTest::verifyGrayMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
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

void mafCodecVolumeTest::verifyRGBMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
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

void mafCodecVolumeTest::verifyFloatMemento(mafMemento *memento, int level, int startPos[3], int dimensions[3]) {
    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        mafMementoPropertyItem comparedItem = findPropertyItem(m_FloatMemento, item.m_Name);
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
            int   readIndex = 0;
            int   scale     = pow(2.f, level - 1);
            bool  dataEqual = true;
            float *readData = (float *)item.m_Value.value<void *>();
            for (int z = 0; z < dimensions[2]; ++z) {
                int comparedIndex = (((startPos[2] + z) * m_FloatDimensions[1] + startPos[1]) * m_FloatDimensions[0] + startPos[0]) * scale;
                for (int y = 0; y < dimensions[1]; ++y) {
                    for (int x = 0; x < dimensions[0]; ++x) {
                        //int readIndex     = (z * dimensions[1] + y) * dimensions[0] + x;
                        //int comparedIndex = (((startPos[2] + z) * m_FloatDimensions[1] + startPos[1] + y) * m_FloatDimensions[0] + startPos[0] + x) * scale;
                        float value         = readData[readIndex++];
                        float comparedValue = comparedIndex * 0.01;
                        //QVERIFY(value == comparedValue);
                        dataEqual &= (value == comparedValue);
                        comparedIndex += scale;
                    }
                    comparedIndex += (m_FloatDimensions[0] - dimensions[0]) * scale;
                }
            }
            QVERIFY(dataEqual);
        } else { 
            QVERIFY2(item.m_Name == comparedItem.m_Name, item.m_Name.toAscii().data());
            QVERIFY2(item.m_Value == comparedItem.m_Value, item.m_Name.toAscii().data());
        }
    }
}


MAF_REGISTER_TEST(mafCodecVolumeTest);
#include "mafCodecVolumeTest.moc"
