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
#include <mafGUIApplicationSettingsDialog.h>
#include <QDebug>

using namespace mafCore;
using namespace mafGUI;

mafMainWindow::mafMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mafMainWindow), m_Logic(NULL) {
    initializeMainWindow();
}

mafMainWindow::mafMainWindow(mafApplicationLogic::mafLogic *logic, QWidget *parent) :QMainWindow(parent), ui(new Ui::mafMainWindow), m_Logic(logic) {
    initializeMainWindow();
    m_Logic->loadPlugins();
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
    m_GUIManager->setLogic(m_Logic);

    m_GUIManager->createMenus();

    connect(m_GUIManager, SIGNAL(guiLoaded(int,QWidget*)), this, SLOT(loadedGUIAvailable(int,QWidget*)));
    connect(m_GUIManager, SIGNAL(guiTypeToRemove(int)), this, SLOT(loadedGUIToRemove(int)));

    connectCallbacks();

    ui->statusBar->showMessage(mafTr("Ready!"));

    // Connecting layouts (needed because from QtDesign is not managed automatically)
    ui->centralWidget->setLayout(ui->gridLayout);
	// SideBar Layout
    ui->sideBarDockContents->setLayout(ui->layoutSideBar);
    // View's tab
    ui->tabView->setLayout(ui->layoutView);
    // Operation's tab
    ui->tabOperation->setLayout(ui->layoutOperation);
    // Hierarchy tree's tab
    ui->tabTree->setLayout(ui->layoutTree);
    ui->hierarchyWidget->setLayout(ui->layoutHierarchy);
    ui->propertiesBoxContainer->setLayout(ui->layoutPropertiesBox);

    //tree widget in sidebar
     m_Model = new mafDragDropModel();

    if(m_Logic) {
        m_Model->setHierarchy(m_Logic->hierarchy());
     }

    // **** SideBar ****
    m_Tree = m_GUIManager->createTreeWidget(m_Model, ui->hierarchyWidget);
    m_Tree->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_Tree->setSelectionMode(QAbstractItemView::SingleSelection);
    //allow drag&drop operation
    m_Tree->setDragEnabled(true);
    m_Tree->setAcceptDrops(true);
    m_Tree->setDropIndicatorShown(true);
    QModelIndex index = m_Model->index(0, 0);
    m_Tree->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);

    QSplitter *splitter = new QSplitter(Qt::Vertical);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->addWidget(ui->hierarchyWidget);
    splitter->addWidget(ui->propertiesBoxContainer);
    ui->layoutTree->addWidget(splitter);
    splitter->setStretchFactor(1, 1);

    // SideBar visibility management
    QObject *sideBarAction = m_GUIManager->menuItemByName("SideBar");
    connect(ui->dockSideBar, SIGNAL(visibilityChanged(bool)), sideBarAction, SLOT(setChecked(bool)));
    connect(sideBarAction, SIGNAL(triggered(bool)), ui->dockSideBar, SLOT(setVisible(bool)));
    connect(m_Tree->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
            m_Model, SLOT(selectItem(const QItemSelection &, const QItemSelection &)));
    // **** LogBar ****
    // LogBar Layout
    ui->logBarWidgetContents->setLayout(ui->gridLayoutLogBar);

    QObject *logBarAction = m_GUIManager->menuItemByName("LogBar");
    m_LogWidget = m_GUIManager->createLogWidget(ui->logBarWidgetContents);
    connect(ui->dockLogBarWidget, SIGNAL(visibilityChanged(bool)), logBarAction, SLOT(setChecked(bool)));
    connect(logBarAction, SIGNAL(triggered(bool)), ui->dockLogBarWidget, SLOT(setVisible(bool)));

    // **** Google chat ****
    QObject *collaborateAction = m_GUIManager->menuItemByName("Collaborate");
    m_DockGoogleChat = new QDockWidget(tr("Google Chat"));
    m_DockGoogleChat->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_DockGoogleChat->setWidget(new GoogleChat());
    if(collaborateAction) {
        connect(collaborateAction, SIGNAL(triggered(bool)), this, SLOT(updateCollaborationDockVisibility(bool)));
        connect(m_DockGoogleChat, SIGNAL(visibilityChanged(bool)), collaborateAction, SLOT(setChecked(bool)));
    }

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowSelected(QMdiSubWindow*)));

    setUnifiedTitleAndToolBarOnMac(true);

    // Restore settings notification should be sent at the end of the initialization code, so to be sure that
    // each GUI element has been created.
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.logic.settings.restore");
}

void mafMainWindow::connectCallbacks() {
    mafRegisterLocalCallback("maf.local.gui.action.save", this, "save()");
    mafRegisterLocalCallback("maf.local.gui.action.about", this, "showAbout()");

    mafRegisterLocalCallback("maf.local.logic.settings.store", this, "writeSettings()");
    mafRegisterLocalCallback("maf.local.logic.settings.restore", this, "readSettings()");

    mafRegisterLocalCallback("maf.local.resources.view.created", this, "viewCreated(mafCore::mafObjectBase *)");
}

mafMainWindow::~mafMainWindow() {
    mafDEL(m_GUIManager);
    delete ui;
}

