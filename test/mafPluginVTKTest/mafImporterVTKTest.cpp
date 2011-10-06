/*
 *  mafImporterVTKTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafImporterVTK.h>
#include <mafDataSet.h>
#include <mafVMEManager.h>
#include <mafOperationManager.h>
#include <mafProxy.h>

#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkAlgorithm.h>
#include <vtkPolyDataWriter.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

class testVMEAddObserver : public QObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(QObject);
    
public:
    /// Object constructor.
    testVMEAddObserver();
    
public Q_SLOTS:
    void vmeImported(mafCore::mafObjectBase *vme);
};

testVMEAddObserver::testVMEAddObserver() {
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeImported(mafCore::mafObjectBase *)")
}

void testVMEAddObserver::vmeImported(mafCore::mafObjectBase *vme) {
    QVERIFY(vme);
    qDebug() << mafTr("Imported VME: ") << vme->objectName();
    
    mafVME *v = qobject_cast<mafVME *>(vme);
    mafDataSet *dataset = v->dataSetCollection()->itemAtCurrentTime();
    mafProxy<vtkAlgorithmOutput> *output = mafProxyPointerTypeCast(vtkAlgorithmOutput, dataset->dataValue());
    vtkAlgorithm *producer = (*output)->GetProducer();
    vtkDataObject *data = producer->GetOutputDataObject(0);
    QVERIFY(data);

}

/**
 Class name: mafImporterVTKTest
 This class implements the test suite for mafImporterVTK.
 */
class mafImporterVTKTest: public QObject {
    Q_OBJECT

    /// Prepare the test data to be used into the test suite.
    void initializeTestData();
    
private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();        
        m_EventBus = mafEventBusManager::instance();
        
        m_VMEManager = mafVMEManager::instance();
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.request");

        m_OperationManager = mafOperationManager::instance();
        
        m_Observer = new testVMEAddObserver();
        initializeTestData();

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        QFile::remove(m_VTKFile);
        delete m_Observer;
        
        m_OperationManager->shutdown();
        m_VMEManager->shutdown();
        
        //restore vme manager status
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.request");
        
        m_EventBus->shutdown();
        mafMessageHandler::instance()->shutdown();
    }
    
    /// Test the import of VTK file created.
    void importVTKFile();

private:
    QString m_VTKFile; ///< VTK filename to import.
    testVMEAddObserver *m_Observer; ///< Observer class to intercept the imported vme.
    
    mafEventBusManager *m_EventBus;
    mafVMEManager *m_VMEManager;
    mafOperationManager *m_OperationManager;
};

void mafImporterVTKTest::initializeTestData() {
    m_VTKFile = QDir::tempPath();
    m_VTKFile.append("/maf3TestData");
    QDir log_dir(m_VTKFile);
    if(!log_dir.exists()) {
        log_dir.mkpath(m_VTKFile);
    }
    m_VTKFile.append("/vtkImporterVTKData.vtk");
    vtkSmartPointer<vtkSphereSource> surfSphere = vtkSmartPointer<vtkSphereSource>::New();
    surfSphere->SetRadius(5);
    surfSphere->SetPhiResolution(10);
    surfSphere->SetThetaResolution(10);
    surfSphere->Update();
    
    vtkSmartPointer<vtkPolyDataWriter> writer = vtkPolyDataWriter::New();
    writer->SetInputConnection(surfSphere->GetOutputPort());
    QByteArray ba = m_VTKFile.toAscii();
    writer->SetFileName(ba.constData());
    writer->SetFileTypeToBinary();
    bool written = writer->Write() != 0;
    if (!written) {
        qCritical() << mafTr("Error writing test data file: ") << m_VTKFile;
    }
}

void mafImporterVTKTest::importVTKFile() {
    QVariantList op;
    op.append(QVariant("mafPluginVTK::mafImporterVTK"));
    QVariantMap params;
    params.insert("filename", QVariant(m_VTKFile));
    op.push_back(params); // Needs push_back to preserve the QVariantList structure as second element of the main variant list.

    mafEventArgumentsList listToSend;
    listToSend.append(mafEventArgument(QVariantList, op));
    m_EventBus->notifyEvent("maf.local.resources.operation.executeWithParameters", mafEventTypeLocal, &listToSend);

    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
    
}

MAF_REGISTER_TEST(mafImporterVTKTest);
#include "mafImporterVTKTest.moc"
