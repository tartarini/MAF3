/*
 *  mafCodecVolume.cpp
 *  mafSerialization
 *
 *  Created by Yubo Tao on 21/12/10.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCodecVolume.h"
#include <mafEventBusManager.h>
#include <QDir>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafSerialization;

mafCodecVolume::mafCodecVolume(const QString code_location) : mafCodecRaw(code_location) {
    m_EncodingType = "VOLUME_LOD";
}

mafCodecVolume::~mafCodecVolume() {
}

void mafCodecVolume::encode(mafMemento *memento) {
    QFile *file = qobject_cast<QFile *>(m_Device);
    REQUIRE(memento != NULL);
    REQUIRE(m_Device != NULL);
    REQUIRE(file != NULL);

    m_DataStreamWrite.setDevice(m_Device);
    m_DataStreamWrite.setVersion(QDataStream::Qt_4_6);

    const QMetaObject* meta = memento->metaObject();
    m_DataStreamWrite << QString("MementoType");
    QString cn = meta->className();
    m_DataStreamWrite << cn;
    QString ot = memento->objectClassType();
    m_DataStreamWrite << ot;

    double dataTime;
    QString dataHash;
    QString url;
    void *data = 0;
    int dataType = -1;
    int componentNum = 0;
    int memoryLimit = 1024;
    int dimensions[3] = {0, 0, 0};
    bool multiresolution = false;

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        // ignore these temporary properties
        if (item.m_Name == "fileName"           ||
            item.m_Name == "originalDimensions" ||
            item.m_Name == "startPositions"     ||
            item.m_Name == "levels"             ||
            item.m_Name == "currentLevel") {
            continue;
        } else if (item.m_Name == "multiresolution") {
            multiresolution = item.m_Value.toBool();
            continue;
        } 

        m_DataStreamWrite << item.m_Name;
        m_DataStreamWrite << item.m_Multiplicity;   
        if (item.m_Name == "codecType") {
            QString codecType = item.m_Value.toString();
            m_DataStreamWrite << codecType;
        } else if (item.m_Name == "mafDataSetTime") {
            dataTime = item.m_Value.toDouble();
            m_DataStreamWrite << dataTime;
        } else if (item.m_Name == "dataHash") {
            dataHash = item.m_Value.toString();
            m_DataStreamWrite << dataHash;
        } else if (item.m_Name == "dataType") {
            dataType = item.m_Value.toInt();
            m_DataStreamWrite << dataType;
        } else if (item.m_Name == "bitsStored") {
            int bitsStored = item.m_Value.toInt();
            m_DataStreamWrite << bitsStored;
        } else if (item.m_Name == "bigEndian") {
            bool bigEndian = item.m_Value.toBool();
            m_DataStreamWrite << bigEndian;
        } else if (item.m_Name == "componentNum") {
            componentNum = item.m_Value.toInt();
            m_DataStreamWrite << componentNum;
        } else if (item.m_Name == "dimensions") {
                QVariantList list = item.m_Value.toList();
                REQUIRE(list.size() == 3);
                for (int i = 0; i < 3; ++i) {
                    dimensions[i] = list[i].toInt();
                    m_DataStreamWrite << dimensions[i];
                }
        } else if (item.m_Name == "spacing") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            Q_FOREACH(QVariant iter, list) {
                float value = iter.toFloat();
                m_DataStreamWrite << value;
            }
        } else if (item.m_Name == "posMatrix") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 16);
            Q_FOREACH(QVariant iter, list) {
                float value = iter.toDouble();
                m_DataStreamWrite << value;
            }
        } else if (item.m_Name == "dataValue") {
            // generate file name and save internal data pointer
            QString path = file->fileName().section('/', 0, -2);
            QTextStream(&url) << path << "/" << dataHash << "_" << dataTime << ".raw";
            data = item.m_Value.value<void *>();
            m_DataStreamWrite << url;
        } else if (item.m_Name == "memoryLimit") {
            memoryLimit = item.m_Value.toInt();
            m_DataStreamWrite << memoryLimit;
        } else {
            marshall(item.m_Value);
        }
    }

    // check whether the raw volume data is valid
    if (data == 0) {
        qCritical() << mafTr("No data value");
        return;
    }
    if (dataType < mafUnsignedChar || dataType > mafDouble) {
        qCritical() << mafTr("Invalid data type");
        return;
    }
    if (dimensions[0] <= 0 || dimensions[1] <= 0 || dimensions[2] <= 0) {
        qCritical() << mafTr("Invalid dimensions");
        return;
    }

    // encode the raw volume data
    int levels = 1;
    if (multiresolution) 
        levels = calcMultiresolutionLevel(dataType, componentNum, dimensions, memoryLimit);
    encode(url, data, dataType, componentNum, dimensions, levels);
    // encode the level number
    m_DataStreamWrite << QString("levels");
    m_DataStreamWrite << 1;
    m_DataStreamWrite << levels;
}

mafMemento *mafCodecVolume::decode() {
    REQUIRE(m_Device != NULL);

    QString mementoTag;
    QString mementoType;
    QString objType;

    m_DataStreamRead.setDevice(m_Device);
    m_DataStreamRead >> mementoTag;
    m_DataStreamRead >> mementoType;
    m_DataStreamRead >> objType;

    mafMemento *memento = (mafMemento *)mafNEWFromString(mementoType);
    memento->setObjectClassType(objType);

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    mafMementoPropertyItem item;

    QString url;
    int levels = 1;
    int dataType = -1;
    int componentNum = 0;
    int memoryLimit = 1024;
    int originalDimensions[3] = {0, 0, 0};

    while(!m_DataStreamRead.atEnd()) {
        m_DataStreamRead >> item.m_Name;
        m_DataStreamRead >> item.m_Multiplicity;

        if (item.m_Name == "codecType") {
            QString codecType;
            m_DataStreamRead >> codecType;
            item.m_Value = codecType;
        } else if (item.m_Name == "mafDataSetTime") {
            double dataTime;
            m_DataStreamRead >> dataTime;
            item.m_Value = dataTime;
        } else if (item.m_Name == "dataHash") {
            QString dataHash;
            m_DataStreamRead >> dataHash;
            item.m_Value = dataHash;
        } else if (item.m_Name == "dataType") {
            m_DataStreamRead >> dataType;
            item.m_Value = dataType;
        } else if (item.m_Name == "bitsStored") {
            int bitsStored;
            m_DataStreamRead >> bitsStored;
            item.m_Value = bitsStored;
        } else if (item.m_Name == "bigEndian") {
            bool bigEndian;
            m_DataStreamRead >> bigEndian;
            item.m_Value = bigEndian;
        } else if (item.m_Name == "componentNum") {
            m_DataStreamRead >> componentNum;
            item.m_Value = componentNum;
        } else if (item.m_Name == "dimensions") {
            m_DataStreamRead >> originalDimensions[0];
            m_DataStreamRead >> originalDimensions[1];
            m_DataStreamRead >> originalDimensions[2];
            QVariantList list;
            list.append(originalDimensions[0]);
            list.append(originalDimensions[1]);
            list.append(originalDimensions[2]);
            item.m_Name  = "originalDimensions";
            item.m_Value = list;
        } else if (item.m_Name == "spacing") {
            float spacing;
            QVariantList list;
            m_DataStreamRead >> spacing;
            list.append(spacing);
            m_DataStreamRead >> spacing;
            list.append(spacing);
            m_DataStreamRead >> spacing;
            list.append(spacing);
            item.m_Value = list;
        } else if (item.m_Name == "posMatrix") {
            double value;
            QVariantList list;
            for (int i = 0; i < 16; ++i) {
                m_DataStreamRead >> value;
                list.append(value);
            }
            item.m_Value = list;
        } else if (item.m_Name == "dataValue") {
            // save the url to decode the internal data later
            m_DataStreamRead >> url;
            item.m_Name         = "fileName";
            item.m_Multiplicity = 1;
            item.m_Value        = url;
        } else if (item.m_Name == "levels") {
            m_DataStreamRead >> levels;
            item.m_Value = levels;
        } else if (item.m_Name == "memoryLimit") {
            m_DataStreamRead >> memoryLimit;
            item.m_Value = memoryLimit;
        } else {
            QString codecType;
            m_DataStreamRead >> codecType;
            item.m_Value = demarshall(codecType, item.m_Multiplicity);
        }

        propList->append(item);
    }

    REQUIRE(QFile::exists(url));

    // determine the level of the data to decode
    int startPos[3]   = { 0, 0, 0 };
    int dimensions[3] = { originalDimensions[0], originalDimensions[1], originalDimensions[2] };
    int level = calcMultiresolutionLevel(dataType, componentNum, originalDimensions, memoryLimit);
    if(level > levels) level = levels;  // bug: may be out of memory
    // scale the start position and dimension to the current level
    for (int i = 0; i < 3; ++i) {
        startPos[i]   = startPos[i]   >> (level - 1);
        dimensions[i] = dimensions[i] >> (level - 1);
    }
    // decode the volume data at the current level
    void * data = decode(url, dataType, componentNum, originalDimensions, startPos, dimensions, level);

    // check whether the volume data is loaded
    if (data == 0) {
        mafDEL(memento);
        return 0;
    }

    // start position in the current resolution
    QVariantList list;
    list.append(startPos[0]);
    list.append(startPos[1]);
    list.append(startPos[2]);
    item.m_Name         = "startPositions";
    item.m_Multiplicity = 1;
    item.m_Value        = list;
    propList->append(item);

    // dimensions in the current resolution
    list.clear();
    list.append(dimensions[0]);
    list.append(dimensions[1]);
    list.append(dimensions[2]);
    item.m_Name         = "dimensions";
    item.m_Multiplicity = 1;
    item.m_Value        = list;
    propList->append(item);

    // current level
    item.m_Name         = "currentLevel";
    item.m_Multiplicity = 1;
    item.m_Value        = level;
    propList->append(item);

    // data values
    item.m_Name         = "dataValue";
    item.m_Multiplicity = 1;
    item.m_Value        = qVariantFromValue(data);
    propList->append(item);

    return memento;
}

void mafCodecVolume::update(mafCore::mafMemento *memento) {
    REQUIRE(memento != NULL);

    QString url;
    void *data = 0;
    int levels = 1;
    int currentLevel = 0;
    int dataType = -1;
    int componentNum = 0;
    int memoryLimit = 1024;
    int originalDimensions[3] = {0, 0, 0};
    int startPos[3] = { 0, 0, 0 };
    int dimensions[3] = { 0, 0, 0 };
    int updatedStartPos[3] = { -1, -1, -1 };
    int updatedDimensions[3] = { -1, -1, -1 };

    mafMementoPropertyList *propList = memento->mementoPropertyList();
    Q_FOREACH(mafMementoPropertyItem item, *propList) {
        // information about the highest resolution volume
        if (item.m_Name == "originalDimensions") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                originalDimensions[i] = list[i].toInt();
        } else if (item.m_Name == "dataType") {
            dataType = item.m_Value.toInt(); 
        } else if (item.m_Name == "componentNum") {
                componentNum = item.m_Value.toInt();
        } else if (item.m_Name == "fileName") {
            url = item.m_Value.toString();
        } else if (item.m_Name == "levels") {
            levels = item.m_Value.toInt();
        } else if (item.m_Name == "memoryLimit") {
            memoryLimit = item.m_Value.toInt();
        // information about the current volume data
        } else if (item.m_Name == "startPositions") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                startPos[i] = list[i].toInt();
        } else if (item.m_Name == "dimensions") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                dimensions[i] = list[i].toInt();
        } else if (item.m_Name == "currentLevel") {
            currentLevel = item.m_Value.toInt();
        } else if (item.m_Name == "dataValue") {
            data = item.m_Value.value<void *>();
        // information about the updated volume data in the highest resolution
        } else if (item.m_Name == "updatedStartPositions") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                updatedStartPos[i] = list[i].toInt();
        } else if (item.m_Name == "updatedDimensions") {
            QVariantList list = item.m_Value.toList();
            REQUIRE(list.size() == 3);
            for (int i = 0; i < 3; ++i)
                updatedDimensions[i] = list[i].toInt();
        }
    }

    // if the start positions are not changed
    bool startPosEqual = true;
    for (int i = 0; i < 3; ++i) {
        if (updatedStartPos[i] == -1)
            updatedStartPos[i] = startPos[i] << (currentLevel - 1);
        else
            startPosEqual &= (startPos[i] == updatedStartPos[i]);
    }
    // if the dimensions are not changed
    bool dimensionsEqual = true;
    for (int i = 0; i < 3; ++i) {
        if (updatedDimensions[i] == -1)
            updatedDimensions[i] = dimensions[i] << (currentLevel - 1);
        else
            dimensionsEqual &= (dimensions[i] == updatedDimensions[i]);
    }
    // check whether need to update
    if (startPosEqual && dimensionsEqual) return;

    // determine the level of the data to decode
    int level = calcMultiresolutionLevel(dataType, componentNum, updatedDimensions, memoryLimit);
    if (level > levels) level = levels; // bug: may be out of memory
    // scale the start position and dimension to the current level
    for (int i = 0; i < 3; ++i) {
        updatedStartPos[i]   = updatedStartPos[i]   >> (level - 1);
        updatedDimensions[i] = updatedDimensions[i] >> (level - 1);
    }

    // reload the volume data
    void *updatedData = 0;
    if (currentLevel == level && inRange(startPos, dimensions, updatedStartPos, updatedDimensions)) {
        // if only the dimension is reduced
        int startOffset[3] = { updatedStartPos[0] - startPos[0], updatedStartPos[1] - startPos[1], updatedStartPos[2] - startPos[2] };
        updatedData = extractVolumeData(data, dataType, componentNum, dimensions, startOffset, updatedDimensions);
    } else {
        updatedData = decode(url, dataType, componentNum, originalDimensions, updatedStartPos, updatedDimensions, level);
    }

    // check whether the volume data is loaded
    if (updatedData == 0)
        return;

    // update the information in memento
    QVariantList list;
    mafMementoPropertyList::iterator iter = propList->begin();
    while (iter != propList->end()) {
        if (iter->m_Name == "startPositions") {
            list.clear();
            list.append(updatedStartPos[0]);
            list.append(updatedStartPos[1]);
            list.append(updatedStartPos[2]);
            iter->m_Value = list;
        } else if (iter->m_Name == "dimensions") {
            list.clear();
            list.append(updatedDimensions[0]);
            list.append(updatedDimensions[1]);
            list.append(updatedDimensions[2]);
            iter->m_Value = list;
        } else if (iter->m_Name == "currentLevel") {
            iter->m_Value = level;
        } else if (iter->m_Name == "dataValue") {
            iter->m_Value = qVariantFromValue(updatedData);
            delete [](char*)data;
            data = 0;
        }

        if (iter->m_Name == "updatedStartPositions" || iter->m_Name == "updatedDimensions")
            iter = propList->erase(iter);
        else
            ++iter;
    }
}

void mafCodecVolume::marshall(const QVariant &value) {
    switch(value.type()){
        case QVariant::Int:
        case QVariant::UInt:
        case QVariant::LongLong:
        case QVariant::ULongLong:
            m_DataStreamWrite << QString("int");
            m_DataStreamWrite << (int)value.toInt();
            break;
        case QVariant::Double:
            m_DataStreamWrite << QString("double");
            m_DataStreamWrite << (double)value.toDouble();
            break;
        case QVariant::Bool:
            m_DataStreamWrite << QString("boolean");
            m_DataStreamWrite << (bool)value.toBool();
            break;
        case QVariant::Date:
            m_DataStreamWrite << QString("dateTime.iso8601");
            m_DataStreamWrite << (QString)value.toDate().toString( Qt::ISODate );
            break;
        case QVariant::DateTime:
            m_DataStreamWrite << QString("dateTime.iso8601");
            m_DataStreamWrite << (QString)value.toDateTime().toString( Qt::ISODate );
            break;
        case QVariant::Time:
            m_DataStreamWrite << QString("dateTime.iso8601");
            m_DataStreamWrite << (QString)value.toTime().toString( Qt::ISODate );
            break;
        case QVariant::StringList:
        case QVariant::List: {
            m_DataStreamWrite << QString("list");
            Q_FOREACH(QVariant item, value.toList()) {
                marshall(item);
            }
            break;
        }
        case QVariant::Map: {
            QMap<QString, QVariant> map = value.toMap();
            QMap<QString, QVariant>::ConstIterator index = map.begin();
            m_DataStreamWrite << QString("map");
            while(index != map.end()) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                marshall(*index);
                ++index;
            }
            break;
        }
        case QVariant::Hash: {
            QVariantHash hash = value.toHash();
            QVariantHash::ConstIterator index = hash.begin();
            m_DataStreamWrite << QString("hash");
            while(index != hash.end()) {
                marshall(index.key());
                int multi = 0;
                if (index.value().type() == QVariant::List) {
                    multi = index.value().toList().count();
                } else if (index.value().type() == QVariant::Map) {
                    multi = index.value().toMap().count();
                } else if (index.value().type() == QVariant::Hash) {
                    multi = index.value().toHash().count();
                }
                m_DataStreamWrite << multi;
                marshall(*index);
                ++index;
            }
            break;
        }
        case QVariant::ByteArray: {
            m_DataStreamWrite << QString("base64");
            m_DataStreamWrite << value.toByteArray().toBase64();
            break;
        }
        default: {
            if(value.canConvert(QVariant::String)) {
                m_DataStreamWrite << QString("string");
                m_DataStreamWrite << (QString)value.toString();
            }
            else {
               //self representation?
            }
            break;
        }
    }
}

QVariant mafCodecVolume::demarshall(QString typeName, int multiplicity) {
    if (typeName == "string") {
        QString value;
        m_DataStreamRead >> value;
        return QVariant(value);
    }
    else if (typeName == "int") {
        int value = 0;
        m_DataStreamRead >> value;
        return QVariant(value);
    }
    else if(typeName == "double") {
        double value = 0;
        m_DataStreamRead >> value;
        return QVariant(value);
    }
    else if(typeName == "boolean") {
        bool value;
        m_DataStreamRead >> value;
        return QVariant(value);
    }
    else if(typeName == "datetime" || typeName == "dateTime.iso8601") {
        QString value;
        m_DataStreamRead >> value;
        return QVariant(QDateTime::fromString(value, Qt::ISODate));
    }
    else if(typeName == "list") {
        QVariantList value;
        for (int i = 0; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
            value.append(QVariant(demarshall(type, multi)));
        }
        return QVariant(value);
    }
    else if(typeName == "map") {
        QMap<QString,QVariant> stct;
        for (int i = 0; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
            QString nodeName = demarshall(type, multi).toString();
            m_DataStreamRead >> type;
            m_DataStreamRead >> multi;
            stct[nodeName] = QVariant(demarshall(type, multi));
        }
        return QVariant(stct);
    }
    else if(typeName == "hash") {
        QHash<QString,QVariant> hash;
        for (int i = 0; i < multiplicity; ++i) {
            QString type;
            int multi = 0;
            m_DataStreamRead >> type;
            QString nodeName = demarshall(type, multi).toString();
            m_DataStreamRead >> multi;
            m_DataStreamRead >> type;
            hash[nodeName] = QVariant(demarshall(type, multi));
        }
        return QVariant(hash);
    }
    else if(typeName == "base64") {
        QVariant returnVariant;
        QByteArray dest;
        QByteArray src;
        m_DataStreamRead >> src;
        dest = QByteArray::fromBase64(src);
        QDataStream ds(&dest, QIODevice::ReadOnly);
        ds.setVersion(QDataStream::Qt_4_6);
        ds >> returnVariant;
        return returnVariant.isValid() ? returnVariant : QVariant(dest);
    }
    qCritical() << QString("Cannot handle type %1").arg(typeName);
    return QVariant();
}

int mafCodecVolume::getByteNum(int dataType) {
    int byteNum = 0;
    switch(dataType) {
        case mafUnsignedChar:
        case QChar:
            byteNum = 1;
            break;
        case mafUnsignedShort:
        case mafShort:
            byteNum = 2;
            break;
        case mafUnsignedInt:
        case mafInt:
        case mafFloat:
            byteNum = 4;
            break;
        case mafDouble:
            byteNum = sizeof(double);
            break;
        default:
            qCritical("Unknown data type.\n");

    }
    return byteNum;
}

int mafCodecVolume::calcMultiresolutionLevel(int dataType, int componentNum, int dimensions[3], int memoryLimit) {
    int levels = 1;
    int xSize = dimensions[0], ySize = dimensions[1], zSize = dimensions[2];
    int bytesPerVoxel = getByteNum(dataType) * componentNum;
    int totalBytes = bytesPerVoxel * xSize * ySize * zSize / 1024;
    // reduce the dimensions by half until the size is not larger than the memory limit 
    while(totalBytes > memoryLimit) {
        xSize = xSize >> 1;
        ySize = ySize >> 1;
        zSize = zSize >> 1;
        totalBytes = xSize * ySize * zSize * bytesPerVoxel / 1024;
        ++levels;
    }
    return levels;
}

template<typename T>
void resampleVolume(T *originalData, T *resampledData, int componentNum, int dimensions[3]) {
    int xSize = dimensions[0] >> 1;
    int ySize = dimensions[1] >> 1;
    int zSize = dimensions[2] >> 1;
    // resample using nearest neighbor interpolation, need to support linear interpolation
    for(int z = 0; z < zSize; ++z) {
        for(int y = 0; y < ySize; ++y) {
            for(int x = 0; x < xSize; ++ x) {
                int originalIndex  = ((z * dimensions[1] + y) * dimensions[0] + x) * componentNum * 2;
                int resampledIndex = ((z * ySize + y) * xSize + x) * componentNum;
                for(int i = 0; i < componentNum; ++i)
                    resampledData[resampledIndex + i] = originalData[originalIndex + i];
            }
        }
    }
}

void mafCodecVolume::resample(void *originalData, void *resampledData, int dataType, int componentNum, int dimensions[3]) {
    // resample the volume data depending on the data type
    switch(dataType) {
        case mafUnsignedChar:
            resampleVolume<unsigned char>((unsigned char*)originalData, (unsigned char*)resampledData, componentNum, dimensions);
            break;
        case QChar:
            resampleVolume<char>((char*)originalData, (char*)resampledData, componentNum, dimensions);
            break;
        case mafUnsignedShort:
            resampleVolume<unsigned short>((unsigned short*)originalData, (unsigned short*)resampledData, componentNum, dimensions);
            break;
        case mafShort:
            resampleVolume<short>((short*)originalData, (short*)resampledData, componentNum, dimensions);
            break;
        case mafUnsignedInt:
            resampleVolume<unsigned int>((unsigned int*)originalData, (unsigned int*)resampledData, componentNum, dimensions);
            break;
        case mafInt:
            resampleVolume<int>((int*)originalData, (int*)resampledData, componentNum, dimensions);
            break;
        case mafFloat:
            resampleVolume<float>((float*)originalData, (float*)resampledData, componentNum, dimensions);
            break;
        case mafDouble:
            resampleVolume<double>((double*)originalData, (double*)resampledData, componentNum, dimensions);
            break;
        default:
            qCritical("Unknown data type.\n");
    }
}

bool mafCodecVolume::inRange(int startPos[3], int dimensions[3], int updatedStartPos[3], int updatedDimensions[3]) {
    return ((updatedStartPos[0] >= startPos[0]) &&
            (updatedStartPos[1] >= startPos[1]) &&
            (updatedStartPos[2] >= startPos[2]) &&
            (updatedStartPos[0] + updatedDimensions[0] <= startPos[0] + dimensions[0]) &&
            (updatedStartPos[1] + updatedDimensions[1] <= startPos[1] + dimensions[1]) &&
            (updatedStartPos[2] + updatedDimensions[2] <= startPos[2] + dimensions[2]));
}

void * mafCodecVolume::extractVolumeData(void *data, int dataType, int componentNum, int originalDimensions[3], int startPos[3], int dimensions[3]) {
    int bytesPerVoxel = getByteNum(dataType) * componentNum;
    int bytesPerLine  = dimensions[0] * bytesPerVoxel;

    char *oldData = (char *)data;
    char *newData = new char[dimensions[0] * dimensions[1] * dimensions[2] * bytesPerVoxel];

    int newOffset = 0;
    for (int z = 0; z < dimensions[2]; ++z) {
        int oldOffset = (((startPos[2] + z) * originalDimensions[1] + startPos[1]) * originalDimensions[0] + startPos[0]) * bytesPerVoxel; 
        for (int y = 0; y < dimensions[1]; ++y) {
            memcpy(newData + newOffset, oldData + oldOffset, bytesPerLine);
            oldOffset += originalDimensions[0] * bytesPerVoxel;
            newOffset += dimensions[0] * bytesPerVoxel;
        }
    }
    return newData;
}

void mafCodecVolume::encode(QString url, void *data, int dataType, int componentNum, int dimensions[3], int levels) {
    // open file
    QFile file(url);
    if (!file.open(QIODevice::WriteOnly)) {
        QByteArray ba = mafTr("Not able to open file '%1'").arg(url).toAscii();
        qCritical("%s", ba.data());
        return;
    }

    // write the multi-resolution data
    qint64 bytesPerVoxel = getByteNum(dataType) * componentNum;
    char *originalData   = (char *)data;
    char *sampledData    = new char[dimensions[0] * dimensions[1] * dimensions[2] / 8 * bytesPerVoxel]; 
    for (int level = 0; level < levels; ++level) {
        if (level > 0) {
            resample(originalData, sampledData, dataType, componentNum, dimensions);
            dimensions[0] = dimensions[0] >> 1;
            dimensions[1] = dimensions[1] >> 1;
            dimensions[2] = dimensions[2] >> 1;
            originalData = sampledData;
        }
        file.write(originalData, dimensions[0] * dimensions[1] * dimensions[2] * bytesPerVoxel);
    }
    delete []sampledData;
}

void * mafCodecVolume::decode(QString url, int dataType, int componentNum, int originalDimensions[3], int startPos[3], int dimensions[3], int level) {
    // open file
    QFile file(url);
    if (!file.open(QIODevice::ReadOnly)) {
        QByteArray ba = mafTr("Not able to open file '%1'").arg(url).toAscii();
        qCritical("%s", ba.data());
        return 0;
    } 

    // find the start position and dimensions in the current resolution level
    qint64 fileOffset = 0;
    qint64 bytesPerVoxel = getByteNum(dataType) * componentNum;
    for (int i = 1; i < level; ++i) {
        fileOffset += originalDimensions[0] * originalDimensions[1] * originalDimensions[2] * bytesPerVoxel;
        for(int j = 0; j < 3; ++j)
            originalDimensions[j] = originalDimensions[j] >> 1;
    }
    // the start offset of the data in the file
    fileOffset += startPos[2] * originalDimensions[1] * originalDimensions[0] * bytesPerVoxel;

    // determine the slice number for the memory mapped file
    int    mappedSlices  = dimensions[2];
    qint64 bytesPerSlice = originalDimensions[0] * originalDimensions[1] * bytesPerVoxel;
    qint64 mappingSize   = mappedSlices * bytesPerSlice;
    qint64 mappingLimit  = 64 * 1024 * 1024;  // 64M
    while (mappingSize > mappingLimit) {
        mappedSlices = mappedSlices >> 1;
        mappingSize  = mappedSlices * bytesPerSlice;
    }

    // read the volume data
    char *data = new char[dimensions[2] * dimensions[1] * dimensions[0] * bytesPerVoxel];
    for (int z = 0; z < dimensions[2]; z += mappedSlices) {
        if (z + mappedSlices > dimensions[2]) { // check for the last mapping
            mappedSlices = dimensions[2] - z;
            mappingSize  = mappedSlices * bytesPerSlice;
        }
        char *mappedMemory = (char*)file.map(fileOffset + z * bytesPerSlice, mappingSize);
        // fill the data from the memory mapped file
        for (int i = 0; i < mappedSlices; ++i) {
            int dataOffset = (z + i) * dimensions[1] * dimensions[0] * bytesPerVoxel;
            int memoryOffset = i * bytesPerSlice + (startPos[1] * originalDimensions[0] + startPos[0]) * bytesPerVoxel;
            for(int y = 0; y < dimensions[1]; ++y) {
                memcpy(data + dataOffset, mappedMemory + memoryOffset, dimensions[0] * bytesPerVoxel);
                dataOffset   += dimensions[0] * bytesPerVoxel;
                memoryOffset += originalDimensions[0] * bytesPerVoxel;
            }
        }
        file.unmap((uchar*)mappedMemory);
    }
    return data;
}
