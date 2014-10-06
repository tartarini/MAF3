/*
 *  mafCodecVolume.h
 *  mafSerialization
 *
 *  Created by Yubo Tao on 21/12/10.
 *  Copyright 2010 University of Bedfordshire. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef mafCodecVolume_H
#define mafCodecVolume_H

// Includes list
#include "mafCodecRaw.h"

namespace mafSerialization {

/**
 * Class name: mafCodecVolume
 * Codec to Encode/Decode maf volume objects into a raw memory dump using
 * multiresolution. The meta information of the raw volume is stored in
 * its media device, and the data item is the file name. The raw volume
 * stores its data from the highest resolution to the lowest resolution in
 * one large file. In the decode, the memory limit is used to determine
 * which level of the volume data to be loaded.
 * @sa mafCodec, mafSerializationManager
 */
class MAFSERIALIZATIONSHARED_EXPORT mafCodecVolume : public mafCodecRaw {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCodecRaw);

public:
    /// Object constructor.
    mafCodecVolume(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ void encode(mafCore::mafMemento *memento);

    /// Decode the output type into the memento.
    /*virtual*/ mafCore::mafMemento *decode();

    /// Update the memento from the output type when the start position or dimension of the volume is changed
    /*virtual*/ void update(mafCore::mafMemento *memento);

protected:
    /// Object destructor.
    /* virtual */ ~mafCodecVolume();

private:
    /// marshalling of QVariant data type in order to write the correct type.
    void marshall(const QVariant &value );

    /// unmarshalling of QVariant data type in order to write the correct type.
    QVariant demarshall(QString typeName, int multiplicity);

    /// get the byte number of the data type
    /** 
     * get the byte number of the data type
     * @param dataType  the data type of the component in voxels
     * @see mafDataType in mafSerializationDefinitions.h
     */
    int getByteNum(int dataType);

    /// calculate the multiresolution level under the memory limit
    /**
     * calculate the multiresolution level under the memory limit
     * @param dataType      the component's data type of the voxel
     * @param componentNum  the component number of the voxel
     * @param dimensions    the dimensions of the volume in the highest resolution
     * @param memoryLimit   the memory limit of the volume in Mega
     */
    int calcMultiresolutionLevel(int dataType, int componentNum, int dimensions[3], int memoryLimit);

    /// resample the volume data by half
    /**
     * resample the volume data by half
     * @param originalData   the volume data in the current resolution
     * @param resampledData  the memory to store the resampled volume data in the next resolution
     * @param dataType       the component's data type of the voxel
     * @param componentNum   the component number of the voxel
     * @param dimensions     the dimensions of the volume in the current resolution
     */
    void resample(void *originalData, void *resampledData, int dataType, int componentNum, int dimensions[3]);

    /// check whether the updated volume is in the previous volume
    /**
     * check whether the updated volume is in the previous volume
     * @param startPos          the start positions of the previous volume in the current resolution
     * @param dimensions        the dimensions of the previous volume in the current resolution
     * @param updatedStartPos   the updated start positions in the current resolution
     * @param updatedDimensions the updated dimensions in the current resolution
     */
    bool inRange(int startPos[3], int dimensions[3], int updatedStartPos[3], int updatedDimensions[3]);

    /// extract a partial volume data from the original volume data
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

    /// encode the raw volume data in multiresolution
    /**
     * encode the raw volume data in multiresolution 
     * @param url           the url to store the volume data
     * @param data          the volume data
     * @param dataType      the component's data type of the voxel
     * @param componentNum  the component number of the voxel
     * @param dimensions    the dimensions of the volume in the highest resolution
     * @param levels        the level number of multiresolution
     */
    void encode(QString url, void *data, int dataType, int componentNum, int dimensions[3], int levels);

    /// decode the raw volume data from the indicated level
    /**
     * decode the raw volume data from the indicated level
     * @param url                the url to load the volume data
     * @param dataType           the component's data type of the voxel
     * @param componentNum       the component number of the voxel
     * @param originalDimensions the dimensions of the original volume in the highest resolution
     * @param startPos           the start positions to be loaded in the current resolution 
     * @param dimensions         the dimensions to be loaded in the current resolution
     * @param level              the multiresolution level to be loaded
     */
    void *decode(QString url, int dataType, int componentNum, int originalDimensions[3], int startPos[3], int dimensions[3], int level);

    QDataStream m_DataStreamRead;
    QDataStream m_DataStreamWrite;
};

} // mafSerialization

#endif // mafCodecVolume_H
