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

#include <mafProxyInterface.h>

namespace mafResources {

// Class forwarding list
class mafPipeData;
class mafMementoDataSet;
class mafInteractor;


/**
Class name: mafVME
This class defines the base class for the VMEs in MAF3. The VME is also observer for the TIME_SET signal to be
notified when new timestamp has been assigned to the system.
*/
class MAFRESOURCESSHARED_EXPORT mafVME : public mafResource {
    Q_OBJECT
    Q_PROPERTY(bool canRead READ canRead WRITE setCanRead)
    Q_PROPERTY(bool canWrite READ canWrite WRITE setCanWrite)
    Q_PROPERTY(bool dataLoaded READ dataLoaded)
    Q_PROPERTY(QString boundXmin READ boundXmin)
    Q_PROPERTY(QString boundYmin READ boundYmin)
    Q_PROPERTY(QString boundZmin READ boundZmin)
    Q_PROPERTY(QString boundXmax READ boundXmax)
    Q_PROPERTY(QString boundYmax READ boundYmax)
    Q_PROPERTY(QString boundZmax READ boundZmax)


    /// typedef macro.
    mafSuperclassMacro(mafResources::mafResource);

public:
    /// Object constructor.
    mafVME(const QString code_location = "");

    /// Check if the object is equal to that passed as argument.
    /* virtual */ bool isEqual(const mafObjectBase *obj) const;

    /// Return the collection of mafDataSet.
    mafDataSetCollection *dataSetCollection();

    /// Return the data pipe of mafDataSet.
    mafPipeData *dataPipe();

    /// Assign a data pipe by giving its type as string.
    void setDataPipe(const QString &pipe_type);

    /// Set the data pipe for the VME.
    void setDataPipe(mafPipeData *pipe);

    /// Return the output data calculated through the data pipe or extracted from the collection.
    mafDataSet *outputData();
    
    /// Return the instance of the resource's status. The caller has to delete the allocated memory he asked.
    /*virtual*/ mafCore::mafMemento *createMemento() const;

    /// Allows setting a previous saved object's state.
    /**
     This is used to implement a sort of undo mechanism for the object's state, but can be used also by the
    serialization mechanism to serialize data into the selected storage type.
    The 'deep_memento' flag is used to avoid the copy of the object unique hash in normal operation like
    undo or copy/paste operations. The complete object save is instead needed for serialization purposes.*/
    /*virtual*/ void setMemento(mafCore::mafMemento *memento, bool deep_memento = false);

    /// Allow to emit the detached signal, so to alert all the observers that the vme is not more inside the tree.
    void detatchFromTree();

    /// Allow to lock/unlock the readability of the VME.
    void setCanRead(bool lock);

    /// Return the readability state for the VME.
    bool canRead() const;

    /// Allow to lock/unlock the ability to make changes to the VME.
    void setCanWrite(bool lock);

    /// Return the possibility to modify the VME.
    bool canWrite() const;

    /// Return true if data has been loaded.
    bool dataLoaded() const;

    /// Return 3D bound of the current dataset containing the external data of the VME.
    QString boundXmin();

    /// Return 3D bound of the current dataset containing the external data of the VME.
    QString boundYmin();

    /// Return 3D bound of the current dataset containing the external data of the VME.
    QString boundZmin();

    /// Return 3D bound of the current dataset containing the external data of the VME.
    QString boundXmax();

    /// Return 3D bound of the current dataset containing the external data of the VME.
    QString boundYmax();

    /// Return 3D bound of the current dataset containing the external data of the VME.
    QString boundZmax();

    /// Return the length of the 3D bounding box of the data at the current timestamp.
    double length();

signals:
    /// Alert observers that the VME has been detatched from the hierarchy tree.
    void detatched();

    /// Notify the lock state for the current VME when the lock state change.
    void vmeLocked(int);

public slots:
    /// Set the current timestamp for the VME.
    void setTimestamp(double t);

    /// Execute the resource algorithm.
    /*virtual*/ void execute();

protected:
    /// Object destructor.
    /* virtual */ ~mafVME();

    /// Return a list mafMementoDataSet contained in memento passed as argument.
    void mementoDataSetMap(mafCore::mafMemento *memento, QMap<double, mafMementoDataSet*> &mementoMap);

private:
    mafDataSetCollection *m_DataSetCollection; ///< Collection of timestamped data posed on homogeneous matrices.
    mafPipeData *m_DataPipe;            ///< Data pipe associated with the VME and used to elaborate new data.
    QHash<mafMementoDataSet *, double> m_MementoDataSetHash; ///< Hash of memento dataset and time.
    bool m_CanRead;                     ///< Flag used to indicate if the VME is readable.
    bool m_CanWrite;                    ///< Flag indicating if the vme is writable.
};

/////////////////////////////////////////////////////////////
// Inline methods
/////////////////////////////////////////////////////////////

inline mafPipeData *mafVME::dataPipe() {
    QReadLocker locker(m_Lock);
    return m_DataPipe;
}

inline bool mafVME::canRead() const {
    QReadLocker locker(m_Lock);
    return m_CanRead;
}

inline bool mafVME::canWrite() const {
    QReadLocker locker(m_Lock);
    return m_CanWrite;
}

} // mafResources

#endif // MAFVME_H
