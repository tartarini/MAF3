/*
 *  mafImporterVTKTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafImporterVTK.h>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataWriter.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafImporterVTKTest
 This class implements the test suite for mafImporterVTK.
 */
class mafImporterVTKTest: public QObject {
    Q_OBJECT

    /// Prepare the test data to be used into the test suite.
    void initializeTestData();
    
private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        initializeTestData();
        m_Importer = mafNEW(mafPluginVTK::mafImporterVTK);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        QFile::remove(m_VTKFile);
        mafDEL(m_Importer);
        mafEventBus::mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafImporterVTK allocation test case.
    void mafImporterVTKAllocationTest();

private:
    mafImporterVTK *m_Importer; ///< Test var.
    QString m_VTKFile; ///< VTK filename to import.
};

void mafImporterVTKTest::initializeTestData() {
    m_VTKFile = QDir::tempPath();
    m_VTKFile.append("/maf3TestData");
    QDir log_dir(m_VTKFile);
    if(!log_dir.exists()) {
        log_dir.mkpath(m_VTKFile);
    }
    m_VTKFile.append("/vtkImporterVTKData.vtk");
    vtkSmartPointer<vtkSphereSource> surfSphere = vtkSphereSource::New();
    surfSphere->SetRadius(5);
    surfSphere->SetPhiResolution(10);
    surfSphere->SetThetaResolution(10);
    surfSphere->Update();
    
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkPolyDataWriter::New();
    writer->SetInputConnection(surfSphere->GetOutputPort());
    writer->SetFileName(m_VTKFile.toAscii().constData());
    writer->SetFileTypeToBinary();
    bool written = writer->Write() != 0;
    if (!written) {
        qCritical() << mafTr("Error writing test data file: ") << m_VTKFile;
    }
}

void mafImporterVTKTest::mafImporterVTKAllocationTest() {
    QVERIFY(m_Importer != NULL);
}

MAF_REGISTER_TEST(mafImporterVTKTest);
#include "mafImporterVTKTest.moc"
