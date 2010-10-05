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

namespace mafResources {

// Class forwarding list
class mafDataPipe;
class mafMementoDataSet;

/**
Class name: mafVME
This class defines the base class for the VMEs in MAF3. The VME is also observer for the TIME_SET signal to be
notified when new timestamp has been assigned to the system.
*/
class MAFRESOURCESSHARED_EXPORT mafVME : public mafResource {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafVME(const mafString code_location = "");

    /// Object destructor.
    /* virtual */ ~mafVME();

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

public slots:
    /// Execute the resource algorithm.
    /*virtual*/ bool execute();

    /// Set the current timestamp for the VME.
    void setTimestamp(double t);

    /// Load dataSet.
    void updateData();

private:
    mafDataSetCollection *m_DataSetCollection; ///< Collection of timestamped data posed on homogeneous matrices.
    mafDataPipe *m_DataPipe; ///< Data pipe associated with the VME and used to elaborate new data.
        mafHash<mafMementoDataSet *, double> m_MementoDatSetHash; ///< Hash of memento dataset and time.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafDataSetCollection *mafVME::dataSetCollection() {
    if(m_DataSetCollection == NULL) {
        m_DataSetCollection = new mafDataSetCollection(mafCodeLocation);
    }
    return m_DataSetCollection;
}

inline mafDataPipe *mafVME::dataPipe() {
    return m_DataPipe;
}


} // mafResources

#endif // MAFVME_H
