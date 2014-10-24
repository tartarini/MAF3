/*
 *  mafOperationTransformTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 20/06/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafResources;
using namespace mafEventBus;

void mafOperationTransformTest::initTestCase() {        
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();

    m_VMEManager = mafVMEManager::instance();
    
    // Create the parametric operation.
    m_OpTransform = mafNEW(mafResources::mafOperationTransform);
    
    m_VME = mafNEW(mafResources::mafVME);
    
    mafMatrix4x4 *newMatrix = new mafMatrix4x4();
    newMatrix->setToIdentity();

    m_DataSet = mafNEW(mafResources::mafDataSet);
    m_DataSet->setPoseMatrix(newMatrix);
    m_VME->dataSetCollection()->insertItem(m_DataSet);
    
    m_OpTransform->setInput(m_VME);
    m_OpTransform->initialize();

}

void mafOperationTransformTest::cleanupTestCase() {
    mafDEL(m_DataSet);
    mafDEL(m_VME);
    mafDEL(m_OpTransform);
    m_VMEManager->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafOperationTransformTest::testExecute() {
    double x, y, z;
    double rx, ry, rz;
    
    x = 10.5;
    y = 2.35;
    z = 0.0;
    
    rx = 30.0;
    ry = 45.0;
    rz = 60.0;
    
    QVariantMap parameters;
    parameters.insert( "xPos_text" , x);
    parameters.insert( "yPos_text" , y);
    parameters.insert( "zPos_text" , z);
    parameters.insert( "xRot_text" , rx);
    parameters.insert( "yRot_text" , ry);
    parameters.insert( "zRot_text" , rz);
    
    m_OpTransform->setParameters(parameters);
    
    // execute the operation
    m_OpTransform->execute();
    
    double pos[3], ori[3];
    m_VME->dataSetCollection()->position(pos);
    m_VME->dataSetCollection()->orientations(ori);
    
    double tolerance = 1.0e-14;
    
    bool result = mafEquals(pos[0], x);
    QVERIFY(result);
    result = mafEquals(pos[1], y);
    QVERIFY(result);
    result = mafEquals(pos[2], z);
    QVERIFY(result);
    result = mafEquals(ori[0], rx, tolerance);
    QVERIFY(result);
    result = mafEquals(ori[1], ry, tolerance);
    QVERIFY(result);
    result = mafEquals(ori[2], rz, tolerance);
    QVERIFY(result);

}

void mafOperationTransformTest::SetGetTest() {
    m_OpTransform->setXPos("4.6");
    m_OpTransform->setYPos("3.9");
    m_OpTransform->setZPos("0.8");
    
    double pos[3];
    pos[0] = m_OpTransform->xPos().toDouble();
    pos[1] = m_OpTransform->yPos().toDouble();
    pos[2] = m_OpTransform->zPos().toDouble();
    
    QVERIFY(mafEquals(pos[0], 4.6) && 
            mafEquals(pos[1], 3.9) &&
            mafEquals(pos[2], 0.8));

    double ori[3];    
    m_OpTransform->setXRot("25");
    m_OpTransform->setYRot("55");
    m_OpTransform->setZRot("42.10");

    ori[0] = m_OpTransform->xRot().toDouble();
    ori[1] = m_OpTransform->yRot().toDouble();
    ori[2] = m_OpTransform->zRot().toDouble();
    
    QVERIFY(mafEquals(ori[0], 25) && 
            mafEquals(ori[1], 55) &&
            mafEquals(ori[2], 42.10));

}


#include "mafOperationTransformTest.moc"


