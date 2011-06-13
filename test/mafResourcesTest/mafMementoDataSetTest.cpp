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
#include <mafProxy.h>
#include <mafExternalDataCodec.h>
#include <mafProxy.h>
#include <mafProxyInterface.h>

#ifdef WIN32
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.dll"
#else
#ifdef __APPLE__
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.dylib"
#else
#define SERIALIZATION_LIBRARY_NAME "mafSerialization.so"
#endif
#endif


using namespace mafCore;
using namespace mafResources;
using namespace mafEventBus;

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
    mafProxy<QString> *m_Cont; ///< Test Var.
};

testExternalDataCodecCustom::testExternalDataCodecCustom(const QString code_location) : mafExternalDataCodec(code_location) {
}

void testExternalDataCodecCustom::decode(const char *input_string, bool binary) {
    Q_UNUSED(binary);
    REQUIRE(input_string != NULL);
    m_Cont = new mafProxy<QString>();
    *m_Cont = new QString;
    m_Cont->externalData()->append(input_string);
    this->m_ExternalData = m_Cont;
}

char *testExternalDataCodecCustom::encode(bool binary) {
    Q_UNUSED(binary);
    mafProxy<QString> *dataSet = mafProxyPointerTypeCast(QString, this->externalData());
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
      // Create before the instance of the Serialization manager, which will register signals.
      bool res(false);
      res = mafInitializeModule(SERIALIZATION_LIBRARY_NAME);
      QVERIFY(res);

      mafMessageHandler::instance()->installMessageHandler();
      mafResourcesRegistration::registerResourcesObjects();
      mafRegisterObject(testExternalDataCodecCustom);
      m_DataSet = mafNEW(mafResources::mafDataSet);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_DataSet);
        mafMessageHandler::instance()->shutdown();
    }

    /// mafMementoDataSet allocation test case.
    void mafMementoDataSetDefaultAllocationTest();
    /// mafMementoDataSet allocation test case.
    void mafMementoDataSetCustomAllocationTest();

private:
    mafDataSet *m_DataSet; ///< Test var.
};

void mafMementoDataSetTest::mafMementoDataSetDefaultAllocationTest() {
    QVERIFY(m_DataSet != NULL);
}

void mafMementoDataSetTest::mafMementoDataSetCustomAllocationTest() {
    QString testString("testStringa");

    mafProxy<testExternalDataType> container;
    container = new testExternalDataType(testString);
    container.setExternalDataType("testExternalDataType");
    container.setExternalCodecType("CUSTOM");
    m_DataSet->setDataValue(&container);

    mafMatrix *matrix = new mafMatrix();
    matrix->setIdentity();
    matrix->setElement(0,0,3);

    m_DataSet->setPoseMatrix(matrix);

    //Plug the codec
    QString obj_type = "testExternalDataType";
    QString encodeType = "CUSTOM";
    QString codec = "testExternalDataCodecCustom";

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, obj_type));
    argList.append(mafEventArgument(QString, encodeType));
    argList.append(mafEventArgument(QString, codec));
    mafEventBusManager::instance()->notifyEvent("maf.local.serialization.plugCodec", mafEventTypeLocal, &argList);

    //! <snippet>
    ////Create the DataSet memento that stores poseMatrix and dataValue
    ////of the DataSet.
    mafMemento *memento = m_DataSet->createMemento();
    //! </snippet>
    QVERIFY(memento != NULL);

    mafDataSet *returnDataSet = mafNEW(mafResources::mafDataSet);
    returnDataSet->setMemento(memento);

    //Check if m_DataSet memento is equal to returnDataSet memento.
    mafMemento *returnMemento = returnDataSet->createMemento();
    QVERIFY(memento->isEqual(returnDataSet));

    delete matrix;
    mafDEL(returnDataSet);
    mafDEL(returnMemento);
    mafDEL(memento);
}

MAF_REGISTER_TEST(mafMementoDataSetTest);
#include "mafMementoDataSetTest.moc"
