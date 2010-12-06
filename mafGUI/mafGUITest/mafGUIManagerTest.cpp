/*
 *  mafGUIManagerTest.cpp
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
#include <mafGUIRegistration.h>
#include <mafGUIManager.h>

using namespace mafCore;
using namespace mafGUI;

/**
 Class name: mafGUIManagerTest
 This class implements the test suite for mafGUIManager.
 */
class mafGUIManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        // Register all the creatable objects for the mafGUI module.
        mafGUIRegistration::registerGUIObjects();
        m_MainWin = new QMainWindow();
        m_GUIManager = new mafGUIManager(m_MainWin, mafCodeLocation);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_GUIManager);
        delete m_MainWin;
    }

    /// allocation test case.
    void mafGUIManagerAllocationTest();
    /// Creation menu test.
    void mafGUIManagerCreateMenuTest();
    /// Fill menu test.
    void mafGUIManagerFillMenuTest();
    /// Max recent file number test
    void maxRecentFileTest();

private:
    mafGUIManager *m_GUIManager; ///< Reference to the GUI Manager.
    QMainWindow *m_MainWin;
};

void mafGUIManagerTest::mafGUIManagerAllocationTest() {
    QVERIFY(m_GUIManager != NULL);
}

void mafGUIManagerTest::mafGUIManagerCreateMenuTest() {
    m_GUIManager->createMenus();
    m_GUIManager->createToolBars();

    QMenu *menu = m_GUIManager->fileMenu();
    QVERIFY(menu != NULL);

    menu = m_GUIManager->editMenu();
    QVERIFY(menu != NULL);

    menu = m_GUIManager->helpMenu();
    QVERIFY(menu != NULL);

    menu = m_GUIManager->operationMenu();
    QVERIFY(menu != NULL);
    mafList<QAction*> items = menu->actions();
    int num_items = items.count();
    QVERIFY(num_items == 0);

    menu = m_GUIManager->viewMenu();
    QVERIFY(menu != NULL);
    items = menu->actions();
    num_items = items.count();
    QVERIFY(num_items == 0);
}

void mafGUIManagerTest::mafGUIManagerFillMenuTest() {
    mafPluggedObjectInformation plug_info;
    mafPluggedObjectsHash hash;

    plug_info.m_ClassType = "mafMyView";
    plug_info.m_Label = "My Custom View";
    hash.insert("mafResources::mafView", plug_info);

    plug_info.m_ClassType = "mafMyOperation";
    plug_info.m_Label = "My Custom Operation";
    hash.insert("mafResources::mafOperation", plug_info);

    m_GUIManager->fillMenuWithPluggedObjects(hash);

    QMenu *menu = m_GUIManager->operationMenu();
    QVERIFY(menu != NULL);
    mafList<QAction*> items = menu->actions();
    int num_items = items.count();
    QVERIFY(num_items == 1);

    menu = m_GUIManager->viewMenu();
    QVERIFY(menu != NULL);
    items = menu->actions();
    num_items = items.count();
    QVERIFY(num_items == 1);
}

void mafGUIManagerTest::maxRecentFileTest() {
    //  Default number of maximum recent files is 5
    int m = m_GUIManager->maxRecentFiles();
    QVERIFY(m == 5);
    // Change the number to 10
    m_GUIManager->setMaxRecentFiles(10);
    m = m_GUIManager->maxRecentFiles();
    QVERIFY(m == 10);
}

MAF_REGISTER_TEST(mafGUIManagerTest);
#include "mafGUIManagerTest.moc"
