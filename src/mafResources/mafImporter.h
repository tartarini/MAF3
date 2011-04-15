/*
 *  mafImporter.h
 *  mafResources
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
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
    
public:
    /// Object constructor.
    mafImporter(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// Allows to set the filename of the VTK data to import.
    void setFilename(const QString f);
    
    /// Return the filename of the VTK data to import.
    QString filename() const;
    
public slots:
    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();
    
    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();
    
    /// Set parameters of operation.
    /*virtual*/ void setParameters(QVariantList parameters);

protected:
    /// Object destructor.
    /* virtual */ ~mafImporter();
    
    /// Request, through the mafEventBus, to import the data corresponding to the m_Filename.
    void requestToImportFile();
    
    /// Allow to assign the encode type for the external data to be imported.
    void setEncodeType(const QString encode_type);

    /// Method called when the operation has been aborted from the user.
    void cleanup();
    
    mafVME *m_VME;  ///< VME containing the imported data coming from the mafSerialization module.

    volatile bool m_DataImported; ///< Flag used to monitor the imported data.

private slots:
    /// Obtain the imported data, put it into a mafVME add it to the data hierarchy.
    virtual void importedData(mafCore::mafContainerInterface *data);
    
private:
    QString m_EncodeType; ///< Encode type of the external data to be imported (e.g. VTK, STL, ...)
    QString m_Filename; ///< Filename of external data to import.
};
    
inline void mafImporter::setFilename(const QString f) {
    m_Filename = f;
}
    
inline QString mafImporter::filename() const {
    return m_Filename;
}
    
inline void mafImporter::setEncodeType(const QString encode_type) {
    m_EncodeType = encode_type;
}

} // namespace mafPluginVTK


#endif // MAFIMPORTERVTK_H