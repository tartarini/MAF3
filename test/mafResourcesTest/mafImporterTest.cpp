/*
 *  mafImporterTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

void mafImporterTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    
    m_Importer = mafNEW(mafResources::mafImporter);
}

void mafImporterTest::cleanupTestCase() {
    mafDEL(m_Importer);
    mafMessageHandler::instance()->shutdown();
}


void mafImporterTest::mafImporterAllocationTest() {
    QVERIFY(m_Importer != NULL);
    QVERIFY(m_Importer->canUnDo());
}

void mafImporterTest::acceptObjectTest() {
    bool res = mafImporter::acceptObject(NULL);
    QVERIFY(res);
}

void mafImporterTest::filenameTest() {
    QString import_file("myFileToImport.ext");
    m_Importer->setFilename(import_file);
    QString f = m_Importer->filename();
    QCOMPARE(import_file, f);
    
    m_Importer->setFilename("");
    QVariantMap parameters;
    parameters.insert("filename", QVariant(import_file));
    m_Importer->setParameters(parameters);

    f = m_Importer->filename();
    QCOMPARE(import_file, f);
}

#include "mafImporterTest.moc"
