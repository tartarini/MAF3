/*
 *  mafGUIManagerTest.cpp
 *  mafGUITest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 26/10/10.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUITestList.h"

using namespace mafCore;
using namespace mafGUI;

void mafGUIManagerTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    // Register all the creatable objects for the mafGUI module.
    mafGUIRegistration::registerGUIObjects();
    m_MainWin = new QMainWindow();

    mafLogger *logger = mafCore::mafMessageHandler::instance()->activeLogger();
    m_GUIManager = new mafGUIManager(m_MainWin, mafCodeLocation);
    //need to change the logger
    mafCore::mafMessageHandler::instance()->setActiveLogger(logger);
}

void mafGUIManagerTest::cleanupTestCase() {
    mafDEL(m_GUIManager);
    delete m_MainWin;
    mafEventBus::mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafGUIManagerTest::mafGUIManagerAllocationTest() {
    QVERIFY(m_GUIManager != NULL);
}

void mafGUIManagerTest::mafGUIManagerCreateMenuTest() {
    m_GUIManager->createMenus();
//    m_GUIManager->createToolBars();

    QObject *menu = m_GUIManager->menuItemByName("File");
    QVERIFY(menu != NULL);

    menu = m_GUIManager->menuItemByName("Edit");
    QVERIFY(menu != NULL);

    menu = m_GUIManager->menuItemByName("Help");
    QVERIFY(menu != NULL);

    menu = m_GUIManager->menuItemByName("Operations");
    QVERIFY(menu != NULL);
    QList<QAction*> items = ((QMenu *)menu)->actions();
    int num_items = items.count();
    QVERIFY(num_items == 0);

    menu = m_GUIManager->menuItemByName("Views");
    QVERIFY(menu != NULL);
    items = ((QMenu *)menu)->actions();
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

    QObject *menu = m_GUIManager->menuItemByName("Operations");
    QVERIFY(menu != NULL);
    QList<QAction*> items = ((QMenu *)menu)->actions();
    int num_items = items.count();
    QVERIFY(num_items == 1);

    menu = m_GUIManager->menuItemByName("Views");
    QVERIFY(menu != NULL);
    items = ((QMenu *)menu)->actions();
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

#include "mafGUIManagerTest.moc"
