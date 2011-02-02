/*
 *  mafContainerTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani on 27/03/09.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafContainer.h>


using namespace mafCore;

/**
 Class name: testCustomDataType
 This class is a custom data type to be wrapped from mafContainer.
 */

class testCustomDataType {
public:
    /// Object constructor
    testCustomDataType();
    /// Object destructor
    ~testCustomDataType();

    /// Update internal var.
    void setValue(testCustomDataType *v) {m_Value = v->value();}

    /// Add a value
    void addValue(int v) {m_Value += v;}

    /// Another custom method
    void setCustomValue() {m_Value = 2 * 3;}

    /// Return the value of internal variable.
    int value() {return m_Value;}

private:
    int m_Value; ///< Test variable
};

testCustomDataType::testCustomDataType() : m_Value(2) {
}

testCustomDataType::~testCustomDataType() {
    m_Value = 0;
}

/**
 Class name: mafContainerTest
 This class implements the test suite for mafContainer.
 */

//! <title>
//mafContainer
//! </title>
//! <description>
//mafContainer This class defines the base concrete class container for the MAF3 dataset values.
//! </description>

class mafContainerTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
    }

    /// Cleanup tes variables memory allocation.
    void cleanupTestCase() {
        delete t;
        delete tw;
    }

    /// create new object and check that is not NULL test case.
    void mafContainerAllocationTest();
    /// Check internal data type existance.
    void mafContainerInternalDataTest();
    /// Check mafContainerInterface 'wrapped' into mafVariant.
    void mafContainerToVariant();

private:
    //! <snippet>
    mafContainer<testCustomDataType> m_Wrapper; ///< Test var.
    //! </snippet>
    testCustomDataType *t;  ///< First object of external to MAF type.
    testCustomDataType *tw; ///< Second object of external to MAF type.
};

void mafContainerTest::mafContainerAllocationTest() {
    // Create the istance of external data object.
    t = new testCustomDataType;

    // Initialize also the second object.
    // and modify its default value.
    tw = new testCustomDataType;
    tw->addValue(58);

    // Assign the external data type to the MAF wrapper.
    m_Wrapper = tw;
}

void mafContainerTest::mafContainerInternalDataTest() {
    // Assign a custom value to the first object to modify its default value.
    t->setCustomValue();

    // Overwrite its value with that one wrapped into the MAF wrapper.
    t->setValue(m_Wrapper);

    // At this point values of both external data objects should be the same.
    int v = t->value();
    int vw = tw->value();
    QVERIFY(v == vw);

    // Use the wrapped data object API from MAF wrapper.
    m_Wrapper->addValue(10);
    vw = tw->value();
    // vw should be 60 + 10 => 70
    QVERIFY(vw == 70);

    mafContainerInterface *dwi = &m_Wrapper;
    mafString dt2 = dwi->externalDataType();
    mafString dt = m_Wrapper.externalDataType();

    QCOMPARE(dt, dt2);
}

void mafContainerTest::mafContainerToVariant() {
    // Get the initial values befor assigning it to the mafVariant type.
    mafContainerInterface *dwi = &m_Wrapper;
    testCustomDataType *data_source = m_Wrapper.externalData();
    int val_source = data_source->value();

    // Create the mafVariant and assign to it the mafContainer
    mafVariant var;
    var.setValue(dwi);
    // Check that it can be converted to the registered type.
    bool ok = var.canConvert<mafContainerInterfacePointer>();
    QVERIFY(ok);

    // Extract the value from the mafVariant and make some checks to ensure that it is equal
    // to the previous one assigned.
    mafContainerInterface *dwi_from_variant = var.value<mafContainerInterfacePointer>();
    mafString edt = dwi_from_variant->externalDataType();
    QCOMPARE(edt, dwi->externalDataType());

    // Check the internal data owned by the mafContainer
    mafContainer<testCustomDataType> *cdt = mafContainerPointerTypeCast(testCustomDataType, dwi_from_variant);
    QVERIFY(cdt != NULL);
    testCustomDataType *data = cdt->externalData();
    QVERIFY(data != NULL);

    int val = data->value();
    QVERIFY(val_source == val);
}

MAF_REGISTER_TEST(mafContainerTest);
#include "mafContainerTest.moc"