void mafMainWindow::setupMainWindow() {
    m_GUIManager->settingsDialog()->setupSettingsDialog();
    this->show();
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

void mafMainWindow::loadedGUIAvailable(int type, QWidget *w) {
    if (w) {
        switch(type) {
            case mafGUILoadedTypeOperation:
                ui->tabWidget->setCurrentIndex(2);
                ui->layoutOperation->addWidget(w);
                break;
            case mafGUILoadedTypeView:
                ui->tabWidget->setCurrentIndex(1);
                ui->layoutView->addWidget(w);
                break;
            case mafGUILoadedTypeVme:
                ui->layoutPropertiesBox->addWidget(w);
                break;
        }
        w->show();
    }
}

void mafMainWindow::loadedGUIToRemove(int type) {
    ui->tabWidget->setCurrentIndex(0);
}

void mafMainWindow::readSettings() {
    qDebug() << "Reading mafMainWindows settings...";
    QSettings settings;
    
    // restoring MainWindow
    QPoint pos = settings.value("MainWindow/Position", QPoint(200, 200)).toPoint();
    QSize size = settings.value("MainWindow/Size", QSize(600, 400)).toSize();
    resize(size);
    move(pos);
    
    // Restoring SideBar
    ui->tabWidget->setCurrentIndex(settings.value("SideBar/ActiveTab", 0).toInt());
    int docPos = settings.value("SideBar/DockPosition", Qt::RightDockWidgetArea).toInt();
    this->addDockWidget((Qt::DockWidgetArea)docPos, ui->dockSideBar);
    ui->dockSideBar->setFloating(settings.value("SideBar/isFloating", false).toBool());
    ui->dockSideBar->setVisible(settings.value("SideBar/isVisible", true).toBool());
//    QRect rectSidebar = settings.value("SideBar/Geometry", QRect(0, 0, 200, 400)).toRect();
//    ui->dockSideBar->resize(rectSidebar.size());

    // Restoring LogBar
    docPos = settings.value("LogBar/DockPosition", Qt::BottomDockWidgetArea).toInt();
    this->addDockWidget((Qt::DockWidgetArea)docPos, ui->dockLogBarWidget);
    ui->dockLogBarWidget->setFloating(settings.value("LogBar/isFloating", false).toBool());
    ui->dockLogBarWidget->setVisible(settings.value("LogBar/isVisible", true).toBool());
//    QRect rectLog = settings.value("LogBar/Geometry", QRect(0, 0, 800, 100)).toRect();
//    ui->dockLogBarWidget->resize(rectLog.size());
}

void mafMainWindow::writeSettings() {
    qDebug() << "Writing mafMainWindows settings...";
    QSettings settings;
    settings.setValue("MainWindow/Position", pos());
    settings.setValue("MainWindow/Size", size());

    // Save the SideBar settings
    settings.setValue("SideBar/Geometry", ui->dockSideBar->geometry());
    settings.setValue("SideBar/ActiveTab", ui->tabWidget->currentIndex());
    settings.setValue("SideBar/DockPosition", this->dockWidgetArea(ui->dockSideBar));
    settings.setValue("SideBar/isFloating", ui->dockSideBar->isFloating());
    settings.setValue("SideBar/isVisible", ui->dockSideBar->isVisible());

    // Save the LogBar settings
    settings.setValue("LogBar/Geometry", ui->dockLogBarWidget->geometry());
    settings.setValue("LogBar/DockPosition", this->dockWidgetArea(ui->dockLogBarWidget));
    settings.setValue("LogBar/isFloating", ui->dockLogBarWidget->isFloating());
    settings.setValue("LogBar/isVisible", ui->dockLogBarWidget->isVisible());

    settings.sync();
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

void mafMainWindow::plugApplicationSettingsPage(mafGUIApplicationSettingsPage *page) {
    m_GUIManager->settingsDialog()->addPage(page);
}

void mafMainWindow::updateCollaborationDockVisibility(bool visible) {
    if(m_DockGoogleChat->parent() == NULL) {
        this->addDockWidget(Qt::LeftDockWidgetArea, m_DockGoogleChat);
    }
    m_DockGoogleChat->setVisible(visible);
}

void mafMainWindow::viewCreated(mafCore::mafObjectBase *view) {
    QObject *widgetObj = view->property("renderWidget").value<QObject*>();
    QWidget *widget = qobject_cast<QWidget*>(widgetObj);

    QMdiSubWindow *sub_win = ui->mdiArea->addSubWindow(widget);
    sub_win->setAttribute(Qt::WA_DeleteOnClose);
    sub_win->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(sub_win, SIGNAL(aboutToActivate()), this, SLOT(viewWillBeSelected()));
    connect(sub_win, SIGNAL(destroyed()), this, SLOT(subWindowDestroyed()));
    connect(sub_win, SIGNAL(destroyed()), view, SLOT(deleteLater()));

    m_ViewSubWindowHash.insert(sub_win, view);
    
    widget->setParent(sub_win);
    sub_win->setMinimumSize(200, 200);
    sub_win->resize(QSize(400, 400));
    sub_win->show();
}

void mafMainWindow::subWindowDestroyed() {
    QMdiSubWindow *subWindow = qobject_cast<QMdiSubWindow *>(QObject::sender());
    mafEventBus::mafEventArgumentsList argList;
    mafObjectBase *view = m_ViewSubWindowHash.value(subWindow);
    if(view == NULL) {
        qCritical(mafTr("View doesn't exist!!").toAscii().constData());
        return;
    }
    argList.append(mafEventArgument(mafCore::mafObjectBase *, view));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.destroy", mafEventBus::mafEventTypeLocal, &argList);
    
}


void mafMainWindow::viewWillBeSelected() {
    qDebug() << "View will be selected!!";
}

void mafMainWindow::subWindowSelected(QMdiSubWindow *sub_win) {
    if(sub_win != NULL) {
        mafEventBus::mafEventArgumentsList argList;
        mafObjectBase *view = m_ViewSubWindowHash.value(sub_win);
        if(view == NULL) {
            qCritical(mafTr("View doesn't exist!!").toAscii().constData());
            return;
        }
        argList.append(mafEventArgument(mafCore::mafObjectBase *, view));
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.select", mafEventBus::mafEventTypeLocal, &argList);
    }
}
