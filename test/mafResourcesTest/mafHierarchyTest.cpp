/*
 *  mafHierarchyTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;


/**
 Class name: testVisualPipeCustom
 This class implements a custom visual pipe.
 */
class  testVisualPipeCustom : public mafPipeVisual {
    Q_OBJECT
    mafSuperclassMacro(mafResources::mafPipeVisual);

public:
    /// Object constructor;
    testVisualPipeCustom(const QString code_location = "") : mafPipeVisual(code_location) {};

    /// Initialize ans create the pipeline.
    /*virtual*/ void createPipe() {};

protected:
    /// Object destructor.
    /* virtual */ ~testVisualPipeCustom() {};

public Q_SLOTS:
    /// Allow to execute and update the pipeline when something change.
    /*virtual*/ void updatePipe(double t = -1) {Q_UNUSED(t);};

private:

};

void mafHierarchyTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_Hierarchy = mafNEW(mafCore::mafHierarchy);
    unbalancedTreeRandomCreation(3000); //Qt Creator gives problems in test machine with 10^5 nodes

    m_HierarchyToManage = mafNEW(mafCore::mafHierarchy);
}

void mafHierarchyTest::cleanupTestCase() {
    unbalancedTreeDestruction();
    mafDEL(m_Hierarchy);
    mafDEL(m_HierarchyToManage);
    mafEventBus::mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafHierarchyTest::mafSceneGraphAllocationTest() {
    QVERIFY(m_Hierarchy != NULL);
}

void mafHierarchyTest::mafScenegraphRootTest() {
    QVERIFY(m_HierarchyToManage != NULL);
    
    mafVME* vmeRoot = mafNEW(mafResources::mafVME);
    vmeRoot->setObjectName("root");
    QString pipeName = "testVisualPipeCustom";
    
    mafSceneNode *sceneNodeRoot = new mafSceneNode(vmeRoot, NULL,"", mafCodeLocation);
    sceneNodeRoot->initialize();
    sceneNodeRoot->setObjectName("sceneNodeRoot");
    m_HierarchyToManage->addHierarchyNode(sceneNodeRoot);
    
    
    mafVME* vmeChild0 = mafNEW(mafResources::mafVME);
    vmeChild0->setObjectName("vmeChild0");
    
    mafSceneNode *sceneNodeChild0 = new mafSceneNode(vmeChild0, NULL, "", mafCodeLocation);
    sceneNodeChild0->initialize();
    sceneNodeChild0->setObjectName("sceneNodeChild0");
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild0);

    bool result = m_HierarchyToManage->hasParent();
    QVERIFY(result);
    
    m_HierarchyToManage->moveTreeIteratorToParent();
    result = m_HierarchyToManage->hasParent();
    QVERIFY(result == false);
    
    // Remove the allocated objects
    mafDEL(sceneNodeRoot);
    mafDEL(sceneNodeChild0);
    mafDEL(vmeChild0);
    mafDEL(vmeRoot);
    
    // Clear the tree.
    m_HierarchyToManage->clear();
}


void mafHierarchyTest::mafSceneGraphManageTest() {
    QVERIFY(m_HierarchyToManage != NULL);

    mafVME* vmeRoot = mafNEW(mafResources::mafVME);
    vmeRoot->setObjectName("root");
    QString pipeName = "testVisualPipeCustom";

    mafSceneNode *sceneNodeRoot = new mafSceneNode(vmeRoot, NULL,pipeName, mafCodeLocation);
    sceneNodeRoot->initialize();
    sceneNodeRoot->setObjectName("sceneNodeRoot");
    m_HierarchyToManage->addHierarchyNode(sceneNodeRoot);

    mafVME* vmeChild0 = mafNEW(mafResources::mafVME);
    vmeChild0->setObjectName("vmeChild0");

    mafSceneNode *sceneNodeChild0 = new mafSceneNode(vmeChild0, NULL, pipeName, mafCodeLocation);
    sceneNodeChild0->initialize();
    sceneNodeChild0->setObjectName("sceneNodeChild0");
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild0);

    mafVME* vmeChild0Child0 = mafNEW(mafResources::mafVME);
    vmeChild0Child0->setObjectName("vmeChild0Child0");

    mafSceneNode *sceneNodeChild0Child0 = new mafSceneNode(vmeChild0Child0, NULL, pipeName, mafCodeLocation);
    sceneNodeChild0Child0->initialize();
    sceneNodeChild0Child0->setObjectName("sceneNodeChild0Child0");
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild0Child0);


    // Search the node by VME hash
    mafVisitorFindSceneNodeByVMEHash *v0 = new mafVisitorFindSceneNodeByVMEHash(sceneNodeChild0->vme()->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v0);
    mafSceneNode *n0 = v0->sceneNode();
    mafDEL(v0);
    m_HierarchyToManage->moveTreeIteratorToNode(n0);

    // Create a new child to add to the tree.
    mafVME* vmeChild1 = mafNEW(mafResources::mafVME);
    vmeChild1->setObjectName("vmeChild1");
    mafSceneNode *sceneNodeChild1 = new mafSceneNode(vmeChild1, NULL, pipeName, mafCodeLocation);
    sceneNodeChild1->setObjectName("sceneNodeChild1");

    // Add the created scene node to the tree.
    m_HierarchyToManage->addHierarchyNode(sceneNodeChild1);

    QString information;
    m_HierarchyToManage->printInformation(information);
    qDebug() << information;

    // Search the node by VME hash
    mafVisitorFindSceneNodeByVMEHash *v1 = new mafVisitorFindSceneNodeByVMEHash(sceneNodeChild0->vme()->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v1);
    mafSceneNode *n1 = v1->sceneNode();
    mafDEL(v1);
    if(n1) {
        // Remove the found node tree.
        m_HierarchyToManage->removeHierarchyNode(n1);
        qDebug() << "Remove SceneNode";
    }
    information.clear();

    // Dump the tree hierarchy
    m_HierarchyToManage->printInformation(information);
    qDebug() << information;

    // Remove the allocated objects
    mafDEL(sceneNodeRoot);
    mafDEL(sceneNodeChild0);
    mafDEL(sceneNodeChild0Child0);
    mafDEL(sceneNodeChild1);

    mafDEL(vmeChild1);
    mafDEL(vmeChild0Child0);
    mafDEL(vmeChild0);
    mafDEL(vmeRoot);

    // Clear the tree.
    m_HierarchyToManage->clear();
}

