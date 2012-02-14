/*
 *  mafToolVTKPlaneTest.cpp
 *  mafPluginVTK
 *
 *  Created by Paolo Quadrani on 26/1/12.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafVME.h>
#include <mafSceneNode.h>
#include <mafDataSet.h>
#include <mafProxy.h>
#include <mafBounds.h>

#include <mafToolVTKPlane.h>
#include <mafToolHandler.h>

#include <mafVTKWidget.h>

#include <vtkAlgorithmOutput.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <QMainWindow>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafToolVTKPlaneTest
 This class creates a vtkPolyData and visualizes it trough the mafToolVTKSelection
 */
class mafToolVTKPlaneTest : public QObject {
    Q_OBJECT
    //initialize all the graphic resources
    void initializeGraphicResources();

    //shutdown all the graphic resources
    void shutdownGraphicResources();

private Q_SLOTS:
    /// Initialize test variables
    void initTestCase() {
        // Install the MAF3 Message handler for logging purposes...
        mafMessageHandler::instance()->installMessageHandler();

        // Register the mafResources objects and the visual pipe that has to be tested.
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObject(mafPluginVTK::mafToolVTKPlane);

        m_SphereActor = vtkActor::New();

        // Initialize the render widget...
        initializeGraphicResources();

        m_ToolHandler = mafNEW(mafResources::mafToolHandler);
        ((mafVTKWidget*)m_RenderWidget)->setToolHandler(m_ToolHandler);
        m_ToolHandler->release();

        m_ToolPlane = mafNEW(mafPluginVTK::mafToolVTKPlane);
        m_ToolPlane->setFollowSelectedObject(false);
        m_ToolPlane->setFollowSelectedObjectVisibility(false);

        m_ToolHandler->addTool(m_ToolPlane);

        // Create the input data source and wrap through the mafProxy
        m_DataSourceLow = vtkSphereSource::New();
        m_DataSourceLow->SetRadius(2.0);
        m_DataSourceLow->Update();
        m_DataSourceLowContainer = m_DataSourceLow->GetOutputPort(0);
        m_DataSourceHigh = vtkSphereSource::New();
        m_DataSourceHigh->SetThetaResolution(25);
        m_DataSourceHigh->SetPhiResolution(25);
        m_DataSourceHigh->Update();
        m_DataSourceHighContainer = m_DataSourceHigh->GetOutputPort(0);

        // put the container inside the mafDataSet
        mafDataSet *dataLow = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        dataLow->setBoundaryAlgorithm(boundaryAlgorithm);
        dataLow->setDataValue(&m_DataSourceLowContainer);

        mafDataSet *dataHigh = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm1;
        boundaryAlgorithm1 = mafNEW(mafDataBoundaryAlgorithmVTK);
        dataHigh->setBoundaryAlgorithm(boundaryAlgorithm1);
        dataHigh->setDataValue(&m_DataSourceHighContainer);

        m_VME = mafNEW(mafResources::mafVME);
        // Assign the dataset to the VME.
        m_VME->dataSetCollection()->insertItem(dataLow, 0.0);
        m_VME->dataSetCollection()->insertItem(dataHigh, 3.0);
        m_VME->setTimestamp(0.0);
        mafDEL(dataLow);
        mafDEL(dataHigh);

        m_SceneNode = new mafSceneNode(m_VME, m_RenderWidget, "");
        m_SceneNode->initialize();

        m_SphereMapper = vtkPolyDataMapper::New();
        m_SphereActor->SetMapper(m_SphereMapper);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        m_DataSourceLow->Delete();
        m_DataSourceHigh->Delete();

        m_SphereMapper->Delete();
        m_SphereActor->Delete();

        mafDEL(m_VME);
        mafDEL(m_SceneNode);
        mafDEL(m_ToolPlane);
        mafDEL(m_ToolHandler);
        mafMessageHandler::instance()->shutdown();

        shutdownGraphicResources();
    }

    /// Allocation test for the mafToolVTKSelection.
    void allocationTest();

    /// Test the update of the visual pipe for the sphere source at high resolution (timestamp 3)
    void updatePipeTest();

private:
    mafToolVTKPlane *m_ToolPlane; ///< Test variable.
    mafToolHandler  *m_ToolHandler; /// Handler for tools.
    vtkSphereSource *m_DataSourceLow; ///< Source data for the test suite.
    mafProxy<vtkAlgorithmOutput> m_DataSourceLowContainer; ///< Container of the Data Source
    mafProxy<vtkAlgorithmOutput> m_DataSourceHighContainer; ///< Container of the Data Source
    vtkSphereSource *m_DataSourceHigh; ///< Source data for the test suite.
    mafSceneNode *m_SceneNode; ///< SceneNode holding the VME.
    mafVME *m_VME; ///< VME vontaining input data.

    vtkPolyDataMapper *m_SphereMapper; ///< Mapper to show the VME inner data.
    vtkActor *m_SphereActor; ///< Actor for the m_SphereMapper.
    QObject *m_RenderWidget; /// renderer widget
    vtkRenderer *m_Renderer;
    QMainWindow *w;
};

void mafToolVTKPlaneTest::initializeGraphicResources() {
    w = new QMainWindow();
    w->setMinimumSize(640,480);
    w->setWindowTitle("mafToolVTKPlane Test");

    m_RenderWidget = new mafVTKWidget();
    ((mafVTKWidget*)m_RenderWidget)->setParent(w);

    m_Renderer = ((mafVTKWidget*)m_RenderWidget)->renderer();
    m_Renderer->AddActor(m_SphereActor);
    m_Renderer->SetBackground(0.1, 0.1, 0.1);

    ((mafVTKWidget*)m_RenderWidget)->update();
    w->show();
}

void mafToolVTKPlaneTest::shutdownGraphicResources() {
    w->close();
}

void mafToolVTKPlaneTest::allocationTest() {
    QVERIFY(m_ToolPlane != NULL);
}

void mafToolVTKPlaneTest::updatePipeTest() {
    mafDataSet *sphere = m_VME->dataSetCollection()->itemAtCurrentTime();
    mafProxy<vtkAlgorithmOutput> *dataSet = mafProxyPointerTypeCast(vtkAlgorithmOutput, sphere->dataValue());
    m_SphereMapper->SetInputConnection(*dataSet);

    QVariantList bv = sphere->bounds();
    double b[6];
    for (int i = 0; i < 6; ++i) {
        b[i] = bv.at(i).toDouble();
    }
    mafCore::mafBounds *bounds = new mafCore::mafBounds(b);
    m_ToolPlane->setVOI(bounds);
    m_ToolPlane->setVisibility(true);
    mafDEL(bounds);

    ((mafVTKWidget*)m_RenderWidget)->update();
    m_Renderer->ResetCamera();
    ((mafVTKWidget*)m_RenderWidget)->GetRenderWindow()->Render();

    QTest::qSleep(3000);
}

MAF_REGISTER_TEST(mafToolVTKPlaneTest);
#include "mafToolVTKPlaneTest.moc"
