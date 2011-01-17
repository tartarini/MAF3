/*
 *  mafMainWindow.cpp
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMainWindow.h"
#include "ui_mafMainWindow.h"

#include <mafEventBusManager.h>

#include "googlechat.h"

#include <mafGUIRegistration.h>
#include <mafTreeModel.h>

#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>

using namespace mafCore;
using namespace mafGUI;

mafMainWindow::mafMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mafMainWindow), m_Logic(NULL) {
    initializeMainWindow();
}

mafMainWindow::mafMainWindow(mafApplicationLogic::mafLogic *logic, QWidget *parent) :QMainWindow(parent), ui(new Ui::mafMainWindow), m_Logic(logic) {
    initializeMainWindow();
}

void mafMainWindow::setLogic(mafApplicationLogic::mafLogic *logic) {
    if(m_Logic) {
        mafDEL(m_Logic);
    }
    m_Logic = logic;
    if(m_Model) {
        m_Model->setHierarchy(m_Logic->hierarchy());
    }
}

void mafMainWindow::initializeMainWindow() {
    ui->setupUi(this);

    mafGUIRegistration::registerGUIObjects();
    m_GUIManager = new mafGUIManager(this, mafCodeLocation);

    m_GUIManager->createMenus();
    m_GUIManager->createToolBars();

    connectCallbacks();

    ui->statusBar->showMessage(mafTr("Ready!"));

    // Connecting layouts (needed because from QtDesign is not managed automatically)
    ui->centralWidget->setLayout(ui->gridLayout);
    ui->sideBarDockContents->setLayout(ui->gridLayoutSideBar);
    ui->logBarWidgetContents->setLayout(ui->gridLayoutLogBar);
    ui->tabTree->setLayout(ui->gridLayoutTree);
    ui->tabProperties->setLayout(ui->gridLayoutProperties);
    ui->tabOperation->setLayout(ui->gridLayoutOperation);

    //tree widget in sidebar
    m_Model = new mafTreeModel();

    if(m_Logic) {
        m_Model->setHierarchy(m_Logic->hierarchy());
     }

    // **** SideBar ****
    m_Tree = m_GUIManager->createTreeWidget(m_Model, ui->tabTree);

    // SideBar visibility management
    QObject *sideBarAction = m_GUIManager->actionByName("SideBar");
    connect(ui->dockSideBar, SIGNAL(visibilityChanged(bool)), sideBarAction, SLOT(setChecked(bool)));
    connect(sideBarAction, SIGNAL(triggered(bool)), ui->dockSideBar, SLOT(setVisible(bool)));

    // **** LogBar ****
    QObject *logBarAction = m_GUIManager->actionByName("LogBar");
    m_LogWidget = m_GUIManager->createLogWidget(ui->logBarWidgetContents);
    connect(ui->dockLogBarWidget, SIGNAL(visibilityChanged(bool)), logBarAction, SLOT(setChecked(bool)));
    connect(logBarAction, SIGNAL(triggered(bool)), ui->dockLogBarWidget, SLOT(setVisible(bool)));

    // **** Google chat ****
    QObject *collaborateAction = m_GUIManager->actionByName("Collaborate");
    m_DockGoogleChat = new QDockWidget(tr("Google Chat"));
    m_DockGoogleChat->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_DockGoogleChat->setWidget(new GoogleChat());
    if(collaborateAction) {
        connect(collaborateAction, SIGNAL(triggered(bool)), this, SLOT(updateCollaborationDockVisibility(bool)));
        connect(m_DockGoogleChat, SIGNAL(visibilityChanged(bool)), collaborateAction, SLOT(setChecked(bool)));
    }

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(viewSelected(QMdiSubWindow*)));

    setUnifiedTitleAndToolBarOnMac(true);

    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.logic.settings.restore");
}

void mafMainWindow::connectCallbacks() {
    mafRegisterLocalCallback("maf.local.gui.action.new", this, "createViewWindow()");
    mafRegisterLocalCallback("maf.local.gui.action.save", this, "save()");
    mafRegisterLocalCallback("maf.local.gui.action.about", this, "showAbout()");

    mafRegisterLocalCallback("maf.local.logic.settings.store", this, "writeSettings()");
    mafRegisterLocalCallback("maf.local.logic.settings.restore", this, "readSettings()");
}

mafMainWindow::~mafMainWindow() {
    mafDEL(m_GUIManager);
    delete ui;
}

void mafMainWindow::showAbout() {
    QMessageBox::about(this, mafTr("About"), mafTr("Simple App v1.0 \nFirst Simple MAF3 application."));
}

void mafMainWindow::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void mafMainWindow::closeEvent(QCloseEvent *event) {
    releaseMouse();
    mafCoreSingletons::mafSingletonsShutdown();
    int ret = maybeSave();
    if (ret == QMessageBox::Save) {
        // Save settings
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.logic.settings.store");
        // .. and maybe should save also application's data.
        // ToDo: Save the Application's data.
        // Then accept the close of the application
        event->accept();
    } else if(ret == QMessageBox::Discard) {
        event->accept();
    } else {
        event->ignore();
    }
}

void mafMainWindow::readSettings() {
    mafMsgDebug() << "Reading mafMainWindows settings...";
    mafSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(600, 400)).toSize();
    resize(size);
    move(pos);
}

void mafMainWindow::writeSettings() {
    mafMsgDebug() << "Writing mafMainWindows settings...";
    mafSettings settings;
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

int mafMainWindow::maybeSave() {
    if (true) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
//        if (ret == QMessageBox::Save)
//            return save();
//        else if (ret == QMessageBox::Cancel)
//            return false;
//        else if(ret == QMessageBox::Discard)
//            return false;
        return ret;
    }
    return QMessageBox::Save;
}

bool mafMainWindow::save() {
    return true;
}

void mafMainWindow::updateCollaborationDockVisibility(bool visible) {
    if(m_DockGoogleChat->parent() == NULL) {
        this->addDockWidget(Qt::LeftDockWidgetArea, m_DockGoogleChat);
    }
    m_DockGoogleChat->setVisible(visible);
}

void mafMainWindow::createViewWindow() {
    static int windowCounter = 1;

    // sphere
    vtkSmartPointer<vtkSphereSource> sphereSource =
        vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->Update();
    vtkSmartPointer<vtkPolyDataMapper> sphereMapper =
        vtkSmartPointer<vtkPolyDataMapper>::New();
    sphereMapper->SetInputConnection(sphereSource->GetOutputPort());
    vtkSmartPointer<vtkActor> sphereActor =
        vtkSmartPointer<vtkActor>::New();
    sphereActor->SetMapper(sphereMapper);

    // VTK Renderer
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(sphereActor);
    QVTKWidget *widget = new QVTKWidget();

//    QMdiSubWindow *subWindow1 = new QMdiSubWindow;
//    subWindow1->setWidget(widget);
//    subWindow1->setAttribute(Qt::WA_DeleteOnClose);
//    subWindow1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QMdiSubWindow *sub_win = ui->mdiArea->addSubWindow(widget);
    connect(sub_win, SIGNAL(aboutToActivate()), this, SLOT(viewWillBeSelected()));

    widget->setParent(sub_win);
    widget->setWindowTitle(mafTr("mafView %1").arg(windowCounter++));
    widget->GetRenderWindow()->AddRenderer(renderer);
    sub_win->setMinimumSize(200, 200);
    sub_win->setGeometry(QRect(0, 0, 0, 0));

    QPropertyAnimation *animation = new QPropertyAnimation(sub_win, "geometry");
    animation->setDuration(500);
    animation->setStartValue(QRect(0, 0, 0, 0));
    animation->setEndValue(QRect(50, 50, 200, 200));
    animation->setEasingCurve(QEasingCurve::InOutSine);

    sub_win->show();
    animation->start();
}

void mafMainWindow::viewWillBeSelected() {
    mafMsgDebug() << "View will be selected!!";
}

void mafMainWindow::viewSelected(QMdiSubWindow *sub_win) {
    mafMsgDebug() << "View selected!!";
}
