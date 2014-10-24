/*
 *  mafSceneNodeTest.h
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
#include <mafResourcesRegistration.h>
#include <mafSceneNode.h>
#include <mafPipeVisual.h>
#include <mafVME.h>

//forward delcarations
class testVisualPipeCustomForSceneNode;

/**
 Class name: mafSceneNodeTest
 This class implements the test suite for mafSceneNode.
 */
class mafSceneNodeTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// set visual pipe test case.
    void mafSceneNodeSetVisualPipeTest();

};
