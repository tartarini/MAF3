/*
 *  mafPluginManagerTest.cpp
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
#include <mafEventBusManager.h>
#include <mafPluginManager.h>
#include <mafDataPipe.h>
#include <mafPlugin.h>

#ifdef WIN32
    #ifdef QT_DEBUG
        #define TEST_LIBRARY_NAME "mafPluginTest_d.mafplugin"
    #else
        #define TEST_LIBRARY_NAME "mafPluginTest.mafplugin"
    #endif
#else
     #ifdef QT_DEBUG
        #define TEST_LIBRARY_NAME "libmafPluginTest_debug.mafplugin"
     #else
        #define TEST_LIBRARY_NAME "libmafPluginTest.mafplugin"
    #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;

//---------------------------------------------------------------------------------------------------------
class testPluginObserver : public mafObjectBase {
    Q_OBJECT
    /// Superclass typedef.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor
    testPluginObserver(const QString code_location = "");

    /// Object destructor
    /*virtual*/ ~testPluginObserver();

    /// Return the plugged pipe.
    mafDataPipe *pluggedPipe();

public slots:
    /// Slots that will receive the REGISTER_PLUGIN event.
    void pluggedObject(mafCore::mafPluggedObjectsHash pluginHash);

private:
    mafDataPipe *m_PluggedPipe; ///< Test var.
};

testPluginObserver::testPluginObserver(const QString code_location) : mafObjectBase(code_location), m_PluggedPipe(NULL) {
    mafRegisterLocalCallback("maf.local.resources.plugin.registerLibrary", this, "pluggedObject(mafCore::mafPluggedObjectsHash)");
}

testPluginObserver::~testPluginObserver() {
    mafDEL(m_PluggedPipe);
}

void testPluginObserver::pluggedObject(mafCore::mafPluggedObjectsHash pluginHash) {
    QString base_class("");
    mafPluggedObjectInformation objInfo;
    mafPluggedObjectsHash::iterator iter = pluginHash.begin();
    while(iter != pluginHash.end()) {
        objInfo = iter.value();
        base_class = iter.key();
        if(base_class == "mafResources::mafDataPipe" && m_PluggedPipe == NULL) {
            m_PluggedPipe = (mafDataPipe *)mafNEWFromString(objInfo.m_ClassType);
            m_PluggedPipe->setObjectName(objInfo.m_Label);
            break;
        }
        ++iter;
    }
}

mafDataPipe *testPluginObserver::pluggedPipe() {
    return m_PluggedPipe;
}

//---------------------------------------------------------------------------------------------------------


/**
 Class name: mafPluginManagerTest
 This class implements the test suite for mafPluginManager.
 */

//! <title>
//mafPluginManager
//! </title>
//! <description>
//mafPluginManager provides the engine for loading plug-ins and define
//the ID: REGISTER_PLUGIN used by external libraries to register their splugged objects.
//! </description>

class mafPluginManagerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafEventBusManager::instance();

        // Initialize the plug-in name with prefix and extension.
        m_PluginName.append(TEST_LIBRARY_NAME);

        //! <snippet>
        m_PluginManager = mafPluginManager::instance();
        //! </snippet>

        m_Observer = mafNEW(testPluginObserver);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        // Free allocated memory
        mafDEL(m_Observer);
        m_PluginManager->shutdown();
        // Shutdown eventbus singleton and core singletons.
        mafEventBusManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafPluginManager allocation test case.
    void mafPluginManagerAllocationTest();
    /// Test loading plug-in compiled as dynamic library
    void mafPluginManagerLoadPluginTest();
    /// Test the unload mechanism for the plugin manager.
//    void mafPluginManagerUnloadTest();

private:
    mafPluginManager *m_PluginManager; ///< Test var.
    QString m_PluginName; ///< Test var.
    testPluginObserver *m_Observer; ///< Test observer.
};

void mafPluginManagerTest::mafPluginManagerAllocationTest() {
    QVERIFY(m_PluginManager != NULL);
}

void mafPluginManagerTest::mafPluginManagerLoadPluginTest() {
    // Load the library containing the objects that I want to plug-in.
    //! <snippet>
    qDebug() << m_PluginName;
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, m_PluginName));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.loadLibrary", mafEventTypeLocal, &argList);
    //! </snippet>
    
    // Dump the plug-in information.
    mafPluginInfo info = m_PluginManager->pluginInformation(m_PluginName);
    qDebug() << "Plug-in Information:";
    qDebug() << "Varsion: " << info.m_Version;
    qDebug() << "Author: " << info.m_Author;
    qDebug() << "Vendor: " << info.m_Vendor;
    qDebug() << "Description: " << info.m_Description;

    mafDataPipe *dp = m_Observer->pluggedPipe();
    QVERIFY(dp != NULL);

    argList.clear();
    QString baseClassType("mafResources::mafDataPipe");
    argList.append(mafEventArgument(QString, baseClassType));
    mafPluggedObjectInformationList *pluggedObjectList = NULL;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafPluggedObjectInformationList *, pluggedObjectList);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.plugin.resourcesQuery", mafEventTypeLocal, &argList, &ret_val);

    QVERIFY(pluggedObjectList != NULL);

    int size = pluggedObjectList->count();
    QVERIFY(size > 0);

    delete pluggedObjectList;
    pluggedObjectList = NULL;

    mafPluggedObjectInformationList baseClassList;
    QString pluggedClassType("mafPluginTest::mafDataPipeSurfacePluginTest");
    m_PluginManager->queryBaseClassType(pluggedClassType, &baseClassList);

    size = baseClassList.count();
    QVERIFY(size == 1);

    mafPluggedObjectInformation objInfo = baseClassList.first();
    QString res("mafResources::mafDataPipe");
    QCOMPARE(res, objInfo.m_ClassType);
}

//void mafPluginManagerTest::mafPluginManagerUnloadTest() {
    // Unload the plug-in library.
//    m_PluginManager->unLoadPlugin(m_PluginName);
//}

//MAF_REGISTER_TEST(mafPluginManagerTest);
#include "mafPluginManagerTest.moc"
