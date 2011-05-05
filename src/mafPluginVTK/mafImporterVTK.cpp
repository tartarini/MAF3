/*
 *  mafImporterVTK.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporterVTK.h"

#include <vtkDataSetReader.h>

using namespace mafPluginVTK;
using namespace mafResources;

mafImporterVTK::mafImporterVTK(const QString code_location) : mafImporter(code_location) {
    m_Reader = vtkDataSetReader::New();
}

mafImporterVTK::~mafImporterVTK() {
    m_Reader->Delete();
}

void mafImporterVTK::execute() {
    if (!filename().isEmpty()) {
        checkImportFile();
        if (m_Status == mafOperationStatusAborted) {
            cleanup();
            return;
        }
    }
    
    m_Reader->SetFileName(filename().toAscii().constData());
    m_Reader->Update();
    
    m_ImportedData = m_Reader->GetOutputPort();
    importedData(&m_ImportedData);
    
    emit executionEnded();
}

