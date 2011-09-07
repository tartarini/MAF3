/*
 *  mafVTKParametricSurfaceTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 20/01/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
//#include <mafPipeVisualVTKSurface.h>
#include <mafVTKParametricSurface.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkAlgorithmOutput.h>
//#include <mafProxy.h>
//#include <vtkRenderWindow.h>
//#include <vtkRenderer.h>
//#include <vtkActor.h>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

//------------------------------------------------------------------------------------------
/**
 Class name: testParametricSurfaceCustom
 This class implements the data pipe to be tested.
 */
class  testParametricSurfaceCustom : public  mafVTKParametricSurface {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafPluginVTK::mafVTKParametricSurface);

public:
    /// Object constructor.
    testParametricSurfaceCustom(const QString code_location = "");

    /// Update surface with parameters.
    /*virtual*/ void updateSurface();


private:
    QString m_PipeLine; ///< Test Var.
};

testParametricSurfaceCustom::testParametricSurfaceCustom(const QString code_location) : mafVTKParametricSurface(code_location) {
}

void testParametricSurfaceCustom::updateSurface() {
    // do nothing
}

//------------------------------------------------------------------------------------------

/**
 Class name: mafVTKParametricSurfaceTest
 This class implements the test suite for mafVTKParametricSurface.
 */

//! <title>
//mafVTKParametricSurface
//! </title>
//! <description>
//mafVTKParametricSurface is the base class of different type of parametric surface classes..
//! </description>

class mafVTKParametricSurfaceTest : public QObject {
    Q_OBJECT

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();

        // Create the parametric surface.
        m_ParametricSurface = mafNEW(testParametricSurfaceCustom);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_ParametricSurface);
        mafMessageHandler::instance()->shutdown();
    }

   /// Test Set/Get method of the parametric surface
    void SetGetCenterTest();


private:
    testParametricSurfaceCustom *m_ParametricSurface; ///< Parametric sphere.
};

void mafVTKParametricSurfaceTest::SetGetCenterTest() {
    double newCenter[3];
    newCenter[0] = 10;
    newCenter[1] = 5;
    newCenter[2] = 20.5;

    //Change center of the surface.
    m_ParametricSurface->setCenter(newCenter);

    //Check if center had changed.
    QCOMPARE(m_ParametricSurface->center()[0], 10.0);
    QCOMPARE(m_ParametricSurface->center()[1], 5.0);
    QCOMPARE(m_ParametricSurface->center()[2], 20.5);
}


MAF_REGISTER_TEST(mafVTKParametricSurfaceTest);
#include "mafVTKParametricSurfaceTest.moc"

