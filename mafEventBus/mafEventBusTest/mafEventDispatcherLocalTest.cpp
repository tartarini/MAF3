/*
 *  mafEventDispatcherLocalTest.cpp
 *  mafEventBusTest
 *
 *  Created by Daniele Giunchi on 20/04/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafEventDispatcherLocal.h>
#include <mafEvent.h>

using namespace mafEventBus;

//-------------------------------------------------------------------------
/**
 Class name: mafObjectCustom
 Custom object needed for testing.
 */
class testObjectCustomForDispatcherLocal : public QObject {
    Q_OBJECT

public:
    /// constructor.
    testObjectCustomForDispatcherLocal();

    /// Return the var's value.
    int var() {return m_Var;}

public slots:
    /// Test slot that will increment the value of m_Var when an UPDATE_OBJECT event is raised.
    void updateObject();
    void setObjectValue(int v);

signals:
    void valueModified(int v);
    void objectModified();

private:
    int m_Var; ///< Test var.
};

testObjectCustomForDispatcherLocal::testObjectCustomForDispatcherLocal() : m_Var(0) {
}

void testObjectCustomForDispatcherLocal::updateObject() {
    m_Var++;
}

void testObjectCustomForDispatcherLocal::setObjectValue(int v) {
    m_Var = v;
}

//-------------------------------------------------------------------------


/**
 Class name: mafEventDispatcherLocalTest
 This class implements the test suite for mafEventDispatcherLocal.
 */

//! <title>
//mafEventDispatcherLocal
//! </title>
//! <description>
//mafEventDispatcherLocal allows dispatching events coming from local
//application to attached observers.
//! </description>

class mafEventDispatcherLocalTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        m_ObjTestObserver = new testObjectCustomForDispatcherLocal;
        m_EventDispatcherLocal =new mafEventBus::mafEventDispatcherLocal;

        mafString updateID = "maf.local.eventBus.globalUpdate";

        int i=0;
        for(; i < 100; i++) {
            testObjectCustomForDispatcherLocal * obj = new testObjectCustomForDispatcherLocal;
            m_ObjectList.append(obj);

            mafEvent *propCallback = new mafEventBus::mafEvent;
            (*propCallback)[TOPIC] = updateID;
            (*propCallback)[TYPE] = mafEventTypeLocal;
            (*propCallback)[SIGTYPE] = mafSignatureTypeCallback;
            QVariant var;
            var.setValue((QObject*)obj);
            (*propCallback)[OBJECT] = var;
            (*propCallback)[SIGNATURE] = "updateObject()";
            m_EventDispatcherLocal->addObserver(*propCallback);
            m_CallBackList.push_back(propCallback);
        }
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        int i=0, size = m_ObjectList.count();
        for(; i< size; i++) {
            if(m_ObjectList[i]) {
                delete m_ObjectList[i];
            }
        }
        delete m_ObjTestObserver;
        delete m_EventDispatcherLocal;
    }

    /// mafEventDispatcherLocal allocation test case.
    void mafEventDispatcherLocalAllocationTest();

private:
    testObjectCustomForDispatcherLocal *m_ObjTestObserver; ///< Test Object var
    mafList<testObjectCustomForDispatcherLocal *> m_ObjectList; ///< object list for benchmark
    mafEventDispatcherLocal *m_EventDispatcherLocal; ///< Test var.
    mafEventItemListType m_CallBackList; ///< callback list fr benchmark
};

void mafEventDispatcherLocalTest::mafEventDispatcherLocalAllocationTest() {
    QVERIFY(m_EventDispatcherLocal != NULL);
}

MAF_REGISTER_TEST(mafEventDispatcherLocalTest);
#include "mafEventDispatcherLocalTest.moc"


