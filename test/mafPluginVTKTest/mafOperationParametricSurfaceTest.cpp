/*
 *  mafOperationParametricSurfaceTest.cpp
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
#include <mafPipeVisualVTKSurface.h>
#include <mafOperationParametricSurface.h>
#include <mafPluginManager.h>
#include <mafVMEManager.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAlgorithmOutput.h>
#include <mafProxy.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafOperationParametricSurfaceTest
 This class implements the test suite for mafOperationParametricSurface.
 */

//! <title>
//mafOperationParametricSurface
//! </title>
//! <description>
//mafOperationParametricSurface represent a VTK sphere surface with modificable parameters.
//! </description>

class mafOperationParametricSurfaceTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);

        m_VMEManager = mafVMEManager::instance();

        // Create the parametric operation.
        m_OpParametric = mafNEW(mafPluginVTK::mafOperationParametricSurface);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_OpParametric);
        m_VMEManager->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// Test the operation's execution.
    void testExecute();

   /// Test Set/Get method of tparametric surface
    void SetGetTest();

private:
    mafOperationParametricSurface *m_OpParametric; ///< Parametric operation.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafVME *m_VmeAdded; ///< test object.
    mafResources::mafVMEManager *m_VMEManager; ///< instance of mafVMEManager.
};

void mafOperationParametricSurfaceTest::testExecute() {
    // execute the operation
    m_OpParametric->execute();

    // get output of the operation
    m_VME = qobject_cast<mafVME *>(m_OpParametric->output());

    QVERIFY(m_VME != NULL);
}

void mafOperationParametricSurfaceTest::SetGetTest() {
    mafPipeVisualVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("scalarVisibility", 0);
    pipe->setProperty("immediateRendering", 1);
    pipe->createPipe();
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafProxy
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, pipe->output());
    QVERIFY(actor != NULL);

    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    // Connect the actor (contained into the container) with the renderer.
    renderer->AddActor(*actor);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    renderer->SetBackground(0.1, 0.1, 0.1);
    renWin->SetSize(640, 480);
    renWin->SetPosition(400,0);

    renWin->Render();
    //iren->Start();
    QTest::qSleep(2000);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(pipe);
}


MAF_REGISTER_TEST(mafOperationParametricSurfaceTest);
#include "mafOperationParametricSurfaceTest.moc"

