/*
 *  mafMementoVMETest.cpp
 *  mafResourcesTest
 *
 *  Created by Roberto Mucci on 24/05/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafDataSet.h>
#include "mafDataPipe.h"
#include <mafContainer.h>
#include <mafExternalDataCodec.h>
#include <mafContainer.h>
#include <mafContainerInterface.h>


using namespace mafCore;
using namespace mafResources;

//! <title>
//mafMementoVME
//! </title>
//! <description>
//mafMementoVME aims to store a mafVME state implementing a sort
//of undo mechanism for the object's state. This is used to restore
// a previous stored VME state (undo mechanism or serialization porpouses).
//! </description>

//------------------------------------------------------------------------------------------

/**
 Class name: testExtDataType
 This class implements the external data type coming from an external library.
 */
class testExtDataType {
public:
    /// Object constructor.
    testExtDataType(mafString v) : m_Value(v) {}

    /// set the new value for the class.
    void setValue(mafString v) {m_Value = v;}
    /// Return the inner value.
    mafString value() {return m_Value;}

private:
    mafString m_Value; ///< Test variable for external data
};



/**
 Class name: testExtDataCodecCustom
 This class implements an external data codec.
 */
class  testExtDataCodecCustom : public  mafCore::mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    testExtDataCodecCustom(const mafString code_location = "");



    /// Encode the memento into the output type.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode the output type into the memento.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

private:
    mafContainer<mafString> *m_Cont; ///< Test Var.
};

testExtDataCodecCustom::testExtDataCodecCustom(const mafString code_location) : mafExternalDataCodec(code_location) {
}

void testExtDataCodecCustom::decode(const char *input_string, bool binary) {
    REQUIRE(input_string != NULL);
    m_Cont = new mafContainer<mafString>();
    *m_Cont = new mafString;
    m_Cont->externalData()->append(input_string);
    this->m_ExternalData = m_Cont;
}

char *testExtDataCodecCustom::encode(bool binary) {
    mafContainer<mafString> *dataSet = mafContainerPointerTypeCast(mafString, this->externalData());
    mafString dataString = dataSet->externalData()->toAscii();
    char *output_string = new char[dataString.size()+1];
    memcpy(output_string,dataString.toAscii().data(),dataString.size()+1);
    return output_string;
}

/**
 Class name: testDataPipe
 This class implements the data pipe to be tested.
 */
class  testDataPipe : public  mafDataPipe {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafResources::mafDataPipe);

public:
    /// Object constructor.
    testDataPipe(const mafString code_location = "");

    /// Initialize and create the pipeline
    /*virtual*/ void createPipe();

    /// Return the string variable initializated and updated from the data pipe.
    mafString pipeline() {return m_PipeLine;}

public slots:
    /// Allow to execute and update the pipeline when something change
    /*virtual*/ void updatePipe(double t = -1);

private:
    mafString m_PipeLine; ///< Test Var.
};

testDataPipe::testDataPipe(const mafString code_location) : mafDataPipe(code_location), m_PipeLine("") {
}

void testDataPipe::createPipe() {
    m_PipeLine = "Created";
}

void testDataPipe::updatePipe(double t) {
    m_PipeLine = "Updated";
    m_PipeLine.append(mafString::number(t));
    if(m_DecoratorPipe) {
        m_DecoratorPipe->updatePipe(t);
    }
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafMementoVMETest
 This class implements the test suite for mafMementoVME.
 */
class mafMementoVMETest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObject(testExtDataCodecCustom);
        m_MementoVME = NULL;
        m_VME = mafNEW(mafResources::mafVME);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
    }

    /// mafMementoVME allocation test case.
    void mafMementoVMEDefaultAllocationTest();
    /// mafMementoVME allocation test case.
    void mafMementoVMECustomAllocationTest();

private:
    mafVME *m_VME; ///< Test var.
    mafDataSet *m_DataSet; 
    mafMementoVME *m_MementoVME; ///< Test var.
};

void mafMementoVMETest::mafMementoVMEDefaultAllocationTest() {
    QVERIFY(m_VME != NULL);
    mafMemento *m = (mafMemento *)mafNEWFromString("mafResources::mafMementoVME");
    QVERIFY(m != NULL);
    m_MementoVME = dynamic_cast<mafMementoVME*>(m);
    QVERIFY(m_MementoVME != NULL);
    mafDEL(m_MementoVME);
}

void mafMementoVMETest::mafMementoVMECustomAllocationTest() {

    mafString testString("testString");
    mafString testString2("testString2");

    //Create first dataSet
    mafContainer<testExtDataType> container;
    container = new testExtDataType(testString);
    container.setExternalDataType("testExtDataType");
    container.setExternalCodecType("testExtDataCodecCustom");
    mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
    dataSet->setDataValue(&container);

    mafPoseMatrix *matrix = new mafPoseMatrix();
    matrix->set_identity();
    matrix->put(0,0,3);
    dataSet->setPoseMatrix(matrix);

    //Create second dataSet
    mafContainer<testExtDataType> container2;
    container2 = new testExtDataType(testString2);
    container2.setExternalDataType("testExtDataType");
    container2.setExternalCodecType("testExtDataCodecCustom");
    mafDataSet *dataSet2 = mafNEW(mafResources::mafDataSet);
    dataSet2->setDataValue(&container2);

    mafPoseMatrix *matrix2 = new mafPoseMatrix();
    matrix2->set_identity();
    matrix2->put(1,0,5);
    dataSet2->setPoseMatrix(matrix2);
    
    mafDataSetCollection *collection = m_VME->dataSetCollection();
    collection->insertItem(dataSet, 0);
    collection->insertItem(dataSet2, 1);

    testDataPipe *dataPipe = mafNEW(testDataPipe);
    m_VME->setDataPipe(dataPipe);

    //! <snippet>
    ////Create the VME Memento that stores dataSetCollection and dataPipe
    ////of the VME.
    mafMemento *m = m_VME->mafResource::createMemento();
    m_MementoVME = new mafMementoVME(m_VME, m_VME->dataSetCollection(), m_VME->dataPipe(), mafCodeLocation);
    //! </snippet>
    QVERIFY(m_MementoVME != NULL);
    m->setParent(m_MementoVME);

    //! <snippet>
    ////Restore the VME trought Memento
    mafVME *returnVME = mafNEW(mafResources::mafVME);
    returnVME->setMemento(m_MementoVME);
    returnVME->updateData();
    //! </snippet>
    QVERIFY(dataPipe->isEqual(returnVME->dataPipe()));

    mafDataSet *returnDataSet;
    returnDataSet = returnVME->dataSetCollection()->itemAt(0);
    mafContainer<mafString> *string = mafContainerPointerTypeCast(mafString, returnDataSet->dataValue());
    mafString out;
    out = string->externalData()->toAscii();
    QCOMPARE(out, testString);

    mafDataSet *returnDataSet2;
    returnDataSet2 = returnVME->dataSetCollection()->itemAt(1);
    mafContainer<mafString> *string2 = mafContainerPointerTypeCast(mafString, returnDataSet2->dataValue());
    mafString out2;
    out2 = string2->externalData()->toAscii();
    QCOMPARE(out2, testString2);

    mafDEL(returnVME);
    mafDEL(m_MementoVME);
}

MAF_REGISTER_TEST(mafMementoVMETest);
#include "mafMementoVMETest.moc"
