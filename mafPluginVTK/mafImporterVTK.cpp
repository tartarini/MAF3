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

using namespace mafPluginVTK;
using namespace mafResources;

mafImporterVTK::mafImporterVTK(const QString code_location) : mafImporter(code_location) {
}

mafImporterVTK::~mafImporterVTK() {
}

void mafImporterVTK::execute() {
    setEncodeType("VTK");
    if (!filename().isEmpty()) {
        requestToImportFile();
        while (!m_DataImported || !m_Abort) {
            ;
        }
        if (m_Abort) {
            cleanup();
            return;
        }
    }
    
    emit executionEnded();
}

