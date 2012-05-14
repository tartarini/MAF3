/*
 *  mafVolume.h
 *  mafResources
 *
 *  Created by Yubo Tao on 24/10/11.
 *  Copyright 2011 University of Bedfordshire. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVOLUME_H
#define MAFVOLUME_H

// Includes list
#include "mafResourcesDefinitions.h"

namespace mafResources {

/**
 Class name: mafVolume
 This is the MAF3 Volume class.
 */
class MAFRESOURCESSHARED_EXPORT mafVolume : public mafCore::mafObject {
    Q_OBJECT
    // properties of the original volume
    //Q_PROPERTY(mafDataType    dataType           READ dataType           WRITE setDataType)
    //Q_PROPERTY(int            bitsStored         READ bitsStored         WRITE setBitsStored)
    //Q_PROPERTY(bool           bigEndian          READ bigEndian          WRITE setBigEndian)
    //Q_PROPERTY(int            componentNum       READ componentNum       WRITE setComponentNum)
    //Q_PROPERTY(int           *originalDimensions READ originalDimensions WRITE setOriginalDimensions)
    //Q_PROPERTY(float         *spacing            READ spacing            WRITE setSpacing)
    //Q_PROPERTY(mafMatrix     *poseMatrix         READ poseMatrix         WRITE setPoseMatrix)
    // properties of the loaded volume in mafVolume
    //Q_PROPERTY(int           *startPosition      READ startPositions     WRITE setStartPositions)
    //Q_PROPERTY(int           *dimensions         READ dimensions         WRITE setDimensions)
    //Q_PROPERTY(int            level              READ level              WRITE setLevel)
    // properties of the volume file for loading / saving
    //Q_PROPERTY(QString        codecType          READ codecType          WRITE setCodecType)
    //Q_PROPERTY(int            levelNum           READ levelNum           WRITE setLevelNum)
    //Q_PROPERTY(bool           bricking           READ bricking           WRITE setBricking)
    //Q_PROPERTY(int            brickSize          READ brickSize          WRITE setBrickSize)
    //Q_PROPERTY(int            memoryLimit        READ memoryLimit        WRITE setMemoryLimit)
    //Q_PROPERTY(QString        fileName           READ fileName           WRITE setFileName)

    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

public:
    /// Object constructor.
    mafVolume(const QString code_location = "");

    /// Return the instance of the resource's status. The caller has to delete the allocated memory he asked.
    /*virtual*/ mafCore::mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization pourposes.*/
    /*virtual*/ void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

    /// Load external data and fill the mafVolume.
    void updateDataValue();

    /// Calculate the bytes per voxel
    int bytesPerVoxel() const;

    /// Calculate the multiresolution level under the memory limit
    /// @param update  set true to use the updated dimensions to calculate the multiresolution level, otherwise the dimensions
    int calcMultiresolutionLevel(bool update = false);

    /// resample the volume data by dimensions
    /**
     * resample the volume data by dimensions
     * @param resampledData  the memory to store the resampled volume data in the next resolution
     * @param dimensions     the dimensions of the volume in the current resolution
     */
    void resample(void *resampledData, int dimensions[3]);

    /// check the requirement of the updated volume
    bool updateDataValueRequired();

    /// check whether the updated volume is in the current volume
    /**
     * check whether the updated volume is in the current volume
     * @param startPos   the updated start positions in the current resolution
     * @param dimensions the updated dimensions in the current resolution
     */
    bool isPartOf(int startPos[3], int dimensions[3]) const;

    /// extract a partial volume data from the current volume
    /**
     * extract a partial volume data from the current volume
     * @param startPos    the start positions of the extracted volume data in the current volume
     * @param dimensions  the dimensions of the extracted volume data
     */
    void *extractVolumeData(int startPos[3], int dimensions[3]);

    /// Return the data value contained into the dataset.
    void *dataValue();

    /// Assign the given data value to the dataset.
    void setDataValue(void *data_value);

    /// Return data type of voxel in the mafVolume.
    mafDataType dataType() const; 

    /// Set data type of voxel in the mafVolume.
    void setDataType(mafDataType dataType);

    /// Return the number of bits stored in each voxel in the mafVolume.
    int bitsStored() const; 

    /// Set the number of bits stored in each voxel in the mafVolume.
    void setBitsStored(int bitsStored);

    /// Return true if data is formated in big endian in the mafVolume.
    bool bigEndian() const; 

