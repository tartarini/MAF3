/*
 *  mafExternalDataCodecVolume.cpp
 *  mafSerialization
 *
 *  Created by Yubo Tao on 24/02/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafExternalDataCodecVolume.h"
#include <mafVolume.h>

using namespace mafCore;
using namespace mafResources;
using namespace mafSerialization;
using namespace mafPluginOutOfCore;

mafExternalDataCodecVolume::mafExternalDataCodecVolume(const QString code_location) : mafExternalDataCodec(code_location) {
}

mafExternalDataCodecVolume::~mafExternalDataCodecVolume() {
}

char *mafExternalDataCodecVolume::encode(bool binary) {
    mafVolume *volume = dynamic_cast<mafProxy<mafVolume>*>(m_ExternalData)->externalData();

    // open file
    QString url = volume->fileName();
    QFile file(url);
    if (!file.open(QIODevice::WriteOnly)) {
        QByteArray ba = mafTr("Not able to open file '%1'").arg(url).toAscii();
        qCritical("%s", ba.data());
        return "";
    }

    // extract properties required for encoding
    int *dims = volume->dimensions();
    int dimensions[3] = { dims[0], dims[1], dims[2] };
    int levels = volume->calcMultiresolutionLevel();
    qint64 bytesPerVoxel = volume->bytesPerVoxel();

    // write the multi-resolution data
    char *originalData = (char *)volume->dataValue();
    file.write(originalData, dimensions[0] * dimensions[1] * dimensions[2] * bytesPerVoxel);

    char *sampledData = new char[dimensions[0] * dimensions[1] * dimensions[2] / 8 * bytesPerVoxel]; 
    for (int level = 1; level < levels; ++level) {
        dimensions[0] = dimensions[0] >> 1;
        dimensions[1] = dimensions[1] >> 1;
        dimensions[2] = dimensions[2] >> 1;
        volume->resample(sampledData, dimensions);
        file.write(sampledData, dimensions[0] * dimensions[1] * dimensions[2] * bytesPerVoxel);
    }
    delete []sampledData;

    return "";
}

void mafExternalDataCodecVolume::decode(const char *input_string, bool binary) {
    mafVolume *volume = dynamic_cast<mafProxy<mafVolume>*>(m_ExternalData)->externalData();

    // check whether the updating is required
    if (!volume->updateDataValueRequired()) return;
    
    // extract required properties for encoding
    int *pos = volume->updatedStartPos();
    int updatedStartPos[3] = { pos[0], pos[1], pos[2] };
    int *dims = volume->updatedDimensions();
    int updatedDimensions[3] = { dims[0], dims[1], dims[2] };
    
    // determine the level of the data to decode
    int level  = volume->calcMultiresolutionLevel(true);
    int levels = volume->levelNum();
    if (level > levels) level = levels; // bug: may be out of memory
    // scale the start position and dimension to the current level
    for (int i = 0; i < 3; ++i) {
        updatedStartPos[i]   = updatedStartPos[i]   >> (level - 1);
        updatedDimensions[i] = updatedDimensions[i] >> (level - 1);
    }

    // reload the volume data
    void *updatedData = 0;
    int previousLevel = volume->level();
    if (level == previousLevel && volume->isPartOf(updatedStartPos, updatedDimensions)) {
        // if only the dimension is reduced
        int *startPos      = volume->startPositions();
        int startOffset[3] = { updatedStartPos[0] - startPos[0], updatedStartPos[1] - startPos[1], updatedStartPos[2] - startPos[2] };
        updatedData = volume->extractVolumeData(startOffset, updatedDimensions);
    } else {
        updatedData = decode(updatedStartPos, updatedDimensions, level);
    }

    // check whether the volume data is loaded
    if (updatedData == 0)
        return;

    // update the volume
    volume->setStartPositions(updatedStartPos);
    volume->setDimensions(updatedDimensions);
    volume->setLevel(level);
    volume->setDataValue(updatedData);
}

void * mafExternalDataCodecVolume::decode(int startPos[3], int dimensions[3], int level) {
    mafVolume *volume = dynamic_cast<mafProxy<mafVolume>*>(m_ExternalData)->externalData();

    // open file
    QString url = volume->fileName();
    QFile file(url);
    if (!file.open(QIODevice::ReadOnly)) {
        QByteArray ba = mafTr("Not able to open file '%1'").arg(url).toAscii();
        qCritical("%s", ba.data());
        return 0;
    } 

    // find the start position and dimensions in the current resolution level
    qint64 fileOffset = 0;
    qint64 bytesPerVoxel = volume->bytesPerVoxel();
    int *oriDims = volume->originalDimensions();
    int  originalDimensions[3] = { oriDims[0], oriDims[1], oriDims[2] };
    for (int i = 1; i < level; ++i) {
        fileOffset += originalDimensions[0] * originalDimensions[1] * originalDimensions[2] * bytesPerVoxel;
        for(int j = 0; j < 3; ++j)
            originalDimensions[j] = originalDimensions[j] >> 1;
    }
    // the start offset of the data in the file
    fileOffset += startPos[2] * originalDimensions[1] * originalDimensions[0] * bytesPerVoxel;

    // determine the slice number for the memory mapped file
    qint64 mappedSlices  = dimensions[2];
    qint64 bytesPerSlice = originalDimensions[0] * originalDimensions[1] * bytesPerVoxel;
    qint64 mappingSize   = mappedSlices * bytesPerSlice;
    qint64 mappingLimit  = 64 * 1024 * 1024;  // 64M
    while (mappingSize > mappingLimit) {
        mappedSlices = mappedSlices >> 1;
        mappingSize  = mappedSlices * bytesPerSlice;
    }

    // read the volume data
    char *data = new char[dimensions[2] * dimensions[1] * dimensions[0] * bytesPerVoxel];
    qint64 bytesPerCurrentLine  = dimensions[0] * bytesPerVoxel;
    qint64 bytesPerOriginalLine = originalDimensions[0] * bytesPerVoxel;
    for (int z = 0; z < dimensions[2]; z += mappedSlices) {
        if (z + mappedSlices > dimensions[2]) { // check for the last mapping
            mappedSlices = dimensions[2] - z;
            mappingSize  = mappedSlices * bytesPerSlice;
        }
        char *mappedMemory = (char*)file.map(fileOffset + z * bytesPerSlice, mappingSize);
        // fill the data from the memory mapped file
        for (qint64 i = 0; i < mappedSlices; ++i) {
            qint64 dataOffset   = (z + i) * dimensions[1] * dimensions[0] * bytesPerVoxel;
            qint64 memoryOffset = i * bytesPerSlice + (startPos[1] * originalDimensions[0] + startPos[0]) * bytesPerVoxel;
            for(int y = 0; y < dimensions[1]; ++y) {
                memcpy(data + dataOffset, mappedMemory + memoryOffset, bytesPerCurrentLine);
                dataOffset   += bytesPerCurrentLine;
                memoryOffset += bytesPerOriginalLine;
            }
        }
        file.unmap((uchar*)mappedMemory);
    }
    return data;
}
