/*
 *  mafImporterFieldMLTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Paolo Quadrani on 30/11/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestConfig.h>
#include <mafTestSuite.h>
#include <mafImporterFieldML.h>
#include <mafDataSet.h>
#include <mafVME.h>
#include <mafVMEManager.h>
#include <mafOperationManager.h>
#include <mafProxy.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginFieldML;

class testFieldMLVMEAddObserver : public QObject {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(QObject);
    
public:
    /// Object constructor.
    testFieldMLVMEAddObserver();
    
public Q_SLOTS:
    void vmeImported(mafCore::mafObjectBase *vme);
};

testFieldMLVMEAddObserver::testFieldMLVMEAddObserver() {
    mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeImported(mafCore::mafObjectBase *)")
}

void testFieldMLVMEAddObserver::vmeImported(mafCore::mafObjectBase *vme) {
    QVERIFY(vme);
    qDebug() << mafTr("Imported VME: ") << vme->objectName();
    
    mafVME *v = qobject_cast<mafVME *>(vme);
    mafDataSet *dataset = v->dataSetCollection()->itemAtCurrentTime();
    /*mafProxy<vtkAlgorithmOutput> *output = mafProxyPointerTypeCast(vtkAlgorithmOutput, dataset->dataValue());
    vtkAlgorithm *producer = (*output)->GetProducer();
    vtkDataObject *data = producer->GetOutputDataObject(0);
    QVERIFY(data);*/
}

/**
 Class name: mafImporterFieldMLTest
 This class implements the test suite for mafImporterFieldML.
 */
class mafImporterFieldMLTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();        
        m_EventBus = mafEventBusManager::instance();
        
        m_FieldMLFile = MAF_DATA_DIR;
        m_FieldMLFile.append("/FieldML/mafImporterFieldMLTestData");

        m_VMEManager = mafVMEManager::instance();
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.request");

        m_OperationManager = mafOperationManager::instance();
        
        m_Observer = new testFieldMLVMEAddObserver();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_Observer;
        
        m_OperationManager->shutdown();
        m_VMEManager->shutdown();
        
        //restore vme manager status
        m_EventBus->notifyEvent("maf.local.resources.hierarchy.request");
        
        m_EventBus->shutdown();
        mafMessageHandler::instance()->shutdown();
    }
    
    /// Test the import of VTK file created.
    void importFieldMLFile();

private:
    QString m_FieldMLFile; ///< VTK filename to import.
    testFieldMLVMEAddObserver *m_Observer; ///< Observer class to intercept the imported vme.
    
    mafEventBusManager *m_EventBus;
    mafVMEManager *m_VMEManager;
    mafOperationManager *m_OperationManager;
};

void mafImporterFieldMLTest::importFieldMLFile() {
    QVariantList op;
    op.append(QVariant("mafPluginFieldML::mafImporterFieldML"));
    QVariantMap params;
    params.insert("filename", QVariant(m_FieldMLFile));
    op.push_back(params); // Needs push_back to preserve the QVariantList structure as second element of the main variant list.

    mafEventArgumentsList listToSend;
    listToSend.append(mafEventArgument(QVariantList, op));
    m_EventBus->notifyEvent("maf.local.resources.operation.executeWithParameters", mafEventTypeLocal, &listToSend);

    QTime dieTime = QTime::currentTime().addSecs(3);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 3);
    }
}

MAF_REGISTER_TEST(mafImporterFieldMLTest);
#include "mafImporterFieldMLTest.moc"
