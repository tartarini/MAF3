/*
 *  mafVME.h
 *  mafResources
 *
 *  Created by Roberto Mucci - Paolo Quadrani on 30/12/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
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
#include <mafBounds.h>

namespace mafResources {

// Class forwarding list
class mafPipeData;
class mafMementoDataSet;
class mafInteractor;

/**
Class name: mafVME
This class defines the base class for the VMEs in MAF3. The VME is also
observer for the TIME_SET signal to be
notified when new timestamp has been assigned to the system.
*/
class MAFRESOURCESSHARED_EXPORT mafVME : public mafResource {
    Q_OBJECT
    Q_PROPERTY(bool canRead READ canRead WRITE setCanRead STORED false)
    Q_PROPERTY(bool canWrite READ canWrite WRITE setCanWrite STORED false)
    Q_PROPERTY(bool dataLoaded READ dataLoaded STORED false)
    Q_PROPERTY(QString boundXmin READ boundXmin WRITE setBoundXmin)
    Q_PROPERTY(QString boundYmin READ boundYmin WRITE setBoundYmin)
    Q_PROPERTY(QString boundZmin READ boundZmin WRITE setBoundZmin)
    Q_PROPERTY(QString boundXmax READ boundXmax WRITE setBoundXmax)
    Q_PROPERTY(QString boundYmax READ boundYmax WRITE setBoundYmax)
    Q_PROPERTY(QString boundZmax READ boundZmax WRITE setBoundZmax)

    Q_PROPERTY(double colorR READ colorR WRITE  setColorR)
    Q_PROPERTY(double colorG READ colorG WRITE  setColorG)
    Q_PROPERTY(double colorB READ colorB WRITE  setColorB)

    Q_PROPERTY(QString objectType READ objectType STORED false)
    
    Q_PROPERTY(mafResources::mafBoundsPointer bounds READ bounds STORED false)
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

    /// Allow to Q_EMIT the detached signal, so to alert all the observers that the vme is not more inside the tree.
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

    /// Set 3D bound of the current dataset containing the external data of the VME.
    void  setBoundXmin(QString bound);

    /// Set 3D bound of the current dataset containing the external data of the VME.
    void  setBoundYmin(QString bound);

    /// Set 3D bound of the current dataset containing the external data of the VME.
    void  setBoundZmin(QString bound);

    /// Set 3D bound of the current dataset containing the external data of the VME.
    void  setBoundXmax(QString bound);

    /// Set 3D bound of the current dataset containing the external data of the VME.
    void  setBoundYmax(QString bound);

    /// Set 3D bound of the current dataset containing the external data of the VME.
    void  setBoundZmax(QString bound);

    /// Return red component of vme color.
    double colorR() const;

    /// Return green component of vme color.
    double colorG() const;

    /// Return blue component of vme color.
    double colorB() const;

    /// set red component of vme color.
    void setColorR(double colorR);

    /// set green component of vme color.
    void setColorG(double colorG);

    /// set blue component of vme color.
    void setColorB(double colorB);

    /// Fill parameter 'b' with 3D bounds of the VME data at given time. Default value means current time.
    void bounds(double b[6], double t = -1);
    
    /// return 3d bounds.
    mafResources::mafBoundsPointer bounds() const;

    /// Return the length of the 3D bounding box of the data at the current timestamp.
    double length();
    
    /// retrieve object Type
    QString objectType() const;

Q_SIGNALS:
    /// Alert observers that the VME has been detatched from the hierarchy tree.
    void detatched();

    /// Notify the lock state for the current VME when the lock state change.
    void vmeLocked(int);

public Q_SLOTS:
    /// Set the current timestamp for the VME.
    void setTimestamp(double t);

    /// Execute the resource algorithm.
    /*virtual*/ void execute();

    /// Update bounds of the VME
    void updateBounds();

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
    double m_ColorR;                    ///< Red component of vme color.
    double m_ColorG;                    ///< Green component of vme color.
    double m_ColorB;                    ///< Blue component of vme color.
    QString m_BoundXmin;                ///< 3D bound of the current dataset
    QString m_BoundYmin;                ///< 3D bound of the current dataset
    QString m_BoundZmin;                ///< 3D bound of the current dataset
    QString m_BoundXmax;                ///< 3D bound of the current dataset
    QString m_BoundYmax;                ///< 3D bound of the current dataset
    QString m_BoundZmax;                ///< 3D bound of the current dataset


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

inline double mafVME::colorR() const {
    return m_ColorR;
}

inline double mafVME::colorG() const {
    return m_ColorG;
}

inline double mafVME::colorB() const {
    return m_ColorB;
}

inline void mafVME::setColorR(double colorR) {
    m_ColorR = colorR;
}

inline void mafVME::setColorG(double colorG) {
    m_ColorG = colorG;
}

inline void mafVME::setColorB(double colorB) {
    m_ColorB = colorB;
}
    
inline QString mafVME::objectType() const{
    QString str(this->metaObject()->className());
    return str.split("::").last();
}

inline void mafVME::setBoundXmin(QString bound) {
    m_BoundXmin = bound;
}

inline void mafVME::setBoundYmin(QString bound) {
    m_BoundYmin = bound;
}

inline void mafVME::setBoundZmin(QString bound) {
    m_BoundZmin = bound;
}

inline void mafVME::setBoundXmax(QString bound) {
    m_BoundXmax = bound;
}

inline void mafVME::setBoundYmax(QString bound) {
    m_BoundYmax = bound;
}

inline void mafVME::setBoundZmax(QString bound) {
    m_BoundZmax = bound;
}

} // mafResources

#endif // MAFVME_H
