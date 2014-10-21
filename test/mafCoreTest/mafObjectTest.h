/*
 *  mafObjectTest.h
 *  mafCoreTest
 *
 *  Created by Daniele Giunchi on 08/07/14.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafObject.h>
#include <mafMementoObject.h>
#include <mafVisitor.h>

//! <title>
//mafObject
//! </title>
//! <description>
//mafObject defines the basic API for objects that wants to be serialized and visited by the mafVisitor class, 
//provides undo mechanism and defines API to control the serialization of tags.
//If you want to create an object with undo state mechanism and tags serialization, it has to inherit from mafObject.
//
//It defines some virtual methods:
//"virtual mafMementoObject *createMemento()"  and "virtual void setMemento(mafMementoObject *memento)"  to implement 
//a sort of undo mechanism for the object's state creating and setting the status of the object mafMementoObject.
//"virtual bool isObjectValid() const" used in MAF3 design by contract to check object's validity. isObjectValid methods play
//an important role in checking the consistency of objects in the debug. isObjectValid is defined as a pure virtual function
//in Object class, thus it needs to be overridden in all inheriting classes.The inheriting class should perform defensive
//checks to make sure that it is in a consistent state. Also note that this method is only available in the debug build.
//"virtual void acceptVisitor(mafVisitor *v)" to allows to accept a mafVisitor which will visit the object and will be 
//executed the mafVisitor algorithm.
//It defines API to add, remove and filter tags: tags wants to define a series of strings that describe the object and
//categorize it in the cloud of MAF3 objects (like a photo tags on Flickr).
//! </description>

/**
 Class name: mafObjectTest
 This class implements the test suite for mafObject.
 */
class mafObjectTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase();

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase();

    /// register new object in factory test case.
    void mafObjectConstructorTest();
    /// object validity test case.
    void isObjectValidTest();
    /// memento creation test case.
    void createMementoTest();
    /// memento assignment test case.
    void setMementoTest();
    /// accept visitor test case.
    void acceptVisitorTest();
    /// Test the Object's addTag functionality.
    void addTagTest();
    /// Test the Object's removeTag functionality.
    void removeTagTest();
    /// Test the Object's filterTag functionality.
    void filterTagTest();
    /// Test the Object's addScript functionality.
    void addScriptTest();
    /// Test the Object's removeScript functionality.
    void removeScriptTest();
    /// Test isEqual method.
    void isEqualTest();    
    /// Test setDictionary method.
    void setDictionaryTest();
    /// Test removeDictionary method.
    void removeDictionaryTest();

private:
    mafObject *m_ObjTestVar; ///< Test variable
    mafMemento *m_Memento; ///< Memento test var
};
