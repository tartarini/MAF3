/*
 *  mafGUIManagerTest.h
 *  mafGUITest
 *
 *  Created by Daniele Giunchi on 10/07/14.
 *  Copyright 2010 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafGUIRegistration.h>
#include <mafGUIManager.h>
#include <QMainWindow>
#include <QMenu>


/**
 Class name: mafGUIManagerTest
 This class implements the test suite for mafGUIManager.
 */
class mafGUIManagerTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup test variables memory allocation.
    void cleanupTestCase();

    /// allocation test case.
    void mafGUIManagerAllocationTest();
    /// Creation menu test.
    void mafGUIManagerCreateMenuTest();
    /// Fill menu test.
    void mafGUIManagerFillMenuTest();
    /// Max recent file number test
    void maxRecentFileTest();

private:
    mafGUI::mafGUIManager *m_GUIManager; ///< Reference to the GUI Manager.
    QMainWindow *m_MainWin;
};
