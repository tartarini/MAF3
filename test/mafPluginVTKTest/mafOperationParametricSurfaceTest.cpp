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

#include <mafVTKWidget.h>

#include <vtkRenderWindowInteractor.h>
#include <vtkAlgorithmOutput.h>
#include <mafProxy.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <QMainWindow>
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
    //initialize all the graphic resources
    void initializeGraphicResources();

    //shutdown all the graphic resources
    void shutdownGraphicResources();

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);

        m_VMEManager = mafVMEManager::instance();

        // Create the parametric operation.
        m_OpParametric = mafNEW(mafPluginVTK::mafOperationParametricSurface);
        m_RenderWidget = new mafVTKWidget();
        initializeGraphicResources();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_OpParametric);
        m_VMEManager->shutdown();
        mafMessageHandler::instance()->shutdown();
        delete m_RenderWidget;
        shutdownGraphicResources();
    }

    /// Test the operation's execution.
    void testExecute();

    /// visualize parametric surface
    void visualizeTest();

private:
    mafOperationParametricSurface *m_OpParametric; ///< Parametric operation.
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafVME *m_VmeAdded; ///< test object.
    mafResources::mafVMEManager *m_VMEManager; ///< instance of mafVMEManager.
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer; ///< Accessory renderer
    QMainWindow *w;
};

void mafOperationParametricSurfaceTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();

    m_Renderer->SetBackground(0.1, 0.1, 0.1);
    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafOperationParametricSurfaceTest::shutdownGraphicResources() {
    w->close();
}

void mafOperationParametricSurfaceTest::testExecute() {
    //Set parameters ( create a cube )
    QVariantMap parameters;
    parameters.insert("type", QVariant(1));
    parameters.insert("cubeXLength", QVariant(5.0));
    parameters.insert("cubeYLength", QVariant(5.0));
    parameters.insert("cubeZLength", QVariant(5.0));
    m_OpParametric->setParameters(parameters);

    // execute the operation
    m_OpParametric->execute();

    // get output of the operation
    m_VME = qobject_cast<mafVME *>(m_OpParametric->output());

    QVERIFY(m_VME != NULL);
}

void mafOperationParametricSurfaceTest::visualizeTest() {
    mafPipeVisualVTKSurface *pipe;
    pipe = mafNEW(mafPluginVTK::mafPipeVisualVTKSurface);
    pipe->setInput(m_VME);
    pipe->setProperty("scalarVisibility", 0);
    pipe->setProperty("immediateRendering", 1);
    pipe->setGraphicObject(m_RenderWidget);
    pipe->updatePipe();

    // Get the vtkActor from the visual pipe
    // And assign to a mafProxy
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, pipe->output());
    QVERIFY(actor != NULL);

    // Connect the actor (contained into the container) with the renderer.
    m_Renderer->AddActor(*actor);

    ((mafVTKWidget*)m_RenderWidget)->update();
    m_Renderer->ResetCamera();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();
    //iren->Start();
    QTest::qSleep(2000);

    pipe->setGraphicObject(NULL);
    mafDEL(pipe);
}

MAF_REGISTER_TEST(mafOperationParametricSurfaceTest);
#include "mafOperationParametricSurfaceTest.moc"


