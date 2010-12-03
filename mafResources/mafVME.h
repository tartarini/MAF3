/*
 *  mafVME.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFVME_H
#define MAFVME_H

// Includes list
#include "mafResource.h"
#include "mafDataSetCollection.h"
#include "mafMementoVME.h"

#include <mafContainerInterface.h>

namespace mafResources {

// Class forwarding list
class mafDataPipe;
class mafMementoDataSet;
class mafInteractor;


/**
Class name: mafVME
This class defines the base class for the VMEs in MAF3. The VME is also observer for the TIME_SET signal to be
notified when new timestamp has been assigned to the system.
*/
class MAFRESOURCESSHARED_EXPORT mafVME : public mafResource {
    Q_OBJECT
    Q_PROPERTY(QVariantList bounds READ bounds WRITE setBounds)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafVME(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafVME();

    /// Set the bounds of the bounding box of the VME.
    void setBounds(mafVariantList bounds);

    /// Return the bounds of the bounding box of the VME.
    mafVariantList bounds();

    /// Return the collection of mafDataSet.
    mafDataSetCollection *dataSetCollection();

    /// Return the data pipe of mafDataSet.
    mafDataPipe *dataPipe();

    /// Assign a data pipe by giving its type as string.
    void setDataPipe(const mafString &pipe_type);

    /// Set the data pipe for the VME.
    void setDataPipe(mafDataPipe *pipe);

    /// Return the output data calculated through the data pipe or extracted from the collection.
    mafDataSet *outputData();
    
    /// Return the instance of the resource's status. The caller has to delete the allocated memory he asked.
    /*virtual*/ mafCore::mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization pourposes.*/
    /*virtual*/ void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

    /// Assign to the VME the interactor that will be used when user interact with the VME.
    void setInteractor(mafInteractor *i);

    /// Return the interactor associated with the VME.
    mafInteractor *interactor();

public slots:
    /// Set the current timestamp for the VME.
    void setTimestamp(double t);

    /// Load dataSet.
    void updateData();

    /// Update bounds of the bounding box.
    /*virtual*/ void updateBounds();

    /// Execute the resource algorithm.
    /*virtual*/ void execute();

private:
    mafInteractor *m_Interactor; ///< Custom interactor associated with the VME.
    mafDataSetCollection *m_DataSetCollection; ///< Collection of timestamped data posed on homogeneous matrices.
    mafDataPipe *m_DataPipe; ///< Data pipe associated with the VME and used to elaborate new data.
    mafHash<mafMementoDataSet *, double> m_MementoDataSetHash; ///< Hash of memento dataset and time.
    QVariantList m_Bounds; ///< List of bounds value of the binding box.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafDataPipe *mafVME::dataPipe() {
    return m_DataPipe;
}

inline mafInteractor *mafVME::interactor() {
    return m_Interactor;
}

inline QVariantList mafVME::bounds()  {
    return m_Bounds;
}

} // mafResources

#endif // MAFVME_H
