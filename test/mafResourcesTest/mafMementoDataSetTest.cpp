/*
 *  mafMementoDataSetTest.cpp
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
#include <mafContainer.h>
#include <mafExternalDataCodec.h>
#include <mafContainer.h>
#include <mafContainerInterface.h>


using namespace mafCore;
using namespace mafResources;

//! <title>
//mafMementoDataSet
//! </title>
//! <description>
//mafMementoDataSet aims to store a mafDataSet state implementing a sort
//of undo mechanism for the object's state. This is used to restore
// a previous stored DataSet state (undo mechanism or serialization porpouses).
//! </description>

//------------------------------------------------------------------------------------------

/**
 Class name: testExternalDataType
 This class implements the external data type coming from an external library.
 */
class testExternalDataType {
public:
    /// Object constructor.
    testExternalDataType(QString v) : m_Value(v) {}

    /// set the new value for the class.
    void setValue(QString v) {m_Value = v;}
    /// Return the inner value.
    QString value() {return m_Value;}

private:
    QString m_Value; ///< Test variable for external data
};


/**
 Class name: testExternalDataCodecCustom
 This class implements the external data codec to be tested.
 */
class  testExternalDataCodecCustom : public  mafCore::mafExternalDataCodec {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafExternalDataCodec);

public:
    /// Object constructor.
    testExternalDataCodecCustom(const QString code_location = "");

    /// Encode the memento into the output type.
    /*virtual*/ char *encode(bool binary = true);

    /// Decode the output type into the memento.
    /*virtual*/ void decode(const char *input_string, bool binary = true);

private:
    mafContainer<QString> *m_Cont; ///< Test Var.
};

testExternalDataCodecCustom::testExternalDataCodecCustom(const QString code_location) : mafExternalDataCodec(code_location) {
}

void testExternalDataCodecCustom::decode(const char *input_string, bool binary) {
    Q_UNUSED(binary);
    REQUIRE(input_string != NULL);
    m_Cont = new mafContainer<QString>();
    *m_Cont = new QString;
    m_Cont->externalData()->append(input_string);
    this->m_ExternalData = m_Cont;
}

char *testExternalDataCodecCustom::encode(bool binary) {
    Q_UNUSED(binary);
    mafContainer<QString> *dataSet = mafContainerPointerTypeCast(QString, this->externalData());
    QString dataString = dataSet->externalData()->toAscii();
    char *output_string = new char[dataString.size()+1];
    memcpy(output_string,dataString.toAscii().data(),dataString.size()+1);
    return output_string;

}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------

/**
 Class name: mafMementoDataSetTest
 This class implements the test suite for mafMementoDataSet.
 */
class mafMementoDataSetTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObject(testExternalDataCodecCustom);
        m_MementoDataSet = NULL;
        m_DataSet = mafNEW(mafResources::mafDataSet);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_MementoDataSet);
        mafDEL(m_DataSet);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafMementoDataSet allocation test case.
    void mafMementoDataSetDefaultAllocationTest();
    /// mafMementoDataSet allocation test case.
    void mafMementoDataSetCustomAllocationTest();

private:
    mafDataSet *m_DataSet; ///< Test var.
    mafMementoDataSet *m_MementoDataSet; ///< Test var.
};

void mafMementoDataSetTest::mafMementoDataSetDefaultAllocationTest() {
    QVERIFY(m_DataSet != NULL);
    mafMemento *m = (mafMemento *)mafNEWFromString("mafResources::mafMementoDataSet");
    QVERIFY(m != NULL);
    m_MementoDataSet = qobject_cast<mafMementoDataSet*>(m);
    QVERIFY(m_MementoDataSet != NULL);
}

void mafMementoDataSetTest::mafMementoDataSetCustomAllocationTest() {
    mafDEL(m_MementoDataSet);
    QString testString("testStringa");

    mafContainer<testExternalDataType> container;
    container = new testExternalDataType(testString);
    container.setExternalDataType("testExternalDataType");
    container.setExternalCodecType("testExternalDataCodecCustom");
    m_DataSet->setDataValue(&container);

    mafPoseMatrix *matrix = new mafPoseMatrix();
    matrix->set_identity();
    matrix->put(0,0,3);
    m_DataSet->setPoseMatrix(matrix);

    //! <snippet>
    ////Create the DataSet memento that stores poseMatrix and dataValue
    ////of the DataSet.
    m_MementoDataSet = new mafMementoDataSet(m_DataSet, mafCodeLocation);
    //! </snippet>
    QVERIFY(m_MementoDataSet != NULL);

    //After setMemento, return value must be equal to the orginal value
    //! <snippet>
    ////Restore the DataSet throught the Memento
    mafDataSet *returnDataSet = mafNEW(mafResources::mafDataSet);
    returnDataSet->setMemento(m_MementoDataSet);
    //! </snippet>
    mafContainer<QString> *string = mafContainerPointerTypeCast(QString, returnDataSet->dataValue());
    QString out;
    out = string->externalData()->toAscii();
    QCOMPARE(out, testString);
    mafDEL(returnDataSet);
}

//MAF_REGISTER_TEST(mafMementoDataSetTest);
#include "mafMementoDataSetTest.moc"
