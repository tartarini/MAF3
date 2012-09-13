/*
 *  mafMainWindow.cpp
 *  SimpleApp
 *
 *  Created by Paolo Quadrani on 14/12/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafMainWindow.h"
#include "ui_mafMainWindow.h"

#include <mafEventBusManager.h>

#include "mafGUIApplicationSettingsDialog.h"
#include <QToolButton>

#include <fvupdater.h>

using namespace mafCore;
using namespace mafGUI;

mafMainWindow::mafMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mafMainWindow), m_Logic(NULL) {
    initializeMainWindow();
}

mafMainWindow::mafMainWindow(mafApplicationLogic::mafLogic *logic, QWidget *parent) : QMainWindow(parent), m_GUIManager(NULL), ui(new Ui::mafMainWindow), m_Logic(logic) {
    initializeMainWindow();
//     m_Logic->loadPlugins();
}

mafGUI::mafGUIManager *mafMainWindow::guiManager() {
    if (m_GUIManager == NULL) {
        mafGUIRegistration::registerGUIObjects();
        m_GUIManager = new mafGUIManager(this, mafCodeLocation);
    }
    return m_GUIManager;
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
    updateApplicationName();

    guiManager()->setLogic(m_Logic);
    m_GUIManager->createMenus();

    connect(m_GUIManager, SIGNAL(guiLoaded(int,QWidget*)), this, SLOT(loadedGUIAvailable(int,QWidget*)));
    connect(m_GUIManager, SIGNAL(guiTypeToRemove(int)), this, SLOT(loadedGUIToRemove(int)));
    connect(m_GUIManager, SIGNAL(updateApplicationName()), this, SLOT(updateApplicationName()));

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
    ui->vmeTab->setLayout(ui->vmeLayout);
    ui->visualPipeTab->setLayout(ui->visualPipeLayout);
    ui->dataPipeTab->setLayout(ui->dataPipeLayout);

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
    splitter->addWidget(ui->tabProperty);
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
    ui->logBarWidgetContents->setLayout(ui->verticalLogLayout);

    QObject *logBarAction = m_GUIManager->menuItemByName("LogBar");
    m_LogWidget = m_GUIManager->createLogWidget(ui->logBarWidgetContents);
    connect(ui->dockLogBarWidget, SIGNAL(visibilityChanged(bool)), logBarAction, SLOT(setChecked(bool)));
    connect(logBarAction, SIGNAL(triggered(bool)), ui->dockLogBarWidget, SLOT(setVisible(bool)));

    //Plug find widget.
    m_FindWidget = new mafFindWidget();
    ui->horizontalLogLayout->addWidget(m_FindWidget);
    bool caseChecked = m_FindWidget->m_ActionCase->isChecked();
    bool wholeChecked = m_FindWidget->m_ActionWhole->isChecked();
    connect(m_FindWidget->m_FindLineEdit, SIGNAL(textEdited(QString)), m_LogWidget, SLOT(find(QString)));
    connect(m_FindWidget->m_ActionCase, SIGNAL(triggered(bool)), m_LogWidget, SLOT(setMatchCase(bool)));
    connect(m_FindWidget->m_ActionWhole, SIGNAL(triggered(bool)), m_LogWidget, SLOT(setMatchWhole(bool)));

    //Add clear button
    QToolButton *clearButton = new QToolButton;
    clearButton->setText("Clear");
    ui->horizontalLogLayout->addWidget(clearButton);
    connect(clearButton, SIGNAL(pressed()), m_LogWidget, SLOT(clear()));

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(subWindowSelected(QMdiSubWindow*)));

    ui->mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    
    setUnifiedTitleAndToolBarOnMac(true);

    // Restore settings notification should be sent at the end of the initialization code, so to be sure that
    // each GUI element has been created.
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.logic.settings.restore");
}

void mafMainWindow::connectCallbacks() {
    mafRegisterLocalCallback("maf.local.gui.action.save", this, "save()");
    mafRegisterLocalCallback("maf.local.gui.action.about", this, "showAbout()");
    mafRegisterLocalCallback("maf.local.gui.action.update", this, "updateApplicationShowDialog()");

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

void mafMainWindow::updateApplicationShowDialog() {
    QMessageBox msgBox;
    QPushButton *updateButton = msgBox.addButton(mafTr("Check Update"), QMessageBox::ActionRole);
    // ui->updateButton is QButton
    QObject::connect(updateButton, SIGNAL(clicked()),
        FvUpdater::sharedUpdater(), SLOT(CheckForUpdatesNotSilent()));
    
    msgBox.exec();
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
                ui->layoutView->addWidget(w);
                break;
            case mafGUILoadedTypeVisualPipe:
                ui->visualPipeLayout->addWidget(w);
                break;
            case mafGUILoadedTypeVme:
                ui->vmeLayout->addWidget(w);
                break;
        }
        w->show();
    }
}

void mafMainWindow::loadedGUIToRemove(int type) {
    switch(type) {
        case mafGUILoadedTypeOperation:
            ui->tabWidget->setCurrentIndex(0);
            break;
        case mafGUILoadedTypeView:
            break;
        case mafGUILoadedTypeVisualPipe:
            break;
        case mafGUILoadedTypeVme:
            break;
    }
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

void mafMainWindow::viewCreated(mafCore::mafObjectBase *view) {
    QObject *widgetObj = view->property("renderWidget").value<QObject*>();
    QWidget *widget = qobject_cast<QWidget*>(widgetObj);

    if (widget == NULL) {
        return;
    }
    QMdiSubWindow *sub_win = ui->mdiArea->addSubWindow(widget);
    sub_win->setWindowTitle(view->objectName());
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
        QByteArray ba = mafTr("View doesn't exist!!").toAscii();
        qCritical(ba.constData());
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
            QByteArray ba = mafTr("View doesn't exist!!").toAscii();
            qCritical(ba.constData());
            return;
        }
        argList.append(mafEventArgument(mafCore::mafObjectBase *, view));
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.select", mafEventBus::mafEventTypeLocal, &argList);
    }
}

void mafMainWindow::updateApplicationName(){
    QString applicationName(QApplication::applicationName());
    QString sessionFileName = guiManager()->fileName();
    if(!sessionFileName.isEmpty()) {
        applicationName.append(" - ");
        applicationName.append(sessionFileName);
    }

    setWindowTitle(applicationName);
}

