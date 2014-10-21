/*
 *  mafOperationManagerTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafCoreRegistration.h>
#include <mafResourcesSingletons.h>
#include <mafEventBusManager.h>
#include <mafVMEManager.h>
#include <mafOperationManager.h>
#include <mafOperation.h>
#include <mafOperationWorker.h>
#include <mafVME.h>
//forward declarations
class testEndlessOperation;
class testNotUndoOperation;
class testUndoOperation;
class testAutoCancelSingleThreadOperation;
class testAutoCancelMultiThreadOperation;



/**
 Class name: mafOperationManagerTest
 This class implements the test suite for mafOperationManager.
 */
class mafOperationManagerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafOperationManager allocation test case.
    void mafOperationManagerAllocationTest();

    /// Start and cancel test case.
    void cancelStartTest();

    /// Abort execution test
    void abortExecutionTest();

    /// Undo-Redo functionality test
    void undoRedoExecutionTest();
    
    /// Operation which stop its execution, canceling it.
    void cancelExecutionTest();

public:
    /// Utility method to start operations. Return the operation started.
    mafCore::mafObjectBase *startOperation(QString opType);

    /// Utility method to retrieve the execution pool.
    const mafExecutionPool *retrievePool();

public Q_SLOTS:
    void vmeAddTest(mafCore::mafObjectBase *vme);

Q_SIGNALS:
    void vmeAddSignalTest(mafCore::mafObjectBase *vme);

private:
    mafEventBus::mafEventBusManager *m_EventBus; ///< Reference to the event bus.
    mafResources::mafOperationManager *m_OperationManager; ///< Reference to the OperationManager.
    const mafExecutionPool *m_ExecutionPool; ///< Pool of running thread.
    mafResources::mafVMEManager *m_VMEManager; ///< Manager needed for the hierarchy.
};
