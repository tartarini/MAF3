/*
 *  mafGUIManager.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2010 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUIManager.h"
#include "mafUILoaderQt.h"
#include "mafTreeWidget.h"
#include "mafTreeModel.h"
#include "mafTextEditWidget.h"
#include "mafTextHighlighter.h"

#include "mafGUIApplicationSettingsDialog.h"

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

mafGUIManager::mafGUIManager(QMainWindow *main_win, const mafString code_location) : mafObjectBase(code_location)
    , m_NewAct(NULL), m_CollaborateAct(NULL)
    , m_OpenAct(NULL), m_SaveAct(NULL), m_SaveAsAct(NULL), m_RecentFilesSeparatorAct(NULL), m_ExitAct(NULL)
    , m_CutAct(NULL), m_CopyAct(NULL), m_PasteAct(NULL), m_AboutAct(NULL)
    , m_MaxRecentFiles(5), m_ActionsCreated(false), m_MainWindow(main_win) {

    m_SettingsDialog = new mafGUIApplicationSettingsDialog();

    mafRegisterLocalCallback("maf.local.resources.plugin.registerLibrary", this, "fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash)");
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "updateMenuForSelectedVme(mafCore::mafObjectBase *)");

    m_UILoader = mafNEW(mafGUI::mafUILoaderQt);
    connect(m_UILoader, SIGNAL(uiLoadedSignal(mafCore::mafContainerInterface*)), this, SLOT(uiLoaded(mafCore::mafContainerInterface*)));
}

mafGUIManager::~mafGUIManager() {
    mafDEL(m_UILoader);
}

void mafGUIManager::createActions() {
    m_NewAct = new QAction(QIcon(":/images/new.png"), mafTr("&New"), this);
    m_NewAct->setIconText(mafTr("New"));
    m_NewAct->setObjectName("New");
    m_NewAct->setShortcuts(QKeySequence::New);
    m_NewAct->setStatusTip(mafTr("Create a new file"));
    m_ActionList.append(m_NewAct);

    m_CollaborateAct = new QAction(QIcon(":/images/collaborate.png"), mafTr("Collaborate"), this);
    m_CollaborateAct->setObjectName("Collaborate");
    m_CollaborateAct->setCheckable(true);
    m_CollaborateAct->setChecked(false);
    m_CollaborateAct->setIconText(mafTr("Collaborate"));
    m_CollaborateAct->setStatusTip(mafTr("Collaborate with your conmtacts in gtalk."));
    m_ActionList.append(m_CollaborateAct);

    m_OpenAct = new QAction(QIcon(":/images/open.png"), mafTr("&Open..."), this);
    m_OpenAct->setObjectName("Open");
    m_OpenAct->setIconText(mafTr("Open"));
    m_OpenAct->setShortcuts(QKeySequence::Open);
    m_OpenAct->setStatusTip(mafTr("Open an existing file"));
    m_ActionList.append(m_OpenAct);

    m_SaveAct = new QAction(QIcon(":/images/save.png"), mafTr("&Save"), this);
    m_SaveAct->setObjectName("Save");
    m_SaveAct->setIconText(mafTr("Save"));
    m_SaveAct->setShortcuts(QKeySequence::Save);
    m_SaveAct->setStatusTip(mafTr("Save the document to disk"));
    m_ActionList.append(m_SaveAct);

    m_SaveAsAct = new QAction(mafTr("Save &As..."), this);
    m_SaveAsAct->setObjectName("SaveAs");
    m_SaveAsAct->setIconText(mafTr("Save As"));
    m_SaveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_SaveAsAct->setStatusTip(mafTr("Save the document under a new name"));
    m_ActionList.append(m_SaveAsAct);

    m_RecentFileActs.clear();
    for (int i = 0; i < m_MaxRecentFiles; ++i) {
        QAction *recentFileAction = new QAction(this);
        m_ActionList.append(recentFileAction);
        recentFileAction->setVisible(false);
        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_RecentFileActs.append(recentFileAction);
    }

    m_ExitAct = new QAction(mafTr("E&xit"), this);
    m_ExitAct->setObjectName("Exit");
    m_ExitAct->setIconText(mafTr("Exit"));
    m_ExitAct->setShortcuts(QKeySequence::Quit);
    m_ExitAct->setStatusTip(mafTr("Exit the application"));
    m_ActionList.append(m_ExitAct);
    connect(m_ExitAct, SIGNAL(triggered()), m_MainWindow, SLOT(close()));

    m_CutAct = new QAction(QIcon(":/images/cut.png"), mafTr("Cu&t"), this);
    m_CutAct->setObjectName("Cut");
    m_CutAct->setIconText(mafTr("Cut"));
    m_CutAct->setShortcuts(QKeySequence::Cut);
    m_CutAct->setStatusTip(mafTr("Cut the current selection's contents to the "
                            "clipboard"));
    m_ActionList.append(m_CutAct);

    m_CopyAct = new QAction(QIcon(":/images/copy.png"), mafTr("&Copy"), this);
    m_CopyAct->setObjectName("Copy");
    m_CopyAct->setIconText(mafTr("Copy"));
    m_CopyAct->setShortcuts(QKeySequence::Copy);
    m_CopyAct->setStatusTip(mafTr("Copy the current selection's contents to the "
                             "clipboard"));
    m_ActionList.append(m_CopyAct);

    m_PasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    m_PasteAct->setObjectName("Paste");
    m_PasteAct->setIconText(mafTr("Paste"));
    m_PasteAct->setShortcuts(QKeySequence::Paste);
    m_PasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    m_ActionList.append(m_PasteAct);

    m_AboutAct = new QAction(tr("&About"), this);
    m_AboutAct->setObjectName("About");
    m_AboutAct->setIconText(mafTr("About"));
    m_AboutAct->setStatusTip(tr("Show the application's About box"));
    m_ActionList.append(m_AboutAct);

    m_SettingsAction = new QAction(mafTr("Settings"), this);
    m_SettingsAction->setObjectName("Settings");
    m_SettingsAction->setIconText(mafTr("Settings"));
    m_SettingsAction->setStatusTip(tr("Show the application's Settings dialog"));
    m_ActionList.append(m_SettingsAction);
    connect(m_SettingsAction, SIGNAL(triggered()), SLOT(showSettingsDialog()));

    m_SideBarAct = new QAction(tr("Sidebar"), this);
    m_SideBarAct->setObjectName("SideBar");
    m_SideBarAct->setCheckable(true);
    m_SideBarAct->setChecked(true);
    m_ActionList.append(m_SideBarAct);

    m_LogBarAct = new QAction(tr("LogBar"), this);
    m_LogBarAct->setObjectName("LogBar");
    m_LogBarAct->setCheckable(true);
    m_LogBarAct->setChecked(true);
    m_ActionList.append(m_LogBarAct);

    m_ActionsCreated = true;

    registerEvents();
}

void mafGUIManager::showSettingsDialog() {
    m_SettingsDialog->show();
}

void mafGUIManager::fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash pluginHash) {
    mafCore::mafObjectBase *sel_vme;
    mafGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

    if(!m_ActionsCreated) {
        // Actions has not been created, so neither the menu.
        // Ask to create it which will crete also the actions.
        createMenus();
    }
    mafString base_class("");
    mafPluggedObjectInformation objInfo;
    mafPluggedObjectsHash::iterator iter = pluginHash.begin();
    while(iter != pluginHash.end()) {
        objInfo = iter.value();
        base_class = iter.key();
        if(base_class == "mafResources::mafOperation" || base_class == "mafResources::mafView") {
            QAction *action = new QAction(mafTr(objInfo.m_Label.toAscii()), NULL);
            QVariant data_type(objInfo.m_ClassType);
            action->setData(data_type);
            if(base_class == "mafResources::mafOperation") {
                // Add a new item to the operation's menu.
                m_OpMenu->addAction(action);
                connect(action, SIGNAL(triggered()), this, SLOT(startOperation()));
            } else if(base_class == "mafResources::mafView") {
                // Add a new item to the view's menu.
                m_ViewMenu->addAction(action);
                connect(action, SIGNAL(triggered()), this, SLOT(createView()));
            }
        }
        iter++;
    }

    this->updateMenuForSelectedVme(sel_vme);
}

QObject *mafGUIManager::actionByName(mafString name) {
    if(!m_ActionsCreated || name.length() == 0) {
        return NULL;
    }
    for (int i = 0; i < m_ActionList.size(); ++i) {
        QObject *action = m_ActionList.at(i);
        mafString an = action->objectName();
        if (an == name) {
            return action;
        }
    }
    return NULL;
}

void mafGUIManager::updateMenuForSelectedVme(mafCore::mafObjectBase *vme) {
    mafStringList accepted_list;
    accepted_list = mafCoreRegistration::acceptObject(vme);

    mafList<QAction *> opActions = m_OpMenu->actions();
    mafString op;
    foreach(QAction *action, opActions) {
        op = action->data().toString();
        action->setEnabled(accepted_list.contains(op));
    }
}

void mafGUIManager::registerEvents() {
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.gui.action.new");
    provider->createNewId("maf.local.gui.action.open");
    provider->createNewId("maf.local.gui.action.save");
    provider->createNewId("maf.local.gui.action.saveas");
    provider->createNewId("maf.local.gui.action.cut");
    provider->createNewId("maf.local.gui.action.copy");
    provider->createNewId("maf.local.gui.action.paste");
    provider->createNewId("maf.local.gui.action.about");
    provider->createNewId("maf.local.gui.pathSelected");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.gui.action.new", m_NewAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.open", m_OpenAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.save", m_SaveAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.saveas", m_SaveAsAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.cut", m_CutAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.copy", m_CopyAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.paste", m_PasteAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.about", m_AboutAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.pathSelected", this, "pathSelected(const mafString)");

    // OperationManager's callback
    mafRegisterLocalCallback("maf.local.resources.operation.started", this, "operationDidStart(const mafCore::mafObjectBase *)");
}

void mafGUIManager::createMenus() {
    if(!m_ActionsCreated) {
        createActions();
    }

    QMenuBar *menuBar = m_MainWindow->menuBar();

    m_FileMenu = menuBar->addMenu(tr("&File"));
    m_FileMenu->addAction(m_NewAct);
    m_FileMenu->addAction(m_CollaborateAct);
    m_FileMenu->addAction(m_OpenAct);
    m_FileMenu->addAction(m_SaveAct);
    m_FileMenu->addAction(m_SaveAsAct);

    m_RecentFilesSeparatorAct = m_FileMenu->addSeparator();
    for (int i = 0; i < m_MaxRecentFiles; ++i) {
        m_FileMenu->addAction(m_RecentFileActs.at(i));
    }

    m_FileMenu->addSeparator();
    m_FileMenu->addAction(m_ExitAct);

    menuBar->addSeparator();

    m_EditMenu = menuBar->addMenu(tr("&Edit"));
    m_EditMenu->addAction(m_CutAct);
    m_EditMenu->addAction(m_CopyAct);
    m_EditMenu->addAction(m_PasteAct);

    menuBar->addSeparator();

    m_ViewMenu = menuBar->addMenu(tr("Views"));

    menuBar->addSeparator();

    m_OpMenu = menuBar->addMenu(tr("Operations"));

    menuBar->addSeparator();

    m_WindowMenu = menuBar->addMenu(tr("&Window"));
    m_WindowMenu->addAction(m_SideBarAct);
    m_WindowMenu->addAction(m_LogBarAct);

    menuBar->addSeparator();

    m_HelpMenu = menuBar->addMenu(tr("&Help"));
    m_HelpMenu->addAction(m_AboutAct);

    m_OptionsMenu = menuBar->addMenu(tr("Options"));
    m_OptionsMenu->addAction(m_SettingsAction);
}

void mafGUIManager::createToolBars() {
    if(!m_ActionsCreated) {
        createActions();
    }
    m_FileToolBar = m_MainWindow->addToolBar(tr("File"));
    m_FileToolBar->addAction(m_NewAct);
    m_FileToolBar->addAction(m_CollaborateAct);
    m_FileToolBar->addAction(m_OpenAct);
    m_FileToolBar->addAction(m_SaveAct);
    m_FileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_EditToolBar = m_MainWindow->addToolBar(tr("Edit"));
    m_EditToolBar->addAction(m_CutAct);
    m_EditToolBar->addAction(m_CopyAct);
    m_EditToolBar->addAction(m_PasteAct);
    m_EditToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void mafGUIManager::startOperation() {
    QAction *opAction = (QAction *)QObject::sender();
    mafString op(opAction->data().toString());
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, op));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
}

mafTreeWidget *mafGUIManager::createTreeWidget(mafTreeModel *model, QWidget *parent) {
//    mafSettings settings;
    mafTreeWidget *w = new mafTreeWidget();
//    w->setGeometry(settings.value("SideBar/Geometry", QRect(0, 0, 200, 400)).toRect());
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    w->setMinimumSize(200, 200);
    w->setMaximumSize(16777215, 16777215);

    if(parent) {
        if(parent->layout()) {
            parent->layout()->addWidget(w);
        } else {
            w->setParent(parent);
        }
    }

    w->setModel( model );
    return w;
}

mafTextEditWidget *mafGUIManager::createLogWidget(QWidget *parent) {
    //syntax highlighter
    mafTextHighlighter *hl = new mafTextHighlighter();
    mafTextEditWidget *w = new mafTextEditWidget(hl, parent);
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    if(parent) {
        if(parent->layout()) {
            parent->layout()->addWidget(w);
        } else {
            w->setParent(parent);
        }
    }

    return w;
}

void mafGUIManager::operationDidStart(const mafCore::mafObjectBase *operation) {
    // Get the started operation
    mafString guiFilename = operation->uiFilename();
    // Set the current panel to the parent panel of operations.
    // m_CurrentPanel = m_OperationPanel;
    // Ask the UI Loader to load the operation's GUI.
    m_UILoader->uiLoad(guiFilename);
}

void mafGUIManager::uiLoaded(mafCore::mafContainerInterface *guiWidget) {
    // Get the widget from the container
    mafContainer<QWidget> *w = mafContainerPointerTypeCast(QWidget, guiWidget);
    QWidget *widget = *w; // TODO: finire il plug della GUI caricata dinamicamente nel pannello corrente delle operazioni.
    // put the widget on the interface.
    //widget->setParent(m_CurrentPanel);
}

void mafGUIManager::createView() {
    QAction *viewAction = (QAction *)QObject::sender();
    mafString view(viewAction->data().toString());
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafString, view));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList);
}

void mafGUIManager::chooseFileDialog(const mafString title, const mafString start_dir, const mafString wildcard) {
    mafString fileName = QFileDialog::getOpenFileName(m_MainWindow, title, start_dir, wildcard);
}

void mafGUIManager::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        mafString file_to_open(action->data().toString());
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafString, file_to_open));
        mafEventBusManager::instance()->notifyEvent("maf.local.logic.openFile", mafEventTypeLocal, &argList);
    }
}

void mafGUIManager::updateRecentFileActions() {
    mafSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)m_MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        m_RecentFileActs.at(i)->setText(text);
        m_RecentFileActs.at(i)->setData(files[i]);
        m_RecentFileActs.at(i)->setVisible(true);
    }
    for (int j = numRecentFiles;  j < m_MaxRecentFiles; ++j)
        m_RecentFileActs.at(j)->setVisible(false);

    m_RecentFilesSeparatorAct->setVisible(numRecentFiles > 0);
}

mafString mafGUIManager::strippedName(const mafString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}
