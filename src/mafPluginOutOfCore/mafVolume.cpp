/*
 *  mafVolume.cpp
 *  mafResources
 *
 *  Created by Yubo Tao on 24/10/11.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafVolume.h"
#include "mafMementoVolume.h"
#include "mafProxy.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginOutOfCore;

mafVolume::mafVolume(const QString code_location) : mafObject(code_location) {
    m_DataValue             = NULL;
    
    m_DataType              = mafVolUndefined;
    m_BitsStored            = 0;
    m_BigEndian             = false;
    m_ComponentNum          = 1;
    m_OriginalDimensions[0] = 0;
    m_OriginalDimensions[1] = 0;
    m_OriginalDimensions[2] = 0;
    m_Spacing[0]            = 1.f;
    m_Spacing[1]            = 1.f;
    m_Spacing[2]            = 1.f;
    m_Matrix                = NULL;

    m_StartPositions[0]     = 0;
    m_StartPositions[1]     = 0;
    m_StartPositions[2]     = 0;
    m_Dimensions[0]         = 0;
    m_Dimensions[1]         = 0;
    m_Dimensions[2]         = 0;
    m_Level                 = 0;

    m_UpdatedStartPos[0]    = 0;
    m_UpdatedStartPos[1]    = 0;
    m_UpdatedStartPos[2]    = 0;
    m_UpdatedDimensions[0]  = 0;
    m_UpdatedDimensions[1]  = 0;
    m_UpdatedDimensions[2]  = 0;
    
    m_CodecType             = "VOLUME_LOD";
    m_LevelNum              = 0;
    m_Bricking              = false;
    m_BrickSize             = 16;
    m_MemoryLimit           = 1024;   // 1M
}

mafVolume::~mafVolume() {
    delete []m_DataValue;
    m_DataValue = NULL;
    
    delete m_Matrix;
    m_Matrix = NULL;
}

mafCore::mafMemento *mafVolume::createMemento() const {
    mafMemento *m = Superclass::createMemento();
    mafMementoVolume *mementoVolume = new mafMementoVolume(this, mafCodeLocation);
    m->setParent(mementoVolume);

    return mementoVolume;
}

void mafVolume::setMemento(mafMemento *memento, bool deep_memento) {
    // Design by contract condition.
    REQUIRE(memento != NULL);
    REQUIRE(memento->objectClassType() == this->metaObject()->className());

    int childrenNum = memento->children().size();
    for (int i = 0; i < childrenNum; ++i) {
        mafMemento *mementoChild = (mafMemento *)memento->children().at(i);
        if (mementoChild->serializationPattern() == mafSerializationPatternInheritance) {
            //set the memento of the superclass
            Superclass::setMemento(mementoChild, true);
          } else {
            //set the memento of the children memento
            QString objClassType = mementoChild->objectClassType();
            mafCore::mafObjectBase *objBase = mafNEWFromString(objClassType);
            mafCore::mafObject *obj = qobject_cast<mafCore::mafObject *>(objBase);
            obj->setMemento(mementoChild, deep_memento);
            mafDEL(objBase);
          }
    }

    // save the properties of the volume
    mafMementoPropertyList *list = memento->mementoPropertyList();
    mafMementoPropertyItem item;
    Q_FOREACH(item, *list) {
        if (item.m_Name == "dataType") {
            m_DataType = static_cast<mafDataType>(item.m_Value.toInt()); 
        } else if (item.m_Name == "bitsStored") {
            m_BitsStored = item.m_Value.toInt();
        } else if (item.m_Name == "bigEndian") {
            m_BigEndian = item.m_Value.toBool();
        } else if (item.m_Name == "componentNum") {
            m_ComponentNum = item.m_Value.toInt();
        } else if (item.m_Name == "dimensions") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                m_OriginalDimensions[i] = list[i].toInt();
        } else if (item.m_Name == "spacing") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                m_Spacing[i] = list[i].toFloat();
        } else if (item.m_Name == "poseMatrix") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 16);
            mafMatrix matrix;
            for (int r = 0; r < 4; ++r) {
                for (int c = 0; c < 4; ++c) {
                    matrix.setElement(r, c, list[r * 4 + c].toFloat());
                }
            }
            setPoseMatrix(&matrix);
        } else if (item.m_Name == "levels") {
            m_LevelNum = item.m_Value.toInt();
        } else if (item.m_Name == "brickSize") {
            m_Bricking  = true;
            m_BrickSize = item.m_Value.toInt();
        } else if (item.m_Name == "fileName") {
            //Save informations about external file, and load data later, when the data is needed.
            m_FileName = item.m_Value.toString();
        } else if (item.m_Name == "encodeType") {
            m_CodecType = item.m_Value.toString();
        } else {
           qDebug() << "***** Unknow Property \"" << item.m_Name << "\" in mafVolume.*****";  
        }
    }

    // initialize other properties for data loading
    m_StartPositions[0]    = 0;
    m_StartPositions[1]    = 0;
    m_StartPositions[2]    = 0;
    m_Dimensions[0]        = 0;
    m_Dimensions[1]        = 0;
    m_Dimensions[2]        = 0;
    m_Level                = 0;
    m_UpdatedStartPos[0]   = 0;
    m_UpdatedStartPos[1]   = 0;
    m_UpdatedStartPos[2]   = 0;
    m_UpdatedDimensions[0] = m_OriginalDimensions[0];
    m_UpdatedDimensions[1] = m_OriginalDimensions[1];
    m_UpdatedDimensions[2] = m_OriginalDimensions[2];
    setDataValue(0);

    setModified(false);
}

void mafVolume::updateDataValue() {
    if (!m_FileName.isEmpty()) {
        mafProxy<mafVolume> container;
        container = (mafVolume*)this;

        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafProxyInterface *, &container));
        argList.append(mafEventArgument(QString, m_FileName));
        argList.append(mafEventArgument(QString, m_CodecType));
        mafEventBusManager::instance()->notifyEvent("maf.local.serialization.update", mafEventTypeLocal, &argList);
    }
}

int mafVolume::bytesPerVoxel() const {
    int byteNum = 0;
    switch(m_DataType) {
        case mafVolUnsignedChar:
        case mafVolChar:
            byteNum = 1;
            break;
        case mafVolUnsignedShort:
        case mafVolShort:
            byteNum = 2;
            break;
        case mafVolUnsignedInt:
        case mafVolInt:
        case mafVolFloat:
            byteNum = 4;
            break;
        case mafVolDouble:
            byteNum = sizeof(double);
            break;
        default:
            qCritical("Unknown data type.\n");

    }
    return byteNum * m_ComponentNum;
}

int mafVolume::calcMultiresolutionLevel(bool update) {
    int levels = 1;
    int xSize = update ? m_UpdatedDimensions[0] : m_Dimensions[0];
    int ySize = update ? m_UpdatedDimensions[1] : m_Dimensions[1];
    int zSize = update ? m_UpdatedDimensions[2] : m_Dimensions[2];
    qint64 bytesPerVoxel = getByteNum(m_DataType) * m_ComponentNum;
    qint64 totalBytes    = bytesPerVoxel * xSize * ySize * zSize / 1024;
    // reduce the dimensions by half until the size is not larger than the memory limit 
    while (totalBytes > m_MemoryLimit) {
        xSize = xSize >> 1;
        ySize = ySize >> 1;
        zSize = zSize >> 1;
        totalBytes = xSize * ySize * zSize * bytesPerVoxel / 1024;
        ++levels;
    }
    return levels;
}

template<typename T>
void resampleVolume(T *originalData, T *resampledData, int componentNum, int orginalDimensions[3], int dimensions[3], int scale[3]) {
    // resample using nearest neighbor interpolation, need to support linear interpolation
    for (int z = 0; z < dimensions[2]; ++z) {
        for (int y = 0; y < dimensions[1]; ++y) {
            for (int x = 0; x < dimensions[0]; ++x) {
                qint64 originalIndex  = ((z * scale[2] * orginalDimensions[1] + y * scale[1]) * orginalDimensions[0] + x * scale[0]) * componentNum ;
                qint64 resampledIndex = ((z * dimensions[1] + y) * dimensions[0] + x) * componentNum;
                for(int i = 0; i < componentNum; ++i)
                    resampledData[resampledIndex + i] = originalData[originalIndex + i];
            }
        }
    }
}

void mafVolume::resample(void *resampledData, int dimensions[3])
{
    // calculate the scale
    int scale[3] = { m_Dimensions[0] / dimensions[0], m_Dimensions[1] / dimensions[1], m_Dimensions[2] / dimensions[2] };

    // resample the volume data depending on the data type
    switch(m_DataType) {
        case mafVolUnsignedChar:
            resampleVolume<unsigned char>((unsigned char*)m_DataValue, (unsigned char*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolChar:
            resampleVolume<char>((char*)m_DataValue, (char*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolUnsignedShort:
            resampleVolume<unsigned short>((unsigned short*)m_DataValue, (unsigned short*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolShort:
            resampleVolume<short>((short*)m_DataValue, (short*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolUnsignedInt:
            resampleVolume<unsigned int>((unsigned int*)m_DataValue, (unsigned int*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolInt:
            resampleVolume<int>((int*)m_DataValue, (int*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolFloat:
            resampleVolume<float>((float*)m_DataValue, (float*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        case mafVolDouble:
            resampleVolume<double>((double*)m_DataValue, (double*)resampledData, m_ComponentNum, m_Dimensions, dimensions, scale);
            break;
        default:
            qCritical("Unknown data type.\n");
    }
}

bool mafVolume::updateDataValueRequired() {
    int updatedStartPos[3]   = { m_UpdatedStartPos[0],   m_UpdatedStartPos[1],   m_UpdatedStartPos[2]   };
    int updatedDimensions[3] = { m_UpdatedDimensions[0], m_UpdatedDimensions[1], m_UpdatedDimensions[2] };
    
    // determine the level of the data to decode
    int level = calcMultiresolutionLevel(true);
    if (level > m_LevelNum) level = m_LevelNum; // bug: may be out of memory
    // scale the start position and dimension to the current level
    for (int i = 0; i < 3; ++i) {
        updatedStartPos[i]   = updatedStartPos[i]   >> (level - 1);
        updatedDimensions[i] = updatedDimensions[i] >> (level - 1);
    }

    // check whether the start positions and the dimensions are changed
    bool startPosEqual   = true;
    bool dimensionsEqual = true;
    for (int i = 0; i < 3; ++i) {
        startPosEqual   &= (m_StartPositions[i] == updatedStartPos[i]);
        dimensionsEqual &= (m_Dimensions[i]     == updatedDimensions[i]);
    }
    // check whether need to update
    return (level != m_Level || !startPosEqual || !dimensionsEqual);
}

bool mafVolume::isPartOf(int updatedStartPos[3], int updatedDimensions[3]) const {
    return ((updatedStartPos[0] >= m_StartPositions[0]) &&
            (updatedStartPos[1] >= m_StartPositions[1]) &&
            (updatedStartPos[2] >= m_StartPositions[2]) &&
            (updatedStartPos[0] + updatedDimensions[0] <= m_StartPositions[0] + m_Dimensions[0]) &&
            (updatedStartPos[1] + updatedDimensions[1] <= m_StartPositions[1] + m_Dimensions[1]) &&
            (updatedStartPos[2] + updatedDimensions[2] <= m_StartPositions[2] + m_Dimensions[2]));
}

void * mafVolume::extractVolumeData(int startPos[3], int dimensions[3]) {
    qint64 bytesPerVoxel = this->bytesPerVoxel();
    qint64 bytesPerLine  = dimensions[0] * bytesPerVoxel;

    char *oldData = (char *)m_DataValue;
    char *newData = new char[dimensions[0] * dimensions[1] * dimensions[2] * bytesPerVoxel];

    qint64 newOffset = 0;
    for (int z = 0; z < dimensions[2]; ++z) {
        qint64 oldOffset = (((startPos[2] + z) * m_Dimensions[1] + startPos[1]) * m_Dimensions[0] + startPos[0]) * bytesPerVoxel; 
        for (int y = 0; y < dimensions[1]; ++y) {
            memcpy(newData + newOffset, oldData + oldOffset, bytesPerLine);
            oldOffset += m_Dimensions[0] * bytesPerVoxel;
            newOffset += dimensions[0]   * bytesPerVoxel;
        }
    }
    return newData;
}

void *mafVolume::dataValue() {
    return m_DataValue;
}

void mafVolume::setDataValue(void *data_value) {
    if(m_DataValue == data_value)
        return; // Data is equal to that one already present.

    delete[] m_DataValue;
    m_DataValue  = data_value;
    setModified();
}

mafMatrix *mafVolume::poseMatrix() {
    if (m_Matrix == NULL) {
        m_Matrix = new mafMatrix();
        m_Matrix->setIdentity();
    }
    return m_Matrix;
}

void mafVolume::setPoseMatrix(const mafMatrix *matrix) {
    if (matrix == NULL) {
        return;
    }
    
    m_Matrix = matrix->clone();
    setModified();
}