    /// Set true if data if formated in big endian in the mafVolume.
    void setBigEndian(bool bigEndian);

    /// Return the number of the component in each voxel in the mafVolume.
    int componentNum() const; 

    /// Set the number of the component in each voxel in the mafVolume.
    void setComponentNum(int componentNum);

    /// Return the original dimensions of the volume in the volume file.
    int *originalDimensions(); 

    /// Set the original dimensions of the volume in the volume file.
    void setOriginalDimensions(int *originalDimensions);

    /// Return spacing of the volume in the mafVolume.
    float *spacing(); 

    /// Set spacing of the volume in the mafVolume.
    void setSpacing(float *spacing);

    /// Set the pose for the current data.
    void setPoseMatrix(const mafMatrix *matrix);

    /// Return the pose matrix of the current data.
    mafMatrix *poseMatrix();

    /// Return the start positions of the volume in the mafVolume.
    int *startPositions(); 

    /// Set the start positions of the volume in the mafVolume.
    void setStartPositions(int *startPositions);

    /// Return dimensions of the volume in the mafVolume.
    int *dimensions(); 

    /// Set dimensions of the volume in the mafVolume.
    void setDimensions(int *dimensions);

    /// Return the current level of the volume in the mafVolume.
    int level() const;

    /// Set the current level of the volume in mafVolume
    void setLevel(int level);

    /// Return the updated start position of the volume in the mafVolume in terms of the original dimensions.
    int *updatedStartPos();

    /// Set the updated start position of the volume in the mafVolume in terms of the original dimensions.
    void setUpdatedStartPos(int *updatedStartPos);

    /// Return the updated dimensions of the volume in the mafVolumein terms of the original dimensions.
    int *updatedDimensions();

    /// Set the updated dimensions of the volume in the mafVolume in terms of the original dimensions, set 0 to get the total volume.
    void setUpdatedDimensions(int *updatedDimensions);

    /// Return type of data contained in the mafDataSet.
    QString codecType() const; 

    /// Set type of data contained in the mafVolume.
    void setCodecType(QString codecType);

    /// Return the number of levels in the multi-resolution volume.
    int levelNum() const;

    // Set the number of levels in the multi-resolution volume.
    void setLevelNum(int levelNum);

    /// Return true if data is bricking in the volume file.
    bool bricking() const;

    /// Set true if data is bricking in the volume file.
    void setBricking(bool bricking);

    /// Return the size of a brick in the volume file.
    int brickSize() const;

    /// Set the size of a brick in the volume file.
    void setBrickSize(int brickSize);

    /// Return the memory limit of the multi-resolution volume.
    int memoryLimit() const; 

    /// Set the memory limit for the multi-resolution volume.
    void setMemoryLimit(int memoryLimit);

    /// Return the file name of the volume in the mafVolume.
    QString fileName() const;

    /// Set the file name of the volume in the mafVolume.
    void setFileName(QString fileName);


protected:
    /// Object destructor.
    /* virtual */ ~mafVolume();

private:
    void *       m_DataValue;               ///< Data value present into the current dataset at current timestamp.

    mafDataType  m_DataType;                ///< Data type of the voxel, such as unsigned char, short int.
    int          m_BitsStored;              ///< Bits stored in each voxel, such as 12 bits in short int.
    bool         m_BigEndian;               ///< Set true when data is big endian.
    int          m_ComponentNum;            ///< Number of the compoent in each voxel, such as 3 for RGB color volume.
    int          m_OriginalDimensions[3];   ///< Dimensions of the volume in the volume file.
    float        m_Spacing[3];              ///< Spacing of the volume.
    mafMatrix *  m_Matrix;                  ///< Pose matrix.

    int          m_StartPositions[3];       ///< Start positions of the volume in memory in the current level.
    int          m_Dimensions[3];           ///< dimensions of the volume in memory in the current level.
    int          m_Level;                   ///< Current level of the volume in the multi-resolution volume.

    int          m_UpdatedStartPos[3];      ///< New start positions of the volume in memory in the original/highest level.
    int          m_UpdatedDimensions[3];    ///< New dimensions of the volume in memory in the original/highest level, 0 for the original dimensions.
    
