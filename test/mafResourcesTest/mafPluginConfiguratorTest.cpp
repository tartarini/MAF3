/*
 *  mafPluginConfiguratorTest.cpp
 *  mafResourcesTest
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 06/06/11.
 *  Copyright 2011 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafResourcesTestList.h"

using namespace mafEventBus;
using namespace mafResources;

void mafPluginConfiguratorTest::initTestCase() {
    mafMessageHandler::instance()->installMessageHandler();
    mafResourcesRegistration::registerResourcesObjects();
    initializeTestData();
}

/// Cleanup test variables memory allocation.
void mafPluginConfiguratorTest::cleanupTestCase() {
    QFile::remove(m_ConfigFile);
    mafMessageHandler::instance()->shutdown();
}

void mafPluginConfiguratorTest::initializeTestData() {
    m_ConfigFile = QDir::tempPath();
    m_ConfigFile.append("/maf3TestData");
    QDir log_dir(m_ConfigFile);
    if(!log_dir.exists()) {
        log_dir.mkpath(m_ConfigFile);
    }
    m_ConfigFile.append("/mafPluginConfiguration.xml");
    QFile f(m_ConfigFile);
    if (!f.open(QIODevice::WriteOnly)) {
        qCritical() << mafTr("Not able to create test data...");
        QVERIFY(false);
    }
    f.write("<plugin>\
        <plug classtype=\"mafPluginVTK::mafPipeVisualVTKSurface\" baseclass=\"mafResources::mafPipeVisual\" label=\"Visual pipe VTK Surface\"></plug>\
        <codec classtype=\"mafPluginVTK::mafExternalDataCodecVTK\" encodetype=\"VTK\"></codec>\
        <serializer classtype=\"mafSerialization::mafSerializerFileSystem\" schematype=\"file\"></serializer>\
        </plugin>");
    f.close();
}

void mafPluginConfiguratorTest::mafPluginConfiguratorParseTest() {
    mafPluginConfigurator configurator;
    bool res;

    // Not existing file => configurator should return false.
    res = configurator.parseConfigurationFile("badFile.xml");
    QVERIFY(res == false);

    // Well formatted and existing file => configurator should return true.
    res = configurator.parseConfigurationFile(m_ConfigFile);
    QVERIFY(res);
}

#include "mafPluginConfiguratorTest.moc"
