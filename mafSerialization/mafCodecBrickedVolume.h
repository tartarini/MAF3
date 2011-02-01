/*
 *  mafCodecBrickedVolume.h
 *  mafSerialization
 *
 *  Created by Yubo Tao on 23/12/10.
 *  Copyright 2010 University of Bedfordshire. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */
#ifndef mafCodecBrickedVolume_H
#define mafCodecBrickedVolume_H

// Includes list
#include "mafCodecRaw.h"

namespace mafSerialization {

/**
 * Class name: mafCodecBrickedVolume
 * Codec to Encode/Decode maf volume objects into a bricked raw memory dump using multiresolution.
 * The meta information of the volume is stored in its media device, and the data item is the file
 * name. The raw volume stores its bicked data from the highest resolution to the lowest resolution
 * in three files. 
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
 * @sa mafCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafCodecBrickedVolume : public mafCodecRaw {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCodecRaw);

public:
    /// Object constructor.
    mafCodecBrickedVolume(const mafString code_location = "");

    /// Object destructor.
    ~mafCodecBrickedVolume();

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafCore::mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafCore::mafMemento *decode();

    /// Update the memento from the output type when the start position or dimension is changed
    /*virtual*/ void update(mafCore::mafMemento *memento);

private:
    /// marshalling of QVariant data type in order to write the correct type.
    void marshall(const QVariant &value );

    /// unmarshalling of QVariant data type in order to write the correct type.
    mafVariant demarshall(mafString typeName, int multiplicity);

    /** 
     * get the byte number of the data type
     * @param dataType  the data type of the component in voxels
     * @see mafDataType in mafSerializationDefinitions.h
     */
    int getByteNum(int dataType);

    /**
     * calculate the multiresolution level under the memory limit
     * @param dataType      the component's data type of the voxel
     * @param componentNum  the component number of the voxel
     * @param dimensions    the dimensions of the volume in the highest resolution
     * @param memoryLimit   the memory limit of the volume in Mega
     */
    int calcMultiresolutionLevel(int dataType, int componentNum, int dimensions[3], int memoryLimit);

    /**
     * resample the volume data by half
     * @param originalData   the volume data in the current resolution
     * @param resampledData  the memory to store the resampled volume data in the next resolution
     * @param dataType       the component's data type of the voxel
     * @param componentNum   the component number of the voxel
     * @param dimensions     the dimensions of the volume in the current resolution
     */
    void resample(void *originalData, void *resampledData, int dataType, int componentNum, int dimensions[3]);

    /**
     * check whether the updated volume is in the previous volume
     * @param startPos          the start positions of the previous volume in the current resolution
     * @param dimensions        the dimensions of the previous volume in the current resolution
     * @param updatedStartPos   the updated start positions in the current resolution
     * @param updatedDimensions the updated dimensions in the current resolution
     */
    bool inRange(int startPos[3], int dimensions[3], int updatedStartPos[3], int updatedDimensions[3]);

    /**
     * extract a partial volume data from the original volume data
     * @param data               the volume data to copy
     * @param dataType           the component's data type of the voxel
     * @param componentNum       the component number of the voxel
     * @param originalDimensions the dimensions of the original volume data     
     * @param startPos           the start positions of the extracted volume data in the original volume
     * @param dimensions         the dimensions of the extracted volume data
     */
    void *extractVolumeData(void *data, int dataType, int componentNum, int originalDimensions[3], int startPos[3], int dimensions[3]);

    /**
     * extract the brick data from the original volume data
     * @param data           the original volume data 
     * @param brickData      the brick volume data
     * @param dimensions     the dimensions of the original volume
     * @param brickIndex     the index of the brick
     * @param brickSize      the brick size
     * @param bytesPerVoxel  the byte number per voxel
     */
    void extractBrickData(char *data, char *brickData, int dimensions[3], int brickIndex[3], int brickSize, int bytesPerVoxel);

    /**
     * check whether the voxels in the brick are all the same
     * @param brickData     the brick volume data
     * @param dataType      the component's data type of the voxel
     * @param componentNum  the component number of the voxel
     * @param brickSize     the brick size
     */
    bool voxelsEqualInBrick(char *brickData, int dataType, int componentNum, int brickSize);

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
    void fillVolumeWithEqualBrick(char *volumeData, char *voxelValue, int volumeStartPos[3], int volumeDimensions[3], int brickStartPos[3], int brickSize, int bytesPerVoxel);

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
    void fillVolumeWithBrick(char *volumeData, char *brickData, int volumeStartPos[3], int volumeDimensions[3], int brickStartPos[3], int brickSize, int bytesPerVoxel);
    
    /**
     * encode the raw volume data in mulriresolution
     * @param url           the url to store bricked position information, bricked equal values, bricked volume
     * @param data          the volume data
     * @param dataType      the component's data type of the voxel
     * @param componentNum  the component number of the voxel
     * @param dimensions    the dimensions of the volume in the highest resolution
     * @param brickSize     the brick size
     * @param levels        the level number of multiresolution
     */
    void encode(mafString url, void *data, int dataType, int componentNum, int dimensions[3], int brickSize, int levels);

    /**
     * decode the raw volume data in the indicated level
     * @param url                the url to load the volume data
     * @param dataType           the component's data type of the voxel
     * @param componentNum       the component number of the voxel
     * @param originalDimensions the dimensions of the original volume in the highest resolution
     * @param brickSize          the brick size
     * @param startPos           the start positions to be loaded in the current resolution 
     * @param dimensions         the dimensions to be loaded in the current resolution
     * @param level              the multiresolution level to be loaded
     */
    void *decode(mafString url, int dataType, int componentNum, int originalDimensions[3], int brickSize, int startPos[3], int dimensions[3], int level);

    mafDataStream m_DataStreamRead;
    mafDataStream m_DataStreamWrite;
};

} // mafSerialization

#endif // MAFCODECBRICKEDVOLUME_H
