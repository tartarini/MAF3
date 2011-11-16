/*
 *  mafImporterCTKDICOM.cpp
 *  mafPluginCTK
 *
 *  Created by Paolo Quadrani on 04/04/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporterCTKDICOM.h"
#include <ctkDICOMAppWidget.h>

using namespace mafPluginCTK;
using namespace mafResources;

mafImporterCTKDICOM::mafImporterCTKDICOM(const QString code_location) : mafImporter(code_location), m_AppWidget(NULL) {
	setProperty("wildcard", mafTr("All Files (*.*)"));
}

mafImporterCTKDICOM::~mafImporterCTKDICOM() {
}

bool mafImporterCTKDICOM::initialize() {
	m_AppWidget = new ctkDICOMAppWidget();
	
 	m_AppWidget->setDatabaseDirectory(QDir::currentPath());
	m_AppWidget->onAddToDatabase();
	m_AppWidget->openImportDialog();
	m_AppWidget->openExportDialog();
	m_AppWidget->openQueryDialog();
 
	m_AppWidget->show();
	
	return true;
}

void mafImporterCTKDICOM::execute() {
    QMutex mutex(QMutex::Recursive);
    QMutexLocker locker(&mutex);

    m_Status = mafOperationStatusExecuting;
    
    /*checkImportFile();
    if (m_Status == mafOperationStatusAborted) {
        cleanup();
        return;
    }*/
    
    Q_EMIT executionEnded();
}
