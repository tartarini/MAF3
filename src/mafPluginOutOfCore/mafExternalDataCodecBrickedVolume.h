/*
 *  mafExternalDataCodecBrickedVolume.h
 *  mafSerialization
 *
 *  Created by Yubo Tao on 24/02/12.
 *  Copyright 2012 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEXTERNALDATACODECBRICKEDVOLUME_H
#define MAFEXTERNALDATACODECBRICKEDVOLUME_H

// Includes list
#include <mafSerialization_global.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>

// Class forwarding list

namespace mafSerialization {

/**
 * Codec to Encode/Decode mafVolume into a bricked raw memory dump using multiresolution.
 * The raw volume stores its bicked data from the highest resolution to the lowest resolution in
 * three files. 
 * (1) The bpi (birck position information) file is the meta information of each brick in a 64 bits. 
 *     The last bit is the flag indicated whether the voxels are all equal, and others bits is the 
 *     brick position in the other two files. 
 * (2) The bev (brick equal voxels) file stores the bricks with equal voxels, and only one voxel is
 *     stored in the bev file for each birck.
 * (3) The bv (brick voxels) file stores the bricks with different voxels, and the whole voxels of 
 *     each brick are stored in the bv file.
 * 
 * To decode the bricked raw volume, the memory limit is first used to determine which level of the 
 * volume data to be loaded, and then check the meta information of the brick and obtain the brick 
 * position information. If the voxels in the brick are all equal, read only one voxel value from
 * the bev file using the brick position and fill the brick with the read voxel value. Otherwise,
 * read the whole voxels of the brick from the bv file using the brick position.
 * 
 * @sa mafVolume,mafExternalDataCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafExternalDataCodecBrickedVolume : public mafCore::mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    mafExternalDataCodecBrickedVolume(const QString code_location = "");

    /// Encode external data into a file directly.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode external data from the file directly.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

protected:
    /// Object destructor.
    ~mafExternalDataCodecBrickedVolume();

    /// extract the brick data from the original volume data
    /**
     * extract the brick data from the original volume data
     * @param data           the original volume data 
     * @param brickData      the brick volume data
     * @param dimensions     the dimensions of the original volume
     * @param brickIndex     the index of the brick
     * @param brickSize      the brick size
     * @param bytesPerVoxel  the byte number per voxel
     */
    void extractBrickData(char *data, char *brickData, int dimensions[3], int brickIndex[3], int brickSize, qint64 bytesPerVoxel);

    /// check whether the voxels in the brick are all the same
    /**
     * check whether the voxels in the brick are all the same
     * @param brickData     the brick volume data
     * @param brickSize     the brick size
     * @param bytesPerVoxel the byte number per voxel
     */
    bool voxelsEqualInBrick(char *brickData, int brickSize, qint64 bytesPerVoxel);

    /// fill the (partial) volume with the equal value
    /**
     * fill the (partial) volume with the equal value
     * @param volumeData       the current volume data
     * @param voxelValue       the voxel value in the brick with all equal voxels
     * @param volumeStartPos   the start position of the current volume in the original volume
     * @param volumeDimensions the dimensions of the current volume
     * @param brickStartPos    the start position of the brick in the original volume
     * @param brickSize        the brick size
     * @param bytesPerVoxel    the byte number per voxel
     */
    void fillVolumeWithEqualBrick(char *volumeData, char *voxelValue, int volumeStartPos[3], int volumeDimensions[3], int brickStartPos[3], int brickSize, qint64 bytesPerVoxel);

    /// fill the (partial) volume with the brick data
    /**
     * fill the (partial) volume with the brick data
     * @param volumeData       the current volume data
     * @param brickData        the brick data
     * @param volumeStartPos   the start position of the current volume in the original volume
     * @param volumeDimensions the dimensions of the current volume
     * @param brickStartPos    the start position of the brick in the original volume
     * @param brickSize        the brick size
     * @param bytesPerVoxel    the byte number per voxel
     */
    void fillVolumeWithBrick(char *volumeData, char *brickData, int volumeStartPos[3], int volumeDimensions[3], int brickStartPos[3], int brickSize, qint64 bytesPerVoxel);
   

    /// decode the raw volume data from the indicated level
    /**
     * decode the raw volume data from the indicated level
     * @param startPos           the start positions to be loaded in the current resolution 
     * @param dimensions         the dimensions to be loaded in the current resolution
     * @param level              the multiresolution level to be loaded
     */
    void *decode(int startPos[3], int dimensions[3], int level);
};

} // mafSerialization

#endif // MAFEXTERNALDATACODECBRICKEDVOLUME_H