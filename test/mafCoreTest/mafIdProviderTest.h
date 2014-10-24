/*
 *  mafIdProviderTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>

/**
 Class name: mafIdProviderTest
 This class implements the test suite for mafIdProvider.
 */

//! <title>
//mafIdProvider
//! </title>
//! <description>
//mafIdProvider is a singleton which provides the generation of unique ID used for objects or events.
//
//It defines methods to assign a unique ID to objects and events:
//"mafId newObjectId(const QString id_name = "")" return next valid Id to assign to an object or to use for an event.
//By default, if no name is given to the ID, a OBJECT_ID name is assigned.
//
//The association between IDs and corresponding name is stored in a mafIdHashType (typedef QHash<mafId, QString> mafIdHashType)
//! </description>

class mafIdProviderTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();
    
    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// Test the ID generation
    void createNewIdTest();
    /// Test the change of ID name
    void setIdNameTest();
    /// idName test case
    void idNameTest();
    /// idValue test case
    void idValueTest();
    /// idRemove test case
    void idRemoveTest();

private:
    mafId m_IdValue; ///< Id created
    mafIdProvider *m_IdProvider; ///< Test variable
};
