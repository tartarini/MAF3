/*
 *  mafExporterVTK.h
 *  mafPluginVTK
 *
 *  Created by Daniele Giunchi on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFEXPORTERVTK_H
#define MAFEXPORTERVTK_H

// Includes list
#include "mafPluginVTKDefinitions.h"
#include <mafOperation.h>

#include <mafProxy.h>
#include <vtkAlgorithmOutput.h>

class vtkDataSetWriter;

namespace mafPluginVTK {

/**
 Class Name: mafExporterVTK
 This class allows to export VTK data into a file through the serialization module.
 */
class MAFPLUGINVTKSHARED_EXPORT mafExporterVTK : public mafResources::mafOperation {
    Q_OBJECT
    Q_PROPERTY(QString filename READ filename WRITE setFilename)
    Q_PROPERTY(bool exportAllTimestamps READ exportAllTimestamps WRITE setExportAllTimestamps)

    /// typedef macro.
    mafSuperclassMacro(mafResources::mafOperation);
    
public:
    /// Object constructor.
    mafExporterVTK(const QString code_location = "");
    
    /// Accept function
    static bool acceptObject(mafCore::mafObjectBase *obj);
    
    /// Allows to set the filename of the VTK data to import.
    void setFilename(const QString f);
    
    /// Return the filename of the VTK data to import.
    QString filename() const;
    
    /// Return the 
    bool exportAllTimestamps() const;
    
    void setExportAllTimestamps(bool exportAll);
    
protected:
    /// Object destructor.
    /* virtual */ ~mafExporterVTK();
    
    /// Terminate the operation.
    /*virtual*/ void terminated();
        
    /// Method called when the operation has been aborted from the user.
    void cleanup();
    
private:
    QString m_Filename; ///< Filename of external data to import.
    
    public slots:
    /// Allows to call the piece of algorithm that is needed to restore the previous state of the operation's execution.
    /*virtual*/ void unDo();
    
    /// Allows to call the piece of algorithm that is needed to apply the operation again.
    /*virtual*/ void reDo();
    
    /// Set parameters of operation.
    /*virtual*/ void setParameters(QVariantList parameters);
    
public slots:
    /// Execute the resource algorithm.
    /*virtual*/ void execute();
    
private:
    mafCore::mafProxy<vtkAlgorithmOutput> m_ImportedData; ///< Container of the Data Source
    vtkDataSetWriter *m_Writer; ///< Reader of the external data.
    bool m_ExportAllTimestamps; ///< Flag used to check if the exporter has to export the current timestamp or all the timestamps.
};

inline bool mafExporterVTK::exportAllTimestamps() const {
    return m_ExportAllTimestamps;
}

inline void mafExporterVTK::setExportAllTimestamps(bool exportAll) {
    m_ExportAllTimestamps = exportAll;
}
    
} // namespace mafPluginVTK


#endif // MAFEXPORTERVTK_H
