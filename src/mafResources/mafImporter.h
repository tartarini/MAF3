/*
 *  mafImporter.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFIMPORTER_H
#define MAFIMPORTER_H

// Includes list
#include "mafOperation.h"

namespace mafResources {

// Class forwarding
class mafVME;
class mafDataSet;
    
/**
 Class Name: mafImporter
 Base class of all the importers defined in MAF3.
 */
class MAFRESOURCESSHARED_EXPORT mafImporter : public mafOperation {
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);

public Q_SLOTS:
    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();
    
    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();
    
public:
    /// Object constructor.
    mafImporter(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// Allows to set the filename of the VTK data to import.
    void setFilename(const QString f);
    
    /// Return the filename of the VTK data to import.
    QString filename() const;
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporter();
    
    /// Terminate the operation.
    /*virtual*/ void terminated();
    
    /// Request, through the mafEventBus, to import the data corresponding to the m_Filename.
    void checkImportFile();
    
    /// Method called when the operation has been aborted from the user.
    void cleanup();
    
    /// Obtain the imported data, put it into a mafVME add it to the data hierarchy.
    virtual void importedData(mafCore::mafProxyInterface *data);
    
    mafVME *m_VME;  ///< VME containing the imported data coming from the mafSerialization module.

private:
    QString m_Filename; ///< Filename of external data to import.

};
    
inline void mafImporter::setFilename(const QString f) {
    QMutex mutex(QMutex::Recursive);
    QMutexLocker locker(&mutex);
    m_Filename = f;
}
    
inline QString mafImporter::filename() const {
    QMutex mutex(QMutex::Recursive);
    QMutexLocker locker(&mutex);
    return m_Filename;
}

} // namespace mafPluginVTK


#endif // MAFIMPORTERVTK_H