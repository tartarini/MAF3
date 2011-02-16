/*
 *  mafOpParametricSurfaceTest.cpp
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
#include <mafVisualPipeVTKSurface.h>
#include <mafOpParametricSurface.h>
#include <mafPluginManager.h>
#include <mafVMEManager.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAlgorithmOutput.h>
#include <mafContainer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QDebug>

using namespace mafResources;
using namespace mafEventBus;
using namespace mafPluginVTK;

/**
 Class name: mafOpParametricSurfaceTest
 This class implements the test suite for mafOpParametricSurface.
 */

//! <title>
//mafOpParametricSurface
//! </title>
//! <description>
//mafOpParametricSurface represent a VTK sphere surface with modificable parameters.
//! </description>

class mafOpParametricSurfaceTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        m_VMEManager = mafVMEManager::instance();

        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);

        // Callbacks related to the VME creation
        mafRegisterLocalCallback("maf.local.resources.vme.add", this, "vmeAdd(mafCore::mafObjectBase *)");

        // Create the parametric operation.
        m_OpParametric = mafNEW(mafPluginVTK::mafOpParametricSurface);

        // execute the operation
        m_OpParametric->execute();

        // get output of the operation
        m_VME = qobject_cast<mafVME *>(m_OpParametric->output());

        QVERIFY(m_VmeAdded->isEqual(m_VME));
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_OpParametric);
        mafMessageHandler::instance()->shutdown();
        m_VMEManager->shutdown();
    }

   /// Test Set/Get method of tparametric surface
    void SetGetTest();

public slots:
    void vmeAdd(mafCore::mafObjectBase *vme);


private:
    mafOpParametricSurface *m_OpParametric; ///< Parametric operation.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafVME *m_VmeAdded; ///< test object.
    mafResources::mafVMEManager *m_VMEManager; ///< instance of mafVMEManager.
};

void mafOpParametricSurfaceTest::SetGetTest() {
    mafVisualPipeVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafVisualPipeVTKSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("scalarVisibility", 0);
    pipe->setProperty("immediateRendering", 1);
    pipe->createPipe();
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafContainer
    mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, pipe->output());
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

void mafOpParametricSurfaceTest::vmeAdd(mafCore::mafObjectBase *vme){
    QVERIFY(vme != NULL);
    m_VmeAdded = qobject_cast<mafVME *>(vme);
    }


MAF_REGISTER_TEST(mafOpParametricSurfaceTest);
#include "mafOpParametricSurfaceTest.moc"


