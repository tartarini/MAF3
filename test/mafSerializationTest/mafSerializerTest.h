/*
 *  mafSerializerTest.cpp
 *  mafSerializationTest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafSerializer.h>

//forward declaration
class  testSerializerCustom;

/**
 Class name: mafSerializerTest
 This class implements the test suite for mafSerializer.
 */

 //! <title>
//mafSerializer
//! </title>
//! <description>
//mafSerializer is the base class that define the interface for MAF3 serializer.
//! </description>

class mafSerializerTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafSerializer allocation test case.
    void mafSerializerAllocationTest();
    /// test the openDevice/closeDevice method.
    void openCloseDeviceTest();
    /// test the data URL assignment.
    void dataURLTest();

private:
    testSerializerCustom *m_Serializer; ///< Test var
};