    QString      m_CodecType;               ///< Type of codec used to encode/decode in the mafDataSet.
    int          m_LevelNum;                ///< Number of levels in the multi-resolution volume int the volume file.
    bool         m_Bricking;                ///< Set true when data is bricking in the volume file.
    int          m_BrickSize;               ///< Size of a brick in the volume file.
    int          m_MemoryLimit;             ///< Memory limit for multiple resolution (the unit is K).
    QString      m_FileName;                ///< Name of the file containing the volume data.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafDataType mafVolume::dataType() const {
    return m_DataType;
}

inline void mafVolume::setDataType(mafDataType dataType) {
    m_DataType = dataType;
}

inline int mafVolume::bitsStored() const {
    return m_BitsStored;
}

inline void mafVolume::setBitsStored(int bitsStored) {
    m_BitsStored = bitsStored;
}

inline bool mafVolume::bigEndian() const {
    return m_BigEndian;
}

inline void mafVolume::setBigEndian(bool bigEndian) {
    m_BigEndian = bigEndian;
}

inline int mafVolume::componentNum() const {
    return m_ComponentNum;
}

inline void mafVolume::setComponentNum(int componentNum) {
    m_ComponentNum = componentNum;
}

inline int * mafVolume::originalDimensions() {
    return &(m_OriginalDimensions[0]);
}

inline void mafVolume::setOriginalDimensions(int *originalDimensions) {
    m_OriginalDimensions[0] = originalDimensions[0];
    m_OriginalDimensions[1] = originalDimensions[1];
    m_OriginalDimensions[2] = originalDimensions[2];
}

inline float* mafVolume::spacing() {
    return &(m_Spacing[0]);
}

inline void mafVolume::setSpacing(float *spacing) {
    m_Spacing[0] = spacing[0];
    m_Spacing[1] = spacing[1];
    m_Spacing[2] = spacing[2];
}

inline int * mafVolume::startPositions() {
    return &(m_StartPositions[0]);
}

inline void mafVolume::setStartPositions(int *startPositions) {
    m_StartPositions[0] = startPositions[0];
    m_StartPositions[1] = startPositions[1];
    m_StartPositions[2] = startPositions[2];
}

inline int * mafVolume::dimensions() {
    return &(m_Dimensions[0]);
}

inline void mafVolume::setDimensions(int *dimensions) {
    m_Dimensions[0] = dimensions[0];
    m_Dimensions[1] = dimensions[1];
    m_Dimensions[2] = dimensions[2];
}

inline int mafVolume::level() const {
    return m_Level;
}

inline void mafVolume::setLevel(int level) {
    m_Level = level;
}

inline int * mafVolume::updatedStartPos() {
    return m_UpdatedStartPos;
}

inline void mafVolume::setUpdatedStartPos(int *updatedStartPos) {
    m_UpdatedStartPos[0] = updatedStartPos[0];
    m_UpdatedStartPos[1] = updatedStartPos[1];
    m_UpdatedStartPos[2] = updatedStartPos[2];
}

inline int * mafVolume::updatedDimensions() {
    return m_UpdatedDimensions;
}

inline void mafVolume::setUpdatedDimensions(int *updatedDimensions) {
    m_UpdatedDimensions[0] = updatedDimensions[0];
    m_UpdatedDimensions[1] = updatedDimensions[1];
    m_UpdatedDimensions[2] = updatedDimensions[2];
}

inline QString mafVolume::codecType() const {
    return m_CodecType;
}

inline void mafVolume::setCodecType(QString codecType) {
    m_CodecType = codecType;
}

inline int mafVolume::levelNum() const {
    return m_LevelNum;
}

inline void mafVolume::setLevelNum(int levelNum) {
    m_LevelNum = levelNum;
}

inline bool mafVolume::bricking() const {
    return m_Bricking;
}

inline void mafVolume::setBricking(bool bricking) {
    m_CodecType = bricking ? "VOLUME_BRICKED_LOD" : "VOLUME_LOD";
    m_Bricking = bricking;
}

inline int mafVolume::brickSize() const {
    return m_BrickSize;
}

inline void mafVolume::setBrickSize(int brickSize) {
    m_BrickSize = brickSize;
}

inline int mafVolume::memoryLimit() const {
    return m_MemoryLimit;
}

inline void mafVolume::setMemoryLimit(int memoryLimit) {
    m_MemoryLimit = memoryLimit;
}

inline QString mafVolume::fileName() const {
    return m_FileName;
}

inline void mafVolume::setFileName(QString fileName) {
    m_FileName = fileName;
}

} //mafResources

#endif // MAFVOLUME_H