/*
 *  mafViewCompoundConfiguratorTest.cpp
 *  mafGUITest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 30/11/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUITestList.h"

using namespace mafCore;
using namespace mafGUI;

//----------------------------------------------------------------------------------------------
class testViewForCompound : public mafObjectBase {
    Q_OBJECT
    Q_PROPERTY(QVariantHash visualPipeHash READ visualPipeHash WRITE setVisualPipeHash)
    Q_PROPERTY(QVariant renderWidget READ renderWidget WRITE setRenderingWidget STORED false)
    Q_PROPERTY(bool parameterFlag READ parameterFlag WRITE setParameterFlag)
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    testViewForCompound(const QString code_location = "");

    /// Set rendering window used by the view.
    void setRenderingWidget(QVariant renWidget);

    /// Get the rendering widget used by the view to render its scene.
    QVariant renderWidget() const;

    /// Return the hash that contains the association between data type and related visual pipe to use for render it.
    QVariantHash visualPipeHash() const;

    /// Assign the hash that will contains the association between data type and related visual pipe to use for render it.
    void setVisualPipeHash(const QVariantHash hash);

    /// Return the parameter flag.
    bool parameterFlag() const;

    /// Allows to assign the parameter flag.
    void setParameterFlag(bool parameter = true);

private:
    QVariantHash *m_VisualPipeHash; ///< Bind between dataType and Visual pipe.
    QObject *m_RenderWidget; ///< Rendering widget for the view.
    bool m_ParameterFlag; ///< Test parameter.
};

testViewForCompound::testViewForCompound(const QString code_location) : mafObjectBase(code_location), m_VisualPipeHash(NULL), m_RenderWidget(NULL), m_ParameterFlag(false) {
    m_RenderWidget = new QPushButton("Button");
}

inline QVariantHash testViewForCompound::visualPipeHash() const {
    return *m_VisualPipeHash;
}

inline void testViewForCompound::setVisualPipeHash(const QVariantHash hash) {
    if (m_VisualPipeHash == NULL) {
        m_VisualPipeHash = new QVariantHash();
    }
    m_VisualPipeHash->unite(hash);
}

inline void testViewForCompound::setRenderingWidget(QVariant renWidget) {
    m_RenderWidget = renWidget.value<QObject*>();
}

inline QVariant testViewForCompound::renderWidget() const {
    QVariant v;
    v.setValue<QObject*>(m_RenderWidget);
    return v;
}

inline bool testViewForCompound::parameterFlag() const {
    return m_ParameterFlag;
}

inline void testViewForCompound::setParameterFlag(bool parameter /* = true */) {
    m_ParameterFlag = parameter;
}
//----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
class testVisualPipeForCompound : public mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public:
    /// Object constructor.
    testVisualPipeForCompound(const QString code_location = "");

};

testVisualPipeForCompound::testVisualPipeForCompound(const QString code_location) : mafObjectBase(code_location) {
}
//----------------------------------------------------------------------------------------------


void mafViewCompoundConfiguratorTest::initTestCase() {
    // Register all objects for the mafGUI module.
    mafGUIRegistration::registerGUIObjects();
    mafRegisterObject(testViewForCompound);
    mafRegisterObject(testVisualPipeForCompound);

    m_ConfigFilename = MAF_DATA_DIR;
    m_ConfigFilename.append("/GUI/CompoundViewConfiguratorTestData.xml");
    
    m_Configurator = new mafViewCompoundConfigurator();
}

void mafViewCompoundConfiguratorTest::cleanupTestCase() {
    QObject *ro = m_Configurator->rootObject();
    if(ro) {
        delete ro;
    }
    delete m_Configurator;
}

void mafViewCompoundConfiguratorTest::mafViewCompoundConfiguratorAllocationTest() {
    QVERIFY(m_Configurator != NULL);
}

void mafViewCompoundConfiguratorTest::mafViewCompoundConfiguratorParseTest() {
    m_Configurator->parseConfigurationFile(m_ConfigFilename);
    QObject *obj = m_Configurator->rootObject();
    QVERIFY(obj != NULL);
}

#include "mafViewCompoundConfiguratorTest.moc"
