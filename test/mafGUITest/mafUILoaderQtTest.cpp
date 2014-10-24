/*
 *  mafUILoaderQtTest.cpp
 *  mafGUITest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 26/10/10.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUITestList.h"

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

public Q_SLOTS:
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

void mafUILoaderQtTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    m_UILoader = new mafUILoaderQt(mafCodeLocation);
    m_LoadRequestor = new testUILoaderRequestor();
}


void mafUILoaderQtTest::cleanupTestCase() {
    delete m_LoadRequestor;
    m_LoadRequestor = NULL;
    mafDEL(m_UILoader);
    mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafUILoaderQtTest::mafUILoaderQtAllocationTest() {
    QVERIFY(m_UILoader != NULL);
}

void mafUILoaderQtTest::mafUILoaderQtUILoadTest() {
    QString uiFile(MAF_DATA_DIR);
    uiFile.append("/GUI/testUIFile.ui");
    m_UILoader->uiLoad(uiFile, 0);
    QVERIFY(m_LoadRequestor->widgetLoaded() != NULL);
}

#include "mafUILoaderQtTest.moc"
