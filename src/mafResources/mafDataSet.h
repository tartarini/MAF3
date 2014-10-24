/*
 *  mafDataSet.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 30/12/09.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFDATASET_H
#define MAFDATASET_H

// Includes list
#include "mafResourcesDefinitions.h"
#include "mafMementoDataSet.h"
#include <mafProxyInterface.h>
#include <mafBounds.h>

namespace mafResources {

// Class forwarding list
class mafDataBoundaryAlgorithm;

/**
 Class name: mafDataSet
 This is the base class for the MAF3 DataSet.
 It provides also 2 signals for data connected and disconnected.
 Each time that someone connect externally some data a dataValueConnected signal is emitted.
 When the external data is deleted or removed manually, a dataValueDisconnected has been emitted.
 */
class MAFRESOURCESSHARED_EXPORT mafDataSet : public mafCore::mafObject {
    Q_OBJECT
    Q_PROPERTY(QString externalDataType READ externalDataType WRITE setExternalDataType)
    Q_PROPERTY(QString externalCodecType READ externalCodecType WRITE setExternalCodecType)
    Q_PROPERTY(QString boundaryAlgorithmName READ boundaryAlgorithmName WRITE setBoundaryAlgorithmName)
    Q_PROPERTY(mafResources::mafBoundsPointer bounds READ bounds WRITE setBounds)
    Q_PROPERTY(QString poseMatrix READ poseMatrixString WRITE setPoseMatrixString)
    
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObject);

Q_SIGNALS:
    /// Signal emitted when data value has been added to the mafDataSet.
    void dataValueConnected();
    /// Signal emitted when data value has been removed from mafDataSet.
    void dataValueDisconnected();

    /// Emitted when mafDataSet needs to load the external data value.
    void loadDataSignal();

private Q_SLOTS:
    /// Load external data and fill the mafDataSet.
    void updateDataValue();

public:
    /// Object constructor.
    mafDataSet(const QString code_location = "");

    /// Return the data boundary.
    mafCore::mafProxyInterface *dataBoundary();

    /// Return the data value contained into the dataset.
    mafCore::mafProxyInterface *dataValue();

    /// Assign the given data value to the dataset.
    void setDataValue(mafCore::mafProxyInterface *data_value);

    /// Set the pose for the current data.
    void setPoseMatrix(const mafMatrix4x4 *matrix);

    /// Set the pose for the current data through matrixString.
    void setPoseMatrixString(const QString matrixString);

    /// Return the pose matrix of the current data.
    mafMatrix4x4 *poseMatrix();

    /// Return the pose matrixString of the current data.
    QString poseMatrixString();

    /// Set the boundary algorithm function to be used to calculate the data value boundary according also to its pose matrix.
    void setBoundaryAlgorithm(mafDataBoundaryAlgorithm *algorithm);

    /// Return the boundary algorithm function used to calculate the data value boundary
    mafDataBoundaryAlgorithm *boundaryAlgorithm() const;

    /// Set the name of mafDataBoundaryAlgorithm.
    void setBoundaryAlgorithmName(QString dataBoundaryAlgorithmName);

    /// Return the name of mafDataBoundaryAlgorithm;
    QString boundaryAlgorithmName() const;

    /// Set the bounds of the bounding box of the VME.
    void setBounds(mafResources::mafBoundsPointer bounds);

    /// Return the bounds of the bounding box of the VME.
    mafResources::mafBoundsPointer bounds();
    
    /// check if boundary is valid.
    bool isValidBounds();

    /// Update bounds of the bounding box.
    /*virtual*/ void updateBounds();

    /// Return the instance of the resource's status. The caller has to delete the allocated memory he asked.
    /*virtual*/ mafCore::mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization purposes.*/
    /*virtual*/ void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

    /// Return type of data contained in the mafDataSet.
    QString externalDataType() const; 

    /// Set type of data contained in the mafDataSet.
    void setExternalDataType(QString externalDataType);

    /// Return type of data contained in the mafDataSet.
    QString externalCodecType() const; 

    /// Set type of data contained in the mafDataSet.
    void setExternalCodecType(QString externalCodecType);

    /// Return true if data has been loaded.
    bool dataLoaded() const;

protected:
    /// Object destructor.
    /* virtual */ ~mafDataSet();

private:
    mafCore::mafProxyInterface *m_DataValue; ///< Data value present into the current dataset at current timestamp.
    mafCore::mafProxyInterface *m_DataBoundary; ///< Represent the boundary of current data value at current timestamp.
    mafMatrix4x4 *m_Matrix; ///< Pose matrix
    mafDataBoundaryAlgorithm *m_DataBoundaryAlgorithm; ///< Represent the class that will calculate the boundary of the m_DataValue and generate as output a boundary to be visualized.
    QString m_DataBoundaryAlgorithmName; ///< Name of the mafDataBoundaryAlgorithm.
    QString m_ExternalDataType; ///< Type of data contained in the mafDataSet.
    QString m_ExternalCodecType; ///< Type of codec used to encode/decode in the mafDataSet.
    bool m_DataLoaded; ///< Set true when data is loaded in memory.
    mafResources::mafBoundsPointer m_Bounds; ///< List of bounds value of the binding box.

    QString m_FileName; ///< Name of the file containing the datat.

};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafDataBoundaryAlgorithm *mafDataSet::boundaryAlgorithm() const {
    return m_DataBoundaryAlgorithm;
}


inline QString mafDataSet::boundaryAlgorithmName() const {
    return m_DataBoundaryAlgorithmName;
}

inline QString mafDataSet::externalDataType() const {
  return m_ExternalDataType;
}

inline void mafDataSet::setExternalDataType(QString externaldataType) {
  m_ExternalDataType = externaldataType;
}

inline QString mafDataSet::externalCodecType() const {
    return m_ExternalCodecType;
}

inline void mafDataSet::setExternalCodecType(QString externalCodecType) {
    m_ExternalCodecType = externalCodecType;
}

inline bool mafDataSet::dataLoaded() const {
    return m_DataLoaded;
}

inline mafResources::mafBoundsPointer mafDataSet::bounds()  {
    return m_Bounds;
}

inline bool mafDataSet::isValidBounds() {
    return m_Bounds->isObjectValid();
}

} //mafResources

#endif // MAFDATASET_H
