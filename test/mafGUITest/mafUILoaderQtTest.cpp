/*
 *  mafUILoaderQtTest.cpp
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
#include <mafProxy.h>
#include <mafEventBusManager.h>
#include <mafUILoaderQt.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

/**
  Class Name: testmafUILoaderCustom
  Custom UILoader for testing mafUILoader interface.
*/
class testUILoaderRequestor : public QObject {
    Q_OBJECT

public:
    /// Object constructor
    testUILoaderRequestor();

    /// Return the inner m_Widget variable
    QWidget *widgetLoaded();

public slots:
    /// UI loader callback.
    void uiQtLoaded(mafCore::mafProxyInterface *widget);

private:
    QWidget *m_Widget; ///< Test var containing the loaded UI
};

testUILoaderRequestor::testUILoaderRequestor() : QObject(), m_Widget(NULL) {
    mafRegisterLocalCallback("maf.local.gui.uiloaded", this, "uiQtLoaded(mafCore::mafProxyInterface *)");
}

QWidget *testUILoaderRequestor::widgetLoaded() {
    return m_Widget;
}

void testUILoaderRequestor::uiQtLoaded(mafCore::mafProxyInterface *widget) {
    mafProxy<QWidget> *w = mafProxyPointerTypeCast(QWidget, widget);
    m_Widget = *w;
}

//----------------------------------------------------------------------

/**
 Class name: mafUILoaderQtTest
 This class implements the test suite for mafUILoaderQt.
 */
class mafUILoaderQtTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        m_UILoader = new mafUILoaderQt(mafCodeLocation);
        m_LoadRequestor = new testUILoaderRequestor();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        delete m_LoadRequestor;
        m_LoadRequestor = NULL;
        mafDEL(m_UILoader);
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafUILoader allocation test case.
    void mafUILoaderQtAllocationTest();

    /// mafUILoader allocation test case.
    void mafUILoaderQtUILoadTest();

private:
    mafUILoaderQt *m_UILoader; ///< Reference to the UI Loader.
    testUILoaderRequestor *m_LoadRequestor; ///< Variable containing the UI load requestor.
};

void mafUILoaderQtTest::mafUILoaderQtAllocationTest() {
    QVERIFY(m_UILoader != NULL);
}

void mafUILoaderQtTest::mafUILoaderQtUILoadTest() {
    m_UILoader->uiLoad("testUIFile.ui");
    QVERIFY(m_LoadRequestor->widgetLoaded() != NULL);
}

MAF_REGISTER_TEST(mafUILoaderQtTest);
#include "mafUILoaderQtTest.moc"
