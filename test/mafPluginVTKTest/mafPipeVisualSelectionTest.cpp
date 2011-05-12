/*
 *  mafPipeVisualSelectionTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 11/11/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafPipeVisualSelection.h>
#include <mafVME.h>
#include <mafDataSet.h>
#include <mafProxy.h>

#include <vtkAlgorithmOutput.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafPipeVisualSelectionTest
 This class creates a vtkPolyData and visualizes it trough the mafPipeVisualSelection
 */
class mafPipeVisualSelectionTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        // Install the MAF3 Message handler for logging pourposes...
        mafMessageHandler::instance()->installMessageHandler();

        // Register the mafResources objects and the visual pipe that has to be tested.
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualSelection);

        m_VisualPipeSelection = mafNEW(mafPluginVTK::mafPipeVisualSelection);

        // Create the input data source and wrap through the mafProxy
        m_DataSourceLow = vtkSphereSource::New();
        m_DataSourceLow->SetRadius(2.0);
        m_DataSourceLowContainer = m_DataSourceLow->GetOutputPort(0);
        m_DataSourceHigh = vtkSphereSource::New();
        m_DataSourceHigh->SetThetaResolution(25);
        m_DataSourceHigh->SetPhiResolution(25);
        m_DataSourceHighContainer = m_DataSourceHigh->GetOutputPort(0);

        // put the container inside the mafDataSet
        mafDataSet *dataLow = mafNEW(mafResources::mafDataSet);
        dataLow->setDataValue(&m_DataSourceLowContainer);

        mafDataSet *dataHigh = mafNEW(mafResources::mafDataSet);
        dataHigh->setDataValue(&m_DataSourceHighContainer);

        m_VME = mafNEW(mafResources::mafVME);
        // Assign the dataset to the VME.
        m_VME->dataSetCollection()->insertItem(dataLow, 0.0);
        m_VME->dataSetCollection()->insertItem(dataHigh, 3.0);
        m_VME->setTimestamp(0.0);
        mafDEL(dataLow);
        mafDEL(dataHigh);

        m_SphereMapper = vtkPolyDataMapper::New();
        m_SphereActor = vtkActor::New();
        m_SphereActor->SetMapper(m_SphereMapper);

        m_RenWin = vtkRenderWindow::New();
        m_Renderer = vtkRenderer::New();
        m_Renderer->AddActor(m_SphereActor);
        m_Iren = vtkRenderWindowInteractor::New();
        m_RenWin->AddRenderer(m_Renderer);
        m_Iren->SetRenderWindow(m_RenWin);

        m_Renderer->SetBackground(0.1, 0.1, 0.1);
        m_RenWin->SetSize(640, 480);
        m_RenWin->SetPosition(200,0);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_DataSourceLow->Delete();
        m_DataSourceHigh->Delete();

        m_SphereMapper->Delete();
        m_SphereActor->Delete();

        m_RenWin->Delete();
        m_Renderer->Delete();
        m_Iren->Delete();

        mafDEL(m_VME);
        mafDEL(m_VisualPipeSelection);
        mafMessageHandler::instance()->shutdown();
    }

    /// Allocation test for the mafPipeVisualSelection.
    void allocationTest();

    /// Test the creation of the visual pipe for the sphere source at low resolution (timestamp 0)
    void createPipeTest();

    /// Test the update of the visual pipe for the sphere source at high resolution (timestamp 3)
    void updatePipeTest();

private:
    mafPipeVisualSelection *m_VisualPipeSelection; ///< Test variable.
    vtkSphereSource *m_DataSourceLow; ///< Source data for the test suite.
    mafProxy<vtkAlgorithmOutput> m_DataSourceLowContainer; ///< Container of the Data Source
    mafProxy<vtkAlgorithmOutput> m_DataSourceHighContainer; ///< Container of the Data Source
    vtkSphereSource *m_DataSourceHigh; ///< Source data for the test suite.
    mafVME *m_VME; ///< VME holding the input data source.

    vtkPolyDataMapper *m_SphereMapper; ///< Mapper to show the VME inner data.
    vtkActor *m_SphereActor; ///< Actor for the m_SphereMapper.
    vtkRenderWindow *m_RenWin; ///< Accessory render window to show the rendered data
    vtkRenderer *m_Renderer; ///< Accessory renderer
    vtkRenderWindowInteractor *m_Iren; ///< Accessory interactor.
};

void mafPipeVisualSelectionTest::allocationTest() {
    QVERIFY(m_VisualPipeSelection != NULL);
    QVERIFY(mafPipeVisualSelection::acceptObject(m_VME));
    m_VisualPipeSelection->setInput(m_VME);
}

void mafPipeVisualSelectionTest::createPipeTest() {
    m_VisualPipeSelection->createPipe();
    mafProxy<vtkActor> *actor = mafProxyPointerTypeCast(vtkActor, m_VisualPipeSelection->output());
    QVERIFY(actor != NULL);

    mafDataSet *sphere = m_VME->dataSetCollection()->itemAt(0);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, sphere->dataValue());
    m_SphereMapper->SetInputConnection(*dataSet);

    // Connect the actor (contained into the container) with the renderer.
    m_Renderer->AddActor(*actor);

    m_RenWin->Render();
    QTest::qSleep(2000);
}

void mafPipeVisualSelectionTest::updatePipeTest() {
    m_VisualPipeSelection->updatePipe(3.0);

    mafDataSet *sphere = m_VME->dataSetCollection()->itemAt(3.0);
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, sphere->dataValue());
    m_SphereMapper->SetInputConnection(*dataSet);

    m_RenWin->Render();
    QTest::qSleep(2000);
}

MAF_REGISTER_TEST(mafPipeVisualSelectionTest);
#include "mafPipeVisualSelectionTest.moc"
