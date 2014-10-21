/*
 *  mafHierarchyTest.cpp
 *  mafResourcesTest
 *
 *  Created by Daniele Giunchi on 14/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafHierarchy.h>
#include <mafSceneNode.h>
#include <mafVME.h>
#include <mafPipeVisual.h>
#include <mafVisitorFindSceneNodeByVMEHash.h>
#include <iostream>

//forward classes
class testVisualPipeCustom;

/**
 Class name: mafHierarchyTest
 This class implements the test suite for mafHierarchy.
 */

//! <title>
//mafHierarchy
//! </title>
//! <description>
//mafHierarchy provides basic API manage a tree of generic mafObjectBase.
//A scenegraph contains a tree of scenenodes which are defined as structs
//that contains a VME and a VisualPipe. It is also defined a tree iterator,
//that represents the current position inside the tree.
//! </description>
class mafHierarchyTest: public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// mafSceneGraph allocation test case.
    void mafSceneGraphAllocationTest();
    
    /// check if has parent retrieve true or false depending if the selected node is root.
    void mafScenegraphRootTest();
    
    /// benchmarking test, search in a vme added in the middle of tree generation
    void mafSceneGraphBenchmarkMiddleTest();

    /// benchmarking test, search in a vme added in the end of tree generation
    void mafSceneGraphBenchmarkEndTest();

    /// mafSceneGraph manage test case.
    void mafSceneGraphManageTest();


private:
    mafCore::mafHierarchy *m_HierarchyToManage; ///< Test var.
    mafCore::mafHierarchy *m_Hierarchy; ///< Test var.
    QString m_CustomHashInTheMiddle; ///< hash to find that is located in the middle of the generation of the tree
    QString m_CustomHashInTheEnd; ///< hash to find that is located in the end of the generation of the tree
    QList<mafResources::mafVME *> m_VMEList; ///< VME list which must be used to delete internal objects

    /// function for quick random creation of an unbalanced tree
    void unbalancedTreeRandomCreation(unsigned int numberOfElements);
    /// function for the destruction of the tree
    void unbalancedTreeDestruction();
};
