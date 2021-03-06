/*
 *  mafImporterCTKDICOM.cpp
 *  mafPluginCTK
 *
 *  Created by Roberto Mucci and Daniele Giunchi on 16/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafImporterCTKDICOM.h"
#include <ctkDICOMAppWidget.h>
#include <QDialog>
#include <QGridLayout>

using namespace mafPluginCTK;
using namespace mafResources;

mafImporterCTKDICOM::mafImporterCTKDICOM(const QString code_location) : mafImporter(code_location) {
	setProperty("wildcard", mafTr("All Files (*.*)"));
}

mafImporterCTKDICOM::~mafImporterCTKDICOM() {
}

bool mafImporterCTKDICOM::initialize() {
	ctkDICOMAppWidget appWidget;
    QGridLayout* formGridLayout = new QGridLayout();
    	
 	appWidget.setDatabaseDirectory(QDir::currentPath());
	appWidget.onAddToDatabase();
// 	appWidget.openImportDialog();
// 	appWidget.openExportDialog();
// 	appWidget.openQueryDialog();
	
    formGridLayout->addWidget(&appWidget);
    dicomDialog.setLayout(formGridLayout);
    appWidget.show();
    dicomDialog.exec();
    appWidget.setParent(NULL);

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
