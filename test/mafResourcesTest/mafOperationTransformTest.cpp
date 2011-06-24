/*
 *  mafOperationTransformTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 20/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafOperationTransform.h>
#include <mafVMEManager.h>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;

/**
 Class name: mafOperationTransformTest
 This class implements the test suite for mafOperationTransform.
 */

//! <title>
//mafOperationTransform
//! </title>
//! <description>
//mafOperationTransform will move a VME modifying its pose matrix.
//! </description>

class mafOperationTransformTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        tolerance = 0.000000001;
        
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();

        m_VMEManager = mafVMEManager::instance();
        
        // Create the parametric operation.
        m_OpTransform = mafNEW(mafResources::mafOperationTransform);
        
        m_VME = mafNEW(mafResources::mafVME);
        
        mafMatrix *newMatrix = new mafMatrix();
        newMatrix->setIdentity();

        m_DataSet = mafNEW(mafResources::mafDataSet);
        m_DataSet->setPoseMatrix(newMatrix);
        m_VME->dataSetCollection()->insertItem(m_DataSet);
        
        m_OpTransform->setInput(m_VME);
        m_OpTransform->initialize();

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSet);
        mafDEL(m_VME);
        mafDEL(m_OpTransform);
        m_VMEManager->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// Test the operation's execution.
    void testExecute();

   /// Test Set/Get method of transform operation
    void SetGetTest();

private:
    mafOperationTransform *m_OpTransform; ///< Transform operation.
    mafVME *m_VME; ///< represents the vme that will be moved
    mafVMEManager *m_VMEManager; ///< instance of mafVMEManager.
    mafDataSet *m_DataSet;
    double tolerance;
};

void mafOperationTransformTest::testExecute() {
    double x, y, z;
    double rx, ry, rz;
    
    x = 10.5;
    y = 2.35;
    z = 0.0;
    
    rx = 30.0;
    ry = 45.0;
    rz = 60.0;
    
    QVariantList parameters;
    parameters << rx;
    parameters << ry;
    parameters << rz;
    parameters << x;
    parameters << y;
    parameters << z;
    
    m_OpTransform->setParameters(parameters);
    
    // execute the operation
    m_OpTransform->execute();
    
    double pos[3], ori[3];
    m_VME->dataSetCollection()->position(pos);
    m_VME->dataSetCollection()->orientations(ori);
    
    
    
    QVERIFY((pos[0] - x) < tolerance && 
            (pos[1] - y) < tolerance &&
            (pos[2] - z) < tolerance &&
            (ori[0] - rx) < tolerance &&
            (ori[1] - ry) < tolerance &&
            (ori[2] - rz) < tolerance);
}

void mafOperationTransformTest::SetGetTest() {
    m_OpTransform->setXPos("4.6");
    m_OpTransform->setYPos("3.9");
    m_OpTransform->setZPos("0.8");
    
    double pos[3];
    pos[0] = m_OpTransform->xPos().toDouble();
    pos[1] = m_OpTransform->yPos().toDouble();
    pos[2] = m_OpTransform->zPos().toDouble();
    
    QVERIFY((pos[0] - 4.6) < tolerance && 
            (pos[1] - 3.9) < tolerance &&
            (pos[2] - 0.8) < tolerance);

    double ori[3];    
    m_OpTransform->setXRot("25");
    m_OpTransform->setYRot("55");
    m_OpTransform->setZRot("42.10");

    ori[0] = m_OpTransform->xRot().toDouble();
    ori[1] = m_OpTransform->yRot().toDouble();
    ori[2] = m_OpTransform->zRot().toDouble();
    
    QVERIFY((ori[0] - 25) < tolerance && 
            (ori[1] - 55) < tolerance &&
            (ori[2] - 42.10) < tolerance);

}


MAF_REGISTER_TEST(mafOperationTransformTest);
#include "mafOperationTransformTest.moc"

