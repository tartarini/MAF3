/*
 *  mafImporterVTK.h
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFIMPORTERVTK_H
#define MAFIMPORTERVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafOperation.h>

namespace mafPluginVTK {

/**
 Class Name: mafImporterVTK
 This class allows to import VTK data into a mafVME through the serialization module.
 */
class MAFPLUGINVTKSHARED_EXPORT mafImporterVTK : public mafResources::mafOperation {
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);
    
public:
    /// Object constructor.
    mafImporterVTK(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// Allows to set the filename of the VTK data to import.
    void setFilename(const QString f);
    
    /// Return the filename of the VTK data to import.
    QString filename() const;
    
public slots:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();
    
    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();
    
protected:
    /// Object destructor.
    /* virtual */ ~mafImporterVTK();
    

private slots:
    /// Obtain the imported VTK data, put it into a mafVME add it to the data hierarchy.
    void importedVTKData(mafCore::mafContainerInterface *data);
    
private:
    /// Method called when the operation has been aborted from the user.
    void cleanup();
    
    QString m_Filename; ///< Filename of VTK data to import.
    mafResources::mafVME *m_VME; ///< Output VME;
    mafResources::mafDataSet *m_DataSet; ///< Dataset of output VME;
    volatile bool m_DataImported; ///< Flag used to monitor the imported data.
};
    
inline void mafImporterVTK::setFilename(const QString f) {
    m_Filename = f;
}
    
inline QString mafImporterVTK::filename() const {
    return m_Filename;
}

} // namespace mafPluginVTK


#endif // MAFIMPORTERVTK_H