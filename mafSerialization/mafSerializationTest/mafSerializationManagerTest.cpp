/*
 *  mafTimerTest.cpp
 *  mafTimeSpaceTest
 *
 *  Created by Paolo Quadrani on 22/09/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include "mafSerializationManager.h"
#include <mafEventBusManager.h>
#include <mafCoreSingletons.h>

#ifdef WIN32
    #define SERIALIZATION_LIBRARY_NAME "mafSerialization_d.dll"
#else
    #ifdef __APPLE__
        #define SERIALIZATION_LIBRARY_NAME "libmafSerialization_debug.dylib"
    #else
        #define SERIALIZATION_LIBRARY_NAME "libmafSerialization_debug.so"
    #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafSerialization;

/**
Class name: testCustomManager
This class defines the custom manager class used to test the mafSerialization manager interface API.
*/
class testCustomManager : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor
    testCustomManager(const mafString code_location = "");

public slots:
    /// observer needed to receive the 'mementoLoaded' signal
    void createdMemento(mafCore::mafMemento *m);
};

testCustomManager::testCustomManager(QString code_location) : mafObjectBase(code_location) {
    mafEventBusManager::instance();

    // Register the Custom Manager to be observer of the Memento Loaded signal.
    /*mafEventItemProperties mementoCallback;
    mementoCallback.m_EventId = ;
    mementoCallback.m_SignatureType = mafSignatureTypeCallback;
    mementoCallback.m_Object = this;
    mementoCallback.m_MethodSignature = "createdMemento(mafCore::mafMemento *)";
    eventBus->addEventProperty(mementoCallback);*/

    mafRegisterLocalCallback("MEMENTO_LOADED", this, "createdMemento(mafCore::mafMemento *)");
}

void testCustomManager::createdMemento(mafCore::mafMemento *m) {
    mafMsgDebug("%s", mafTr("Memento loaded!!").toAscii().data());
    QVERIFY(m != NULL);
    mafDEL(m);
}

/**
 Class name: mafSerializationManagerTest
 This class implements the test suite for mafSerializationManager.
 */

//! <title>
//mafSerializationManager
//! </title>
//! <description>
// This singletone provides the facade class for the object serialization mechanism.
//! </description>

class mafSerializationManagerTest: public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Create before the instance of the Serialization manager, which will register signals.
        m_SerializationManager = mafSerializationManager::instance();
        mafEventBusManager::instance();
        m_CustomManager = mafNEW(testCustomManager);
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_CustomManager);
        m_SerializationManager->shutdown();
        mafEventBusManager::instance()->shutdown();
    }

    /// Test library loading
    void mafSerializationLibraryLoading();
    /// mafSerializationManager allocation test case.
    void mafSerializationManagerAllocationTest();

    /// mafSerializationManager save test case.
    void mafSerializationManagerSaveTest();
    /// mafSerializationManager load test case.
    void mafSerializationManagerLoadTest();

private:
    mafSerializationManager *m_SerializationManager; ///< Test var
    mafString m_TestURL; ///< Test URL for file.
    testCustomManager *m_CustomManager; ///< Manager test var
};

void mafSerializationManagerTest::mafSerializationLibraryLoading() {
    bool module_initialized(false);
    module_initialized = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
    QVERIFY(module_initialized);
}

void mafSerializationManagerTest::mafSerializationManagerAllocationTest() {
    QVERIFY(m_SerializationManager != NULL);
}

void mafSerializationManagerTest::mafSerializationManagerSaveTest() {
    mafString pathURL = QDir::tempPath();
    pathURL.append("/maf3Logs");
    QDir log_dir(pathURL);
    if(!log_dir.exists()) {
        log_dir.mkpath(pathURL);
    }
    m_TestURL = pathURL;
    m_TestURL.append("/testSerializationManager1.maf3");

    mafString obj_type("mafCore::mafObject");
    mafString vtk = "VTK";
    mafString stl = "STL";
    mafString vrml = "VRML";
    mafString codecVTK = "myNamespace::mafCodecVTK";
    mafString codecSTL = "myNamespace::mafCodecSTL";
    mafString codecVRML = "myNamespace::mafCodecVRML";

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, obj_type));
    argList.append(mafEventArgument(mafString, vtk));
    argList.append(mafEventArgument(mafString, codecVTK));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);

    argList.clear();
    argList.append(mafEventArgument(mafString, obj_type));
    argList.append(mafEventArgument(mafString, stl));
    argList.append(mafEventArgument(mafString, codecSTL));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal,&argList);

    argList.clear();
    argList.append(mafEventArgument(mafString, obj_type));
    argList.append(mafEventArgument(mafString, vrml));
    argList.append(mafEventArgument(mafString, codecVRML));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal,&argList);

    mafObject *obj1 = mafNEW(mafCore::mafObject);
    obj1->setObjectName("Obj1");

    mafMemento *m = obj1->createMemento();

    // Try to use a unknown codec; should be used the RAW default.
    mafString encodeType = "MP3";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafMemento *, m));
    argList.append(mafEventArgument(mafString, m_TestURL));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal,&argList);

    QVERIFY(mafFile::exists(m_TestURL));
    QFileInfo fInfo1(m_TestURL);
    QVERIFY(fInfo1.size() > 0);
    mafFile::remove(m_TestURL);

    //! [1..]
    // Fake memento with custom object type to be serialized with mafCodecRaw; it should be able to serialize it.
    mafMemento *cm = mafNEW(mafCore::mafMemento);
    mafMementoPropertyList *list = cm->mementoPropertyList();
    mafMementoPropertyItem item;
    item.m_Name = "objectType";
    item.m_Multiplicity = 1;
    item.m_Value = mafVariant("myNamespace::mafCustomObject");
    list->append(item);

    m_TestURL = pathURL;
    m_TestURL.append("/testSerializationManager2.maf3");

    encodeType = "RAW";
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafMemento *, cm));
    argList.append(mafEventArgument(mafString, m_TestURL));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal,&argList);
    //! [1..]
    QVERIFY(mafFile::exists(m_TestURL));
    QFileInfo fInfo3(m_TestURL);
    QVERIFY(fInfo3.size() > 0);
    mafFile::remove(m_TestURL);

    // Try to use the default codec (mafCodecRaw).
    m_TestURL = pathURL;
    m_TestURL.append("/testSerializationManager3.maf3");
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafMemento *, m));
    argList.append(mafEventArgument(mafString, m_TestURL));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.save", mafEventTypeLocal,&argList);

    QVERIFY(mafFile::exists(m_TestURL));
    QFileInfo fInfo2(m_TestURL);
    QVERIFY(fInfo2.size() > 0);

    mafDEL(cm);
    mafDEL(obj1);
    mafDEL(m);
}

void mafSerializationManagerTest::mafSerializationManagerLoadTest() {
    mafString encodeType = "RAW";
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, m_TestURL));
    argList.append(mafEventArgument(mafString, encodeType));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.load", mafEventTypeLocal,&argList);
    //! [2..]
    mafFile::remove(m_TestURL);
}

MAF_REGISTER_TEST(mafSerializationManagerTest);
#include "mafSerializationManagerTest.moc"
