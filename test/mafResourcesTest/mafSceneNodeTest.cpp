/*
 *  mafSceneNodeTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafSceneNode.h>
#include <mafPipeVisual.h>

using namespace mafCore;
using namespace mafResources;

//------------------------------------------------------------------------------------------
/**
 Class name: testVisualPipeCustomForSceneNode
 This class implements the data pipe to be tested.
 */
class  testVisualPipeCustomForSceneNode : public  mafResources::mafPipeVisual {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
    /// Object constructor.
    testVisualPipeCustomForSceneNode(const QString code_location = "");

    /// Return the string variable initialized and updated from the data pipe.
    QString pipeline() {return m_PipeLine;}
    
    /// set the selection pipe for viualization.
    virtual void setPipeVisualSelection(mafPipeVisual *pipeVisualSelection);

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);
    
    /// Allows to ask the rendering engine to render the graphic scene.
    /*virtual*/ void render();

private:
    QString m_PipeLine; ///< Test Var.
};

testVisualPipeCustomForSceneNode::testVisualPipeCustomForSceneNode(const QString code_location) : mafPipeVisual(code_location), m_PipeLine("Created") {
}

void testVisualPipeCustomForSceneNode::setPipeVisualSelection(mafPipeVisual *pipeVisualSelection) {
    Q_UNUSED(pipeVisualSelection);
}

void testVisualPipeCustomForSceneNode::render() {
    
}

void testVisualPipeCustomForSceneNode::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(QString::number(t));
}
//------------------------------------------------------------------------------------------


/**
 Class name: mafSceneNodeTest
 This class implements the test suite for mafSceneNode.
 */
class mafSceneNodeTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObject(testVisualPipeCustomForSceneNode);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafMessageHandler::instance()->shutdown();
    }

    /// set visual pipe test case.
    void mafSceneNodeSetVisualPipeTest();

};


void mafSceneNodeTest::mafSceneNodeSetVisualPipeTest() {
    mafVME* vme = mafNEW(mafResources::mafVME);
    vme->setObjectName("vme");
    mafSceneNode *sceneNode = new mafSceneNode(vme, NULL,  "testVisualPipeCustomForSceneNode", mafCodeLocation);
    sceneNode->setVisibility(true);

    QVERIFY(sceneNode->visualPipe() != NULL);
    mafDEL(sceneNode);
    mafDEL(vme);
}

MAF_REGISTER_TEST(mafSceneNodeTest);
#include "mafSceneNodeTest.moc"

