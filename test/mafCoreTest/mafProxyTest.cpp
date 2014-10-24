/*
 *  mafProxyTest.cpp
 *  mafCoreTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 27/03/09.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafCoreTestList.h"

using namespace mafCore;

/**
 Class name: testCustomDataType
 This class is a custom data type to be wrapped from mafProxy.
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

void mafProxyTest::initTestCase() {
}

void mafProxyTest::cleanupTestCase() {
    delete t;
    delete tw;
}

void mafProxyTest::mafProxyAllocationTest() {
    // Create the istance of external data object.
    t = new testCustomDataType;

    // Initialize also the second object.
    // and modify its default value.
    tw = new testCustomDataType;
    tw->addValue(58);

    // Assign the external data type to the MAF wrapper.
    m_Wrapper = tw;
}

void mafProxyTest::mafProxyInternalDataTest() {
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

    mafProxyInterface *dwi = &m_Wrapper;
    QString dt2 = dwi->externalDataType();
    QString dt = m_Wrapper.externalDataType();

    QCOMPARE(dt, dt2);
}

void mafProxyTest::mafProxyToVariant() {
    // Get the initial values befor assigning it to the QVariant type.
    mafProxyInterface *dwi = &m_Wrapper;
    testCustomDataType *data_source = m_Wrapper.externalData();
    int val_source = data_source->value();

    // Create the QVariant and assign to it the mafProxy
    QVariant var;
    var.setValue(dwi);
    // Check that it can be converted to the registered type.
    bool ok = var.canConvert<mafProxyInterfacePointer>();
    QVERIFY(ok);

    // Extract the value from the QVariant and make some checks to ensure that it is equal
    // to the previous one assigned.
    mafProxyInterface *dwi_from_variant = var.value<mafProxyInterfacePointer>();
    QString edt = dwi_from_variant->externalDataType();
    QCOMPARE(edt, dwi->externalDataType());

    // Check the internal data owned by the mafProxy
    mafProxy<testCustomDataType> *cdt = mafProxyPointerTypeCast(testCustomDataType, dwi_from_variant);
    QVERIFY(cdt != NULL);
    testCustomDataType *data = cdt->externalData();
    QVERIFY(data != NULL);

    int val = data->value();
    QVERIFY(val_source == val);
}

#include "mafProxyTest.moc"
