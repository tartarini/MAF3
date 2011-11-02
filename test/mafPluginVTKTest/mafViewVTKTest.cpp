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
#include <mafVisitorFindSceneNodeByVMEHash.h>
#include <mafResourcesRegistration.h>
#include <mafVMEManager.h>
#include <mafView.h>
#include <mafViewVTK.h>
#include <mafPipeVisualVTKSurface.h>
#include <mafDataBoundaryAlgorithmVTK.h>
#include <mafSceneNode.h>
#include <mafVME.h>
#include <mafProxy.h>
#include <mafDataSet.h>

#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkCubeSource.h>
#include <vtkRenderWindow.h>
#include <vtkAlgorithmOutput.h>

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

private Q_SLOTS:

    /// Initialize test variables
    void initTestCase() {
        mafMessageHandler::instance()->installMessageHandler();
        mafResourcesRegistration::registerResourcesObjects();
        mafRegisterObjectAndAcceptBind(mafPluginVTK::mafPipeVisualVTKSurface);
        mafVMEManager::instance();

        m_BindingHash.insert("vtkPolyData","mafPluginVTK::mafPipeVisualVTKSurface");

        /// Create the view...
        m_View = mafNEW(mafPluginVTK::mafViewVTK);

        // Create a polydata.
        m_DataSource = vtkCubeSource::New();
        m_DataSource->SetXLength(5);
        m_DataSource->SetYLength(3);
        m_DataSource->SetZLength(8);

        m_DataSourceContainer.setExternalCodecType("mafPluginVTK::mafExternalDataCodecVTK");
        m_DataSourceContainer.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainer = m_DataSource->GetOutputPort(0);

        //Insert data into VME
        m_VmeCube = mafNEW(mafResources::mafVME);
        m_VmeCube->setObjectName("VME Cube");
        m_DataSetCube = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm;
        boundaryAlgorithm = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSetCube->setBoundaryAlgorithm(boundaryAlgorithm);
        m_DataSetCube->setDataValue(&m_DataSourceContainer);
        m_VmeCube->dataSetCollection()->insertItem(m_DataSetCube, 0);
        m_DataSetCube->release();

        // Second test VME
        m_DataSourceMoved = vtkCubeSource::New();
        m_DataSourceMoved->SetXLength(5);
        m_DataSourceMoved->SetYLength(3);
        m_DataSourceMoved->SetZLength(8);

        vtkTransform *t = vtkTransform::New();
        t->RotateZ(50);
        t->RotateX(50);
        t->Update();

        m_PDataFilter = vtkTransformPolyDataFilter::New();
        m_PDataFilter->SetTransform(t);
        m_PDataFilter->SetInputConnection(m_DataSourceMoved->GetOutputPort(0));
        m_PDataFilter->Update();
        t->Delete();

        m_DataSourceContainerMoved.setExternalCodecType("mafPluginVTK::mafExternalDataCodecVTK");
        m_DataSourceContainerMoved.setClassTypeNameFunction(vtkClassTypeNameExtract);
        m_DataSourceContainerMoved = m_PDataFilter->GetOutputPort(0);
        m_DataSetCubeMoved = mafNEW(mafResources::mafDataSet);
        mafDataBoundaryAlgorithmVTK *boundaryAlgorithm1;
        boundaryAlgorithm1 = mafNEW(mafDataBoundaryAlgorithmVTK);
        m_DataSetCubeMoved->setBoundaryAlgorithm(boundaryAlgorithm1);
        m_DataSetCubeMoved->setDataValue(&m_DataSourceContainerMoved);
        
        mafMatrix mat;
        mat.setIdentity();
        m_DataSetCube->setPoseMatrix(&mat);
        m_DataSetCubeMoved->setPoseMatrix(&mat);

        m_VmeCubeMoved = mafNEW(mafResources::mafVME);
        m_VmeCubeMoved->setObjectName("Moved VME Cube");
        m_VmeCubeMoved->dataSetCollection()->insertItem(m_DataSetCubeMoved, 0);
        m_DataSetCubeMoved->release();
    }

    /// Cleanup test variables memory allocation.
    void cleanupTestCase() {
        //To close widget window
        QObject *widgetObj = m_View->property("renderWidget").value<QObject*>();
        QWidget *widget = qobject_cast<QWidget*>(widgetObj);
        widget->close();

        mafDEL(m_View);
        mafDEL(m_VmeCube);
        mafDEL(m_VmeCubeMoved);
        m_PDataFilter->Delete();
        m_DataSource->Delete();
        m_DataSourceMoved->Delete();
        mafVMEManager::instance()->shutdown();
        mafMessageHandler::instance()->shutdown();
    }

    /// mafViewVTK allocation test case.
    void mafViewVTKAllocationTest();

    /// mafViewVTK 2 VME visualization test case.
    void mafViewVTKCreateView2VMETest();
    
    /// mafViewVTK test visualization using pipevisual.
    void mafViewPipeVisualTest();

