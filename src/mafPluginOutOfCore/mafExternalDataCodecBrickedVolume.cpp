/*
 *  mafExternalDataCodecBrickedVolume.cpp
 *  mafSerialization
 *
 *  Created by Yubo Tao on 24/02/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafExternalDataCodecBrickedVolume.h"
#include "mafVolume.h"

using namespace mafCore;
using namespace mafResources;
using namespace mafSerialization;
using namespace mafPluginOutOfCore;

mafExternalDataCodecBrickedVolume::mafExternalDataCodecBrickedVolume(const QString code_location) : mafExternalDataCodec(code_location) {
}

mafExternalDataCodecBrickedVolume::~mafExternalDataCodecBrickedVolume() {
}

char *mafExternalDataCodecBrickedVolume::encode(bool binary) {
    mafVolume *volume = dynamic_cast<mafProxy<mafVolume>*>(m_ExternalData)->externalData();

    // open all files
    QString url = volume->fileName();
    QFile bpiFile(url + ".bpi"), bevFile(url + ".bev"), bvFile(url + ".bv");
    QByteArray ba;
    if (!bpiFile.open(QIODevice::WriteOnly)) {
        ba = mafTr("Not able to open file '%1'.bpi").arg(url).toAscii();
        qCritical("%s", ba.data());
        return "";
    }
    if (!bevFile.open(QIODevice::WriteOnly)) {
        ba = mafTr("Not able to open file '%1'.bev").arg(url).toAscii();
        qCritical("%s", ba.data());
        return "";
    }
    if (!bvFile.open(QIODevice::WriteOnly)) {
        ba = mafTr("Not able to open file '%1'.bv").arg(url).toAscii();
        qCritical("%s", ba.data());
        return "";
    }

    // extract properties required for encoding
    int  *dims = volume->dimensions();
    int   dimensions[3] = { dims[0], dims[1], dims[2] };
    int   brickSize     = volume->brickSize();
    int   levels        = volume->calcMultiresolutionLevel();
    qint64 bytesPerVoxel = volume->bytesPerVoxel();
    qint64 bytesPerBrick = brickSize * brickSize * brickSize * bytesPerVoxel;

    // encode the volume data in bricks
    char *brickData    = new char[bytesPerBrick];
    char *sampledData  = new char[dimensions[0] * dimensions[1] * dimensions[2] / 8 * bytesPerVoxel];
    char *originalData = (char *)volume->dataValue();

    qint64 position, bevPosition = 0, bvPosition = 0;
    for (int level = 0; level < levels; ++level) {
        // resample the volume data if not the highest resolution data
        if(level > 0) { 
            dimensions[0] = dimensions[0] >> 1;
            dimensions[1] = dimensions[1] >> 1;
            dimensions[2] = dimensions[2] >> 1;
            volume->resample(sampledData, dimensions);
            originalData = sampledData;
        }

        // encode bricks of the current resolution data
        int bricksNum[3];
        for (int i = 0; i < 3; ++i)
            bricksNum[i] = static_cast<uint>(ceil(float(dimensions[i]) / brickSize));
        
        for (int brickZ = 0; brickZ < bricksNum[2]; ++brickZ) {
            for (int brickY = 0; brickY < bricksNum[1]; ++brickY) {
                for (int brickX = 0; brickX < bricksNum[0]; ++brickX) {
                    int brickIndex[3] = { brickX, brickY, brickZ };
                    // extract the brick data from the original volume data
                    extractBrickData(originalData, brickData, dimensions, brickIndex, brickSize, bytesPerVoxel);
                    bool equal = voxelsEqualInBrick(brickData, brickSize, bytesPerVoxel);
                    if (equal) {
                        position  = bevPosition << 1;
                        position |= 1;
                        bevFile.write(brickData, bytesPerVoxel);    // store only one voxel
                        bevPosition += bytesPerVoxel;
                    } else {
                        position = bvPosition << 1;
                        bvFile.write(brickData, bytesPerBrick);     // store the whole voxels
                        bvPosition += bytesPerBrick;
                    }
                    bpiFile.write((const char*)&position, sizeof(qint64));  // store the meta information of the brick
                }
            }
        }  
    }

    delete []brickData;
    delete []sampledData;

    return "";
}

void mafExternalDataCodecBrickedVolume::decode(const char *input_string, bool binary) {
    mafVolume *volume = dynamic_cast<mafProxy<mafVolume>*>(m_ExternalData)->externalData();

    // check whether the updating is required
    if (!volume->updateDataValueRequired()) return;
    
    // extract required properties for encoding
    int *pos = volume->updatedStartPos();
    int  updatedStartPos[3] = { pos[0], pos[1], pos[2] };
    int *dims = volume->updatedDimensions();
    int  updatedDimensions[3] = { dims[0], dims[1], dims[2] };
    
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

void mafExternalDataCodecBrickedVolume::extractBrickData(char *data, char *brickData, int dimensions[3], int brickIndex[3], int brickSize, qint64 bytesPerVoxel) {
    memset(brickData, 0, brickSize * brickSize * brickSize * bytesPerVoxel);
    // decide the start and end positions in the original volume
    qint64 startPos[3], endPos[3];
    for (int i = 0; i < 3; ++i) {
        startPos[i] = brickIndex[i] * brickSize;
        endPos[i]   = startPos[i] + brickSize;
        if (endPos[i] > dimensions[i])
            endPos[i] = dimensions[i];
    }
    // copy the brick data from the original volume
    qint64 bytesPerLine = (endPos[0] - startPos[0]) * bytesPerVoxel;
    for (qint64 z = startPos[2]; z < endPos[2]; ++z) {
        for (qint64 y = startPos[1]; y < endPos[1]; ++y) {
            qint64 offset = ((z * dimensions[1] + y) * dimensions[0] + startPos[0]) * bytesPerVoxel;
            qint64 brickOffset = ((z - startPos[2]) * brickSize + (y - startPos[1])) * brickSize * bytesPerVoxel;
            memcpy(brickData + brickOffset, data + offset, bytesPerLine);
        }
    }
}

bool mafExternalDataCodecBrickedVolume::voxelsEqualInBrick(char *brickData, int brickSize, qint64 bytesPerVoxel) {
    bool equal = true;
    qint64 bytesNum = brickSize * brickSize * brickSize * bytesPerVoxel;
    for (qint64 i = bytesPerVoxel; i < bytesNum; i += bytesPerVoxel) {
        for (qint64 j = 0; j < bytesPerVoxel; ++j)
            equal &= (brickData[j] == brickData[i + j]);
        if (!equal)
            break;
    }
    return equal;
}

void mafExternalDataCodecBrickedVolume::fillVolumeWithEqualBrick(char *volumeData, char *voxelValue, int volumePos[3], int volumeDimensions[3], int brickStartPos[3], int brickSize, qint64 bytesPerVoxel) {
    // convert the start position in the original volume to the current volume
    int volumeStartPos[3] = { volumePos[0], volumePos[1], volumePos[2] };
    int volumeEndPos[3], length[3];
    for (int i = 0; i < 3; ++i) {
        volumeStartPos[i] = brickStartPos[i] - volumeStartPos[i];
        volumeEndPos[i]   = volumeStartPos[i] + brickSize;
        if (volumeStartPos[i] < 0)
            volumeStartPos[i] = 0;
        if (volumeEndPos[i] > volumeDimensions[i])
            volumeEndPos[i] = volumeDimensions[i];
        length[i] = volumeEndPos[i] - volumeStartPos[i];
    }

    // copy the data from the brick to fill the current volume
    qint64 bytesPerLine = length[0] * bytesPerVoxel;
    qint64 volumeOffset = ((volumeStartPos[2] * volumeDimensions[1] + volumeStartPos[1]) * volumeDimensions[0] + volumeStartPos[0]) * bytesPerVoxel;
    for (int z = 0; z < length[2]; ++z) {
        for (int y = 0; y < length[1]; ++y) {
            qint64 offset = volumeOffset + y * volumeDimensions[0] * bytesPerVoxel;
            for (int i = 0; i < bytesPerLine; i += bytesPerVoxel, offset += bytesPerVoxel) {
                for (int j = 0; j < bytesPerVoxel; ++j)
                    volumeData[offset + j] = voxelValue[j];
            }
        }
        volumeOffset += volumeDimensions[1] * volumeDimensions[0] * bytesPerVoxel;
    }
}

void mafExternalDataCodecBrickedVolume::fillVolumeWithBrick(char *volumeData, char *brickData, int volumePos[3], int volumeDimensions[3], int brickPos[3], int brickSize, qint64 bytesPerVoxel) {
    // convert the start position in the original volume to the current volume
    int volumeStartPos[3] = { volumePos[0], volumePos[1], volumePos[2] };
    int brickStartPos[3]  = { brickPos[0], brickPos[1], brickPos[2] };
    int volumeEndPos[3], brickEndPos[3], length[3];
    for (int i = 0; i < 3; ++i) {
        volumeStartPos[i] = brickStartPos[i] - volumeStartPos[i];
        volumeEndPos[i]   = volumeStartPos[i] + brickSize;
        brickStartPos[i]  = 0;
        brickEndPos[i]    = brickSize;
        if (volumeStartPos[i] < 0) {
            brickStartPos[i]  = -volumeStartPos[i];
            volumeStartPos[i] = 0;
        }
        if (volumeEndPos[i] > volumeDimensions[i]) {
            brickEndPos[i]  = brickSize - (volumeEndPos[i] - volumeDimensions[i]);
            volumeEndPos[i] = volumeDimensions[i];
        }
        length[i] = volumeEndPos[i] - volumeStartPos[i];
    }

    // copy the data from the brick to fill the current volume
    qint64 bytesPerLine = length[0] * bytesPerVoxel;
    qint64 volumeOffset = ((volumeStartPos[2] * volumeDimensions[1] + volumeStartPos[1]) * volumeDimensions[0] + volumeStartPos[0]) * bytesPerVoxel;
    qint64 brickOffset  = ((brickStartPos[2]  * brickSize           + brickStartPos[1])  * brickSize           + brickStartPos[0])  * bytesPerVoxel;
    for (int z = 0; z < length[2]; ++z) {
        for (int y = 0; y < length[1]; ++y) {
            memcpy(volumeData + volumeOffset + y * volumeDimensions[0] * bytesPerVoxel, brickData + brickOffset + y * brickSize * bytesPerVoxel, bytesPerLine);
        }
        volumeOffset += volumeDimensions[1] * volumeDimensions[0] * bytesPerVoxel;
        brickOffset  += brickSize * brickSize * bytesPerVoxel;
    }
}

void * mafExternalDataCodecBrickedVolume::decode(int startPos[3], int dimensions[3], int level) {
    mafVolume *volume = dynamic_cast<mafProxy<mafVolume>*>(m_ExternalData)->externalData();
    
    // open all files
    QString url = volume->fileName();
    QFile bpiFile(url + ".bpi"), bevFile(url + ".bev"), bvFile(url + ".bv");
    QByteArray ba;
    if (!bpiFile.open(QIODevice::ReadOnly)) {
        ba = mafTr("Not able to open file '%1'.bpi").arg(url).toAscii();
        qCritical("%s", ba.data());
        return 0;
    }
    if (!bevFile.open(QIODevice::ReadOnly)) {
        ba = mafTr("Not able to open file '%1'.bev").arg(url).toAscii();
        qCritical("%s", ba.data());
        return 0;
    }
    if (!bvFile.open(QIODevice::ReadOnly)) {
        ba = mafTr("Not able to open file '%1'.bv").arg(url).toAscii();
        qCritical("%s", ba.data());
        return 0;
    }

    // find the start brick position at the current resolution level
    int  bricksNum[3];
    int  brickSize = volume->brickSize();
    int *oriDims = volume->originalDimensions();
    int  originalDimensions[3] = { oriDims[0], oriDims[1], oriDims[2] };
    qint64 brickOffset = 0;
    for (int i = 1; i < level; ++i) {
        for (int j = 0; j < 3; ++j) {
            bricksNum[j] = static_cast<int>(ceil(float(originalDimensions[j]) / brickSize));
            originalDimensions[j] = originalDimensions[j] >> 1;
        }
        brickOffset += bricksNum[0] * bricksNum[1] * bricksNum[2];
    }

    // the brick number, start brick position and end brick position in each dimension
    int startBrick[3], endBrick[3];
    for (int i = 0; i < 3; ++i) {
        bricksNum[i]  = static_cast<int>(ceil(float(originalDimensions[i]) / brickSize));
        startBrick[i] = static_cast<int>(floor(float(startPos[i]) / brickSize));
        endBrick[i]   = static_cast<int>(ceil(float(startPos[i] + dimensions[i]) / brickSize));
    }

    // read all brick position information at the current resolution level
    qint64 totalBricksNum = bricksNum[0] * bricksNum[1] * bricksNum[2]; 
    qint64 *bpiData = (qint64*)bpiFile.map(brickOffset * sizeof(qint64), totalBricksNum * sizeof(qint64));

    // read all brick equal values at all resolution levels
    qint64 bevSize = bevFile.size();
    char * bevData = (char*)bevFile.map(0, bevSize);

    // determine the brick number for the memory mapped brick volume file
    qint64 bytesPerVoxel = volume->bytesPerVoxel();
    qint64 bytesPerBrick = brickSize * brickSize * brickSize * bytesPerVoxel;
    qint64 mappedBricks = bricksNum[0] * bricksNum[1] * (endBrick[2] - startBrick[2]);
    qint64 mappedBytes  = mappedBricks * bytesPerBrick;
    qint64 mappedBytesLimit = 64 * 1024 * 1024;  // 64M
    while (mappedBytes > mappedBytesLimit) {
        mappedBricks = mappedBricks >> 1;
        mappedBytes  = mappedBricks * bytesPerBrick;
    }

    // read each brick to fill the volume
    qint64 bvStartPos = 0;
    qint64 bvSize     = bvFile.size();
    char * bvData     = 0;
    int brickStartPos[3];       // the start position of the brick in the current resolution level
    char *brickData  = new char[bytesPerBrick];
    char *volumeData = new char[dimensions[2] * dimensions[1] * dimensions[0] * bytesPerVoxel];
    qint64 position = 0;
    for (int brickZ = startBrick[2]; brickZ < endBrick[2]; ++brickZ) {
        brickStartPos[2] = brickZ * brickSize;
        for (int brickY = startBrick[1]; brickY < endBrick[1]; ++brickY) {
            brickStartPos[1] = brickY * brickSize;
            int brickIndex = (brickZ * bricksNum[1] + brickY) * bricksNum[0]; 
            for (int brickX = startBrick[0]; brickX < endBrick[0]; ++brickX) {
                brickStartPos[0] = brickX * brickSize;
                position = bpiData[brickIndex + brickX];
                // check the voxels in the brick are all equal
                bool equal = position & 1;
                position = position >> 1;
                if(equal) { // read the data from brick equal voxels
                    fillVolumeWithEqualBrick(volumeData, bevData + position, startPos, dimensions, brickStartPos, brickSize, bytesPerVoxel);
                } else {    // read the data from brick voxels
                    if(!bvData) {   // the first memory mapping
                        bvStartPos = position;
                        if (bvStartPos + mappedBytes > bvSize) mappedBytes = bvSize - bvStartPos;
                        bvData = (char *)bvFile.map(bvStartPos, mappedBytes); 
                    } else {
                        while (position >= bvStartPos + mappedBytes) { // the read brick is not in the current mapped bricks
                            bvStartPos += mappedBytes;
                            bvFile.unmap((uchar *)bvData);
                            if (bvStartPos + mappedBytes > bvSize) mappedBytes = bvSize - bvStartPos;
                            bvData = (char *)bvFile.map(bvStartPos, mappedBytes);
                        }
                    }
                    position -= bvStartPos;
                    fillVolumeWithBrick(volumeData, bvData + position, startPos, dimensions, brickStartPos, brickSize, bytesPerVoxel);
                }
            }
        }
    }

    delete []brickData;
    bpiFile.unmap((uchar *)bpiData);
    bevFile.unmap((uchar *)bevData);
    bvFile.unmap((uchar *)bvData);
    return volumeData;
}
