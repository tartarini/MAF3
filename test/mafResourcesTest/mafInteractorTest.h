/*
 *  mafInteractorTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafInteractor.h>
#include <mafProxyInterface.h>

/**
 Class name: mafInteractorTest
 This class implements the test suite for mafInteractor.
 */
class mafInteractorTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafInteractor allocation test case.
    void mafInteractorAllocationTest();

    /// mafInteractor second constructor allocation test case.
    void mafInteractorStyleConstructorAllocationTest();

    /// style assignment test suite.
    void styleAssignmentTest();

private:
    mafResources::mafInteractor *m_Interactor; ///< Test var.
};
