/*
 *  mafUILoaderTest.cpp
 *  mafGUITest
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafContainerInterface.h>
#include <mafEventBusManager.h>
#include <mafUILoader.h>
#include <QDebug>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

/**
  Class Name: testmafUILoaderCustom
  Custom UILoader for testing mafUILoader interface.
*/
class testmafUILoaderCustom : public mafUILoader {
    Q_OBJECT

public:
    ///object constructor
    testmafUILoaderCustom();
    /// method for load the file
    /*virtual*/ void uiLoad(const mafString& fileName);
    /// check if gui is loaded
    bool isUILoaded() {
        return m_IsUILoaded;
    }

public slots:
   ///return a value when the gui is loaded
   void uiLoaded(mafCore::mafContainerInterface *widget);

private:
     bool m_IsUILoaded;///< variable which represents if the ui is loaded
};

testmafUILoaderCustom::testmafUILoaderCustom() : mafUILoader(), m_IsUILoaded(false) {
}

void testmafUILoaderCustom::uiLoad(const mafString& fileName) {
    REQUIRE(!fileName.isEmpty());
    mafContainerInterface *gui = NULL;
    mafEventArgumentsList list;
    list.append(mafEventArgument(mafCore::mafContainerInterface *, gui));
    mafEventBusManager::instance()->notifyEvent("maf.local.gui.uiloaded", mafEventTypeLocal, &list);
}

void testmafUILoaderCustom::uiLoaded(mafCore::mafContainerInterface  *widget) {
    Q_UNUSED(widget);
    mafMsgDebug() << "ui loaded";
    m_IsUILoaded = true;
}

/**
 Class name: mafUILoaderTest
 This class implements the test suite for mafUILoader.
 */
class mafUILoaderTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_UILoader = new testmafUILoaderCustom();
        mafRegisterLocalCallback("maf.local.gui.uiloaded", m_UILoader, "uiLoaded(mafCore::mafContainerInterface *)");
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_UILoader;
        m_UILoader = NULL;
       mafEventBusManager::instance()->shutdown();
       mafMessageHandler::instance()->shutdown();
    }

    /// mafUILoader allocation test case.
    void mafUILoaderAllocationTest();

    /// mafUILoader allocation test case.
    void mafUILoaderUILoadTest();

private:
    testmafUILoaderCustom *m_UILoader; ///< Reference to the GUI Manager.
};

void mafUILoaderTest::mafUILoaderAllocationTest() {
    QVERIFY(m_UILoader != NULL);
}

void mafUILoaderTest::mafUILoaderUILoadTest() {
    m_UILoader->uiLoad("uiFileName");
    QVERIFY(m_UILoader->isUILoaded() == true);
}

MAF_REGISTER_TEST(mafUILoaderTest);
#include "mafUILoaderTest.moc"
