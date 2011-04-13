/*
 *  mafImporterTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafImporter.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafImporterTest
 This class implements the test suite for mafImporter.
 */

//! <title>
// mafImporterTest
//! </title>
//! <description>
// mafImporterTest represents the test suite for mafImporter.
//! </description>
class mafImporterTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_Importer = mafNEW(mafResources::mafImporter);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_Importer);
        mafEventBus::mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafImporter allocation test case.
    void mafImporterAllocationTest();
    
    /// acceptObject test case.
    void acceptObjectTest();
    
    /// filename test.
    void filenameTest();

private:
    mafImporter *m_Importer; ///< Test var.
};

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
}

MAF_REGISTER_TEST(mafImporterTest);
#include "mafImporterTest.moc"
