/*
 *  mafViewVTKTest.cpp
 *  mafPluginVTKTest
 *
 *  Created by Roberto Mucci on 26/03/10.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <mafTestSuite.h>
#include <mafCoreSingletons.h>
#include <mafResourcesRegistration.h>
#include <mafView.h>
#include <mafViewVTK.h>
#include <mafVisualPipeVTKSurface.h>
#include <mafVME.h>
#include <mafContainer.h>
#include <mafDataSet.h>

#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkCellArray.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>

#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafResources;
using namespace mafPluginVTK;

/**
 Class name: mafViewTest
 This class implements the test suite for mafViewVTK.
 */

//! <title>
//mafViewVTK
//! </title>
//! <description>
//mafViewVTK defines the VTK view in MAF3.
//! </description>

class mafViewVTKTest : public QObject {
    Q_OBJECT

private slots:
    /// Initialize test variables
    void initTestCase() {
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafVisualPipeVTKSurface);

        /// Create the view...
        m_View = mafNEW(mafPluginVTK::mafViewVTK);

        /// Create the render window on which render the objects.
        m_RenWin = vtkRenderWindow::New();

        // Assign the render window to the view.
        m_View->setRenderingWindow(&m_RenWin);
        
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
        
        // First test VME
        m_VmeCube = mafNEW(mafResources::mafVME);
        m_Container.setDestructionFunction(&vtkPolyData::Delete);
        m_Container = surface;
        mafDataSet *dataSetCube = mafNEW(mafResources::mafDataSet);
        dataSetCube->setDataValue(&m_Container);
        //Insert data into VME
        m_VmeCube->dataSetCollection()->insertItem(dataSetCube);

        // Second test VME
        vtkPolyData *surfaceMoved = vtkPolyData::New();
        surfaceMoved->DeepCopy(surface);
        surfaceMoved->Update();

        vtkTransform *t = vtkTransform::New();
        t->RotateZ(50);
        t->RotateX(50);
        t->Update();

        vtkTransformPolyDataFilter *ptf = vtkTransformPolyDataFilter::New();
        ptf->SetTransform(t);
        ptf->SetInput(surfaceMoved);
        ptf->Update();
        t->Delete();

        surfaceMoved->DeepCopy(ptf->GetOutput());
        surfaceMoved->Update();
        ptf->Delete();

        m_VmeCubeMoved = mafNEW(mafResources::mafVME);
        m_ContainerMoved.setDestructionFunction(&vtkPolyData::Delete);
        m_ContainerMoved = surfaceMoved;
        mafDataSet *dataSet = mafNEW(mafResources::mafDataSet);
        dataSet->setDataValue(&m_ContainerMoved);
        //Insert data into VME
        m_VmeCubeMoved->dataSetCollection()->insertItem(dataSet);
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        mafDEL(m_View);
        mafDEL(m_VmeCube);
        mafDEL(m_VmeCubeMoved);

        mafCoreSingletons::mafSingletonsShutdown();
    }

    /// mafViewVTK allocation test case.
    void mafViewVTKAllocationTest();

    /// mafViewVTK 2 VME visualization test case.
    void mafViewVTKCreateView2VMETest();

private:
    mafViewVTK *m_View; ///< Test var.
    mafCore::mafContainer<vtkRenderWindow> m_RenWin; ///< Test RenderWindow.
    mafVME *m_VmeCube; ///< Test var.
    mafVME *m_VmeCubeMoved; ///< Test var.
    mafContainer<vtkPolyData> m_Container; ///< Test var.
    mafContainer<vtkPolyData> m_ContainerMoved; ///< Test var.
};

void mafViewVTKTest::mafViewVTKAllocationTest() {
    QVERIFY(m_View != NULL);
}

void mafViewVTKTest::mafViewVTKCreateView2VMETest() {
    //! <snippet>
    m_View->create();
    m_View->addVME(m_VmeCube);
    m_View->plugVisualPipe("vtkPolyData","mafPluginVTK::mafVisualPipeVTKSurface");
    //! </snippet>
    // Visualize first cube
    m_View->showVME(m_VmeCube, true);
    QTest::qSleep(2000);
    // Visualize also second cube (I could cutomize visualPipe)
    m_View->addVME(m_VmeCubeMoved);
    m_View->showVME(m_VmeCubeMoved, true,  "mafPluginVTK::mafVisualPipeVTKSurface");
    QTest::qSleep(2000);
    // Show off first cube
    m_View->showVME(m_VmeCube, false);
    QTest::qSleep(2000);

    // Remove VME
    m_View->removeVME(m_VmeCube);
}

MAF_REGISTER_TEST(mafViewVTKTest);
#include "mafViewVTKTest.moc"