private:
    mafViewVTK *m_View; ///< Test var.
    mafVME *m_VmeCube; ///< Test var.
    mafVME *m_VmeCubeMoved; ///< Test var.
    vtkCubeSource *m_DataSource;
    vtkCubeSource *m_DataSourceMoved;
    vtkTransformPolyDataFilter *m_PDataFilter; ///< Filter used to transform the bounding box.
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainer; ///< Container of the Data Source
    mafProxy<vtkAlgorithmOutput> m_DataSourceContainerMoved; ///< Container of the Data S
    mafResources::mafDataSet *m_DataSetCube;
    mafResources::mafDataSet *m_DataSetCubeMoved;
    QHash<QString, QString> m_BindingHash;
};

void mafViewVTKTest::mafViewVTKAllocationTest() {
    QVERIFY(m_View != NULL);
}

void mafViewVTKTest::mafViewVTKCreateView2VMETest() {
    //! <snippet>
    m_View->initialize();

    // Ask for the inner widget.
    QObject *widgetObj = m_View->property("renderWidget").value<QObject*>();
    QWidget *widget = qobject_cast<QWidget*>(widgetObj);

    widget->resize(QSize(400, 400));
    widget->show();

    QString name_result = widget->objectName();
    QCOMPARE(name_result, QString("VTKWidget"));

    //Request VME Hierarchy.
    mafCore::mafHierarchyPointer hierarchy;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, hierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
    QVERIFY(hierarchy != NULL);

    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VmeCube));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    m_View->plugVisualPipeBindingHash(&m_BindingHash);
    //! </snippet>

    // Visualize first cube
    QString h = m_VmeCube->objectHash();
    mafVisitorFindSceneNodeByVMEHash *v = new mafVisitorFindSceneNodeByVMEHash(h, mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v);
    mafSceneNode *cubeNode = v->sceneNode();
    m_View->showSceneNode(cubeNode, true);
    mafDEL(v);

    QTest::qSleep(2000);
    // Visualize also second cube (I could customize visualPipe)
    argList.clear();
    argList.append(mafEventArgument(mafCore::mafObjectBase *, m_VmeCubeMoved));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);

    v = new mafVisitorFindSceneNodeByVMEHash(m_VmeCubeMoved->objectHash(), mafCodeLocation);
    mafObjectRegistry::instance()->findObjectsThreaded(v);
    mafSceneNode *cubeMovedNode = v->sceneNode();
    mafDEL(v);
    m_View->showSceneNode(cubeMovedNode, true);
    QTest::qSleep(2000);

    // Show off first cube
    m_View->showSceneNode(cubeNode, false);
    QTest::qSleep(2000);    
}

void mafViewVTKTest::mafViewPipeVisualTest(){
    
}


MAF_REGISTER_TEST(mafViewVTKTest);
#include "mafViewVTKTest.moc"