void mafHierarchyTest::mafSceneGraphBenchmarkMiddleTest() {
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(m_CustomHashInTheMiddle, mafCodeLocation);
    QBENCHMARK {
        mafObjectRegistry::instance()->findObjectsThreaded(v);
    }
    mafDEL(v);
}

void mafHierarchyTest::mafSceneGraphBenchmarkEndTest() {
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(m_CustomHashInTheEnd, mafCodeLocation);
    QBENCHMARK {
        mafObjectRegistry::instance()->findObjectsThreaded(v);
    }
    mafDEL(v);
}

void mafHierarchyTest::unbalancedTreeRandomCreation(unsigned int numberOfElements) {
    REQUIRE(numberOfElements != 0);
    m_Hierarchy->clear();
    
    mafVME* vmeRoot = mafNEW(mafResources::mafVME);
    vmeRoot->setObjectName("root");
    QString pipeName = "testVisualPipeCustom";
    mafSceneNode *sceneNodeRoot = new mafSceneNode(vmeRoot, NULL, pipeName, mafCodeLocation);
    sceneNodeRoot->initialize();
    m_Hierarchy->addHierarchyNode(sceneNodeRoot);
    m_VMEList.push_back((mafResources::mafVME *)sceneNodeRoot->vme());

    mafVME* vmeChild0 = mafNEW(mafResources::mafVME);
    vmeChild0->setObjectName("vmeChild0");
    mafSceneNode *sceneNodeChild0 = new mafSceneNode(vmeChild0, NULL, pipeName, mafCodeLocation);
    sceneNodeChild0->initialize();
    m_Hierarchy->addHierarchyNode(sceneNodeChild0);
    m_VMEList.push_back((mafResources::mafVME *)sceneNodeChild0->vme());

    unsigned int count = 1;
    enum {
      SIBLING = 0,
      CHILD = 1,
    };
    int max = CHILD;
    int min = SIBLING;
    int randomNumber = 0;
    while(count != numberOfElements) {
        //generate random number from which decides where to insert node
        randomNumber = (int)(rand()/(RAND_MAX + 1.0) * (max + 1 - min) + min);
        //basing on random number determine if iterator must create a child or a sibling
        if(randomNumber == SIBLING) {
            m_Hierarchy->moveTreeIteratorToParent();
        } //else it will be a children

        mafVME* vmeChild = mafNEW(mafResources::mafVME);
        QString name = "vmeChild";
        name.append(QString::number(count));
        vmeChild->setObjectName(name);
        mafSceneNode *sceneNode = new mafSceneNode(vmeChild, NULL, pipeName);
        sceneNode->initialize();

        m_VMEList.push_back((mafResources::mafVME *)sceneNode->vme());

        if(count == numberOfElements / 2) {
            m_CustomHashInTheMiddle == vmeChild->objectHash();
        }

        if(count == numberOfElements-10) {
            m_CustomHashInTheEnd == vmeChild->objectHash();
        }

        m_Hierarchy->addHierarchyNode(sceneNode);
        ++count;
        mafDEL(sceneNode);
    }

    mafDEL(sceneNodeRoot);
    mafDEL(sceneNodeChild0);
}

void mafHierarchyTest::unbalancedTreeDestruction() {
    mafVME *vme;
    Q_FOREACH(vme, m_VMEList) {
        mafDEL(vme);
    }
    m_VMEList.clear();
    m_Hierarchy->clear();
}

#include "mafHierarchyTest.moc"
