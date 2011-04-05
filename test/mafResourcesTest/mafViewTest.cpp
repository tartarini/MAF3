/*
 *  mafViewTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafResourcesSingletons.h>
#include <mafContainer.h>
#include <mafView.h>
#include <mafVME.h>
#include <mafSceneNode.h>
#include <mafVisitorFindSceneNodeByVMEHash.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

/**
 Class name: mafViewTest
 This class implements the test suite for mafView.
 */

//! <title>
//mafView
//! </title>
//! <description>
//mafView defines the base class for the view in MAF3.
//! </description>

class mafViewTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesSingletons::mafSingletonsInitialize();

        mafResourcesRegistration::registerResourcesObjects();
        m_View = mafNEW(mafResources::mafView);
        m_View->create();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_View);
        mafResourcesSingletons::mafSingletonsShutdown();
        mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.create");
        mafMessageHandler::instance()->shutdown();
    }

    /// mafView allocation test case.
    void mafViewAllocationTest();

    /// mafView add & remove mafSceneNode test case.
    void mafViewAddRemoveSceneNodeTest();

    /// mafView show scene node test case.
    void mafViewShowSceneNodeTest();

    /// mafView show scene node test case.
    void mafViewPlugVisualPipeTest();

    /// mafView setRenderingWidget test case.
    void mafViewRenderingWidgetTest();

private:
    mafView *m_View; ///< Test var.
};

void mafViewTest::mafViewAllocationTest() {
    QVERIFY(m_View != NULL);
}

void mafViewTest::mafViewAddRemoveSceneNodeTest() {
    mafVME *vme = mafNEW(mafResources::mafVME);

    //add VME
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    // remove VME => authomatic will be removed the corresponding scene node
    // from the view and destroyed the eventually created visual pipe.
    mafDEL(vme);
}

void mafViewTest::mafViewShowSceneNodeTest() {
    mafVME *vme = mafNEW(mafResources::mafVME);
    mafSceneNode *node = new mafSceneNode(vme, NULL, mafCodeLocation);

    //try to show a node not added with a fake visual pipe.
    m_View->showSceneNode(node, true,"mafPipesLibrary::mafPipeVisualVTKSurface");

    mafDEL(node);

    // add vme and show (can not show, thos is a base class)
    //add VME
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, vme));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    // search the scene node associated with the vme added.
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(vme->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v);
    node = v->sceneNode();
    mafDEL(v);

    m_View->showSceneNode(node, false,"mafPipesLibrary::mafPipeVisualVTKSurface");

    // Remove the VME and its related classes.
    mafDEL(vme);
}

void mafViewTest::mafViewPlugVisualPipeTest() {
    // ?!?
    m_View->plugVisualPipe("vtkPolyData","mafPipesLibrary::mafPipeVisualVTKSurface");
}

void mafViewTest::mafViewRenderingWidgetTest() {
    // Create a fake widget...
    mafObject *obj = mafNEW(mafCore::mafObject);
    mafContainer<mafObject> w;
    w = obj;
    w->setObjectName("My Widget");

    // Assign the widget to the mafView.
    QVariant vw;
    vw.setValue<mafContainerInterfacePointer>(&w);
    m_View->setRenderingWidget(vw);

    // Try to retrieve the widget through the properties API
    mafObjectBase *v = m_View;
    // Result variable.
    mafContainerInterfacePointer resultWidget;

    resultWidget = v->property("renderWidget").value<mafCore::mafContainerInterfacePointer>();

    mafObject *resultObject = mafContainerPointerTypeCast(mafCore::mafObject, resultWidget)->externalData();

    QString name_result = resultObject->objectName();
    QCOMPARE(name_result, QString("My Widget"));

    mafDEL(obj);

}

MAF_REGISTER_TEST(mafViewTest);
#include "mafViewTest.moc"
