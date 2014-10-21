/*
 *  mafPluginTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

#define TEST_LIBRARY_NAME "mafPluginTest.mafplugin"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

void mafPluginTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafEventBusManager::instance();
    mafResourcesSingletons::mafSingletonsInitialize();
    // Initialize the plug-in name with prefix and extension.
    m_PluginName.append(TEST_LIBRARY_NAME);
    m_Plugin = NULL;
}


void mafPluginTest::cleanupTestCase() {
    // Free allocated memory
    mafDEL(m_Plugin);

    // Shutdown event bus singleton and core singletons.
    mafResourcesSingletons::mafSingletonsShutdown();

    //restore vme manager status
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request");

    mafEventBusManager::instance()->shutdown();
    mafMessageHandler::instance()->shutdown();
}

void mafPluginTest::mafPluginAllocationTest() {
    m_Plugin = new mafPlugin("myLibrary", mafCodeLocation);
    mafDEL(m_Plugin);

    // instantiate the plug in and load the library.
    m_Plugin = new mafPlugin(m_PluginName, mafCodeLocation);
    QVERIFY(m_Plugin != NULL);
}

void mafPluginTest::pluginRegistrationTest() {
    m_Plugin->registerPlugin();
    mafPluginInfo info = m_Plugin->pluginInfo();
    QVERIFY(info.m_Author.length() > 0);
}

#include "mafPluginTest.moc"
