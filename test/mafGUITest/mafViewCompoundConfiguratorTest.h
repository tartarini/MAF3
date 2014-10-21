/*
 *  mafViewCompoundConfiguratorTest.h
 *  mafGUITest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestConfig.h>
#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafGUIRegistration.h>
#include <mafViewCompoundConfigurator.h>

#include <QPushButton>

//forward declarations
class testViewForCompound;
class testVisualPipeForCompound;

/**
 Class name: mafViewCompoundConfiguratorTest
 This class implements the test suite for mafViewCompoundConfigurator.
 */
class mafViewCompoundConfiguratorTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// allocation test case.
    void mafViewCompoundConfiguratorAllocationTest();

    /// allocation test case.
    void mafViewCompoundConfiguratorParseTest();

private:
    mafGUI::mafViewCompoundConfigurator *m_Configurator; ///< Reference to the compound view configurator.
	QString m_ConfigFilename; ///< Filename for the configurator.
};
