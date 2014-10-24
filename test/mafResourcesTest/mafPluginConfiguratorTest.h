/*
 *  mafPluginConfiguratorTest.h
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafResourcesRegistration.h>
#include <mafPluginConfigurator.h>

/**
 Class name: mafPluginConfiguratorTest
 This class implements the test suite for mafPluginConfigurator.
 */

 //! <title>
//mafPluginConfigurator
//! </title>
//! <description>
//mafPluginConfigurator defines the parser of XML configuration file to plug objects into the framework and customize the plug in an easy way.
//! </description>

class mafPluginConfiguratorTest : public QObject {
    Q_OBJECT

    /// Prepare the test data to be used into the test suite.
    void initializeTestData();

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// Test the parsing method.
    void mafPluginConfiguratorParseTest();

private:
   QString m_ConfigFile; ///< Test var.
};
