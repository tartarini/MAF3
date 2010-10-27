/*
 *  mafVisualPipeVTKSurfaceTest.cpp
 *  mafPluginVTK
 *
 *  Created by Roberto Mucci on 01/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafEventBusManager.h>
#include <mafVisualPipeVTKSurface.h>
#include <mafResourcesRegistration.h>
#include <mafVisualPipe.h>
#include <mafContainer.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <mafPluginManager.h>
#include <mafPlugin.h>

// render window stuff
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#ifdef WIN32
    #define TEST_LIBRARY_NAME "mafPluginVTK_d.dll"
#else
    #ifdef __APPLE__
        #define TEST_LIBRARY_NAME "libmafPluginVTK_debug.dylib"
    #else
        #define TEST_LIBRARY_NAME "libmafPluginVTK_debug.so"
    #endif
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;


/**
 Class name: mafVisualPipeVTKSurfaceTest
 This class creates a vtkPolyData and visualizes it trough the mafVisualPipeVTKSurface
 */

//! <title>
//mafVisualPipeVTKSurface
//! </title>
//! <description>
//mafVisualPipeVTKSurface is a visual pipe used to render VTK polydata.
//It takes a mafDataSet as input and returns a mafContainer<vtkActor>.
//! </description>

class mafVisualPipeVTKSurfaceTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);

        // Create a polydata.
        vtkPolyData *surface = vtkPolyData::New();
        vtkPoints *points = vtkPoints::New();
        vtkCellArray *polys = vtkCellArray::New();
        vtkFloatArray *scalars = vtkFloatArray::New();

        int i;
        static float x[8][3]={{0,0,0}, {1,0,0}, {1,1,0}, {0,1,0},
                        {0,0,1}, {1,0,1}, {1,1,1}, {0,1,1}};
        static vtkIdType pts[6][4]={{0,1,2,3}, {4,5,6,7}, {0,1,5,4},
                        {1,2,6,5}, {2,3,7,6}, {3,0,4,7}};
        for (i=0; i<8; i++) points->InsertPoint(i,x[i]);
        for (i=0; i<6; i++) polys->InsertNextCell(4,pts[i]);
        for (i=0; i<8; i++) scalars->InsertTuple1(i,i);

        // We now assign the pieces to the vtkPolyData.
        surface->SetPoints(points);
        surface->SetPolys(polys);
        surface->GetPointData()->SetScalars(scalars);
        points->Delete();
        polys->Delete();
        scalars->Delete();

        //! <snippet>
        //// Create a container with a vtkPolyData
        //// m_PolyData is the container of type vtkPolyData
        //// to "wrap" the 'surface' of type vtkPolyData just simply use the code below.
        m_PolyData = surface;
        //// Assign the destruction function that will be used to delete the external data
        //// by the mafContainer when destroyed.
        m_PolyData.setDestructionFunction(&vtkPolyData::Delete);

        //// and give it to the mafDataSet.
        m_VME = mafNEW(mafResources::mafVME);
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        dataSet->setDataValue(&m_PolyData);
        m_VME->dataSetCollection()->insertItem(dataSet);
        //! </snippet>

    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_VME);
        mafEventBusManager::instance()->shutdown();
    }

    /// Test the creation of the vtkActor
    void updatePipeTest();

    /// Test the creation of the vtkActor
    void updatePipeTestFromPlugIn();

private:
    mafVME *m_VME; ///< Contain the only item vtkPolydata representing a surface.
    mafContainer<vtkPolyData> m_PolyData; ///< Container of the vtkPolyData
};

void mafVisualPipeVTKSurfaceTest::updatePipeTest() {


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

    //Change scalarVisibility flag
    pipe->setProperty("scalarVisibility", 1);
    pipe->updatePipe();
    renWin->Render();
    QTest::qSleep(2000);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(pipe);
}

void mafVisualPipeVTKSurfaceTest::updatePipeTestFromPlugIn() {

    mafPluginManager *pluginManager = mafPluginManager::instance();
    mafString pluginName = TEST_LIBRARY_NAME;

    // Load the library containing the visual pipe that I want to plug-in.
    pluginManager->loadPlugin(pluginName);


    // Dump the plug-in information.
    mafPluginInfo info = pluginManager->pluginInformation(pluginName);
    mafMsgDebug() << "Plug-in Information:";
    mafMsgDebug() << "Varsion: " << info.m_Version;
    mafMsgDebug() << "Author: " << info.m_Author;
    mafMsgDebug() << "Vendor: " << info.m_Vendor;
    mafMsgDebug() << "Description: " << info.m_Description;


    mafStringList binding_class_list;
    binding_class_list = mafResourcesRegistration::acceptObject(m_VME->dataSetCollection()->itemAtCurrentTime());
    int num = binding_class_list.count();
    QVERIFY(num == 1);

    mafString visualPipeType = binding_class_list.at(0);
    QCOMPARE(visualPipeType,mafString("mafPluginVTK::mafVisualPipeVTKSurface"));

    //! <snippet>
    mafVisualPipe *visualPipe = (mafVisualPipe *)mafNEWFromString(visualPipeType);
    visualPipe->setProperty("scalarVisibility", 1);
    visualPipe->setInput(m_VME);
    visualPipe->createPipe();
    visualPipe->updatePipe();
    //! </snippet>

    //! <snippet>
    mafContainer<vtkActor> *actor = mafContainerPointerTypeCast(vtkActor, visualPipe->output());
    //! </snippet>
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    vtkRenderer *renderer = vtkRenderer::New();
    vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();

    renderer->AddActor(*actor);
    renWin->AddRenderer(renderer);
    iren->SetRenderWindow(renWin);

    renderer->SetBackground(0.1, 0.1, 0.1);
    renWin->SetSize(640, 480);
    renWin->SetPosition(400,0);

    renWin->Render();
    //iren->Start();
    QTest::qSleep(2000);

    //Change scalarVisibility flag
    visualPipe->setProperty("scalarVisibility", 0);
    visualPipe->updatePipe();
    renWin->Render();
    QTest::qSleep(2000);

    renWin->Delete();
    renderer->Delete();
    iren->Delete();
    mafDEL(visualPipe);

    pluginManager->shutdown();
}


MAF_REGISTER_TEST(mafVisualPipeVTKSurfaceTest);
#include "mafVisualPipeVTKSurfaceTest.moc"
