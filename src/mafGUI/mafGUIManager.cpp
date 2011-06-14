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
#include "mafLoggerWidget.h"
#include "mafTextEditWidget.h"
#include "mafGUIApplicationSettingsDialog.h"
#include "mafTreeItemDelegate.h"
#include "mafTreeItemSceneNodeDelegate.h"
#include <mafView.h>

#include "mafOperationWidget.h"

#include <mafObjectBase.h>

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

mafGUIManager::mafGUIManager(QMainWindow *main_win, const QString code_location) : mafObjectBase(code_location)
    , m_VMEWidget(NULL), m_ViewWidget(NULL), m_MaxRecentFiles(5), m_MainWindow(main_win)
    , m_Model(NULL), m_TreeWidget(NULL), m_Logic(NULL), m_CompleteFileName(), m_LastPath() {

    m_SettingsDialog = new mafGUIApplicationSettingsDialog();
    m_OperationWidget = new mafOperationWidget();
    connect(m_OperationWidget, SIGNAL(operationDismissed()), this, SLOT(removeOperationGUI()));

    m_Logger = new mafLoggerWidget(mafCodeLocation);

    mafCore::mafMessageHandler::instance()->setActiveLogger(m_Logger);

    mafRegisterLocalSignal("maf.local.gui.new", this, "newWorkinSessioneSignal()");
    mafRegisterLocalCallback("maf.local.gui.new", this, "newWorkingSession()");

    mafRegisterLocalCallback("maf.local.resources.plugin.registerLibrary", this, "fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash)")
    
    // VME selection callbacks.
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "updateMenuForSelectedVme(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "updateTreeForSelectedVme(mafCore::mafObjectBase *)")

    // OperationManager's callback
    mafRegisterLocalCallback("maf.local.resources.operation.started", this, "operationDidStart(mafCore::mafObjectBase *)");
    
    // ViewManager's callback.
    mafRegisterLocalCallback("maf.local.resources.view.select", this, "viewSelected(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.view.noneViews", this, "viewDestroyed()");


    m_UILoader = mafNEW(mafGUI::mafUILoaderQt);
    connect(m_UILoader, SIGNAL(uiLoadedSignal(mafCore::mafProxyInterface*)), this, SLOT(showGui(mafCore::mafProxyInterface*)));
}

mafGUIManager::~mafGUIManager() {
    mafDEL(m_Logger);
    mafDEL(m_UILoader);
}

void mafGUIManager::newWorkingSession() {
    m_Model->setTreeModelStatus(mafTreeModelStatusGenerate);
    mafHierarchyPointer h = m_Logic->requestNewHierarchy();

    /// view select
    mafCore::mafObjectBase *sel_view;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_view);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &ret_val);

    QVariant v;
    if (sel_view) {
        v = sel_view->property("hierarchy");
        h = v.value<mafCore::mafHierarchyPointer>();
    }
    
    m_Model->setHierarchy(h);
        
    QModelIndex index = m_Model->index(0, 0);
    m_TreeWidget->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    m_CompleteFileName = "";
    emit updateApplicationName();
}

void mafGUIManager::quitApplication() {
    // post the closeEvent
    QApplication::postEvent(qApp, new QCloseEvent());
}

void mafGUIManager::createToolbar(QDomElement node) {
    QDomNamedNodeMap attributes = node.attributes();
    QString title = attributes.namedItem("title").nodeValue();
    QString actions = attributes.namedItem("actionList").nodeValue();

    QToolBar *toolBar = m_MainWindow->addToolBar(tr(title.toAscii().constData()));

    QStringList actionList = actions.split(",");
    foreach (QString action, actionList) {
        toolBar->addAction((QAction*)menuItemByName(action));
    }

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void mafGUIManager::createAction(QDomElement node) {
    QDomNamedNodeMap attributes = node.attributes();
    QString title = attributes.namedItem("title").nodeValue();
    QString icon = attributes.namedItem("icon").nodeValue();
    QString tip = attributes.namedItem("tip").nodeValue();
    QString checkable = attributes.namedItem("checkable").nodeValue();
    QString checked = attributes.namedItem("checked").nodeValue();
    QString slot = attributes.namedItem("slot").nodeValue();
    QString topic = attributes.namedItem("topic").nodeValue();

    QAction *action = new QAction(QIcon(icon), mafTr(title.toAscii().constData()), this);
    action->setIconText(mafTr(title.toAscii().constData()));
    action->setObjectName(title);
//    action->setShortcuts(QKeySequence::New);
    action->setStatusTip(mafTr(tip.toAscii().constData()));
    action->setCheckable(checkable.toInt() != 0);
    action->setChecked(checked.toInt() != 0);
    if (!slot.isEmpty()) {
        slot.prepend(CALLBACK_SIGNATURE);
        connect(action, SIGNAL(triggered()), this, slot.toAscii().constData());
    }
    if (!topic.isEmpty()) {
        mafIdProvider *provider = mafIdProvider::instance();
        provider->createNewId(topic);
        mafRegisterLocalSignal(topic, action, "triggered()");
    }
    
    if (m_CurrentMenu != NULL) {
        m_CurrentMenu->addAction(action);
    }
    
    m_MenuItemList.append(action);
}

void mafGUIManager::createMenuItem(QDomElement node) {
    QDomNamedNodeMap attributes = node.attributes();
    QString title = attributes.namedItem("title").nodeValue();
    QString parentName = attributes.namedItem("parent").nodeValue();

    if (parentName.isEmpty()) {
        // Create a new menu item.
        QMenuBar *menuBar = m_MainWindow->menuBar();
        m_CurrentMenu = menuBar->addMenu(mafTr(title.toAscii().constData()));
        m_CurrentMenu->setObjectName(title);
        m_MenuItemList.append(m_CurrentMenu);
    } else {
        // Create the new menu as child of the given parent.
        QMenu *menu = new QMenu(mafTr(title.toAscii().constData()));
        menu->setObjectName(title);
        QMenu *parentMenu = (QMenu *)this->menuItemByName(parentName);
        if (parentMenu) {
            parentMenu->addMenu(menu);
            m_MenuItemList.append(menu);
        } else {
            qWarning() << mafTr("Wrong parent name") << "' " << parentName << "' " << mafTr("for menu ") << title;
            delete menu;
        }
    }
}

void mafGUIManager::parseMenuAttributes(QDomNode current) {
    if (current.nodeType() == QDomNode::ElementNode) {
        QDomElement ce = current.toElement();
        QString name = ce.tagName();
        if (name == "Menu") {
            // Create the new menu item.
            createMenuItem(ce);
        } else if (name == "Action") {
            // Create the new action.
            createAction(ce);
        } else if (name == "Separator" && m_CurrentMenu != NULL) {
            // Add a separator to the current menu item.
            m_CurrentMenu->addSeparator();
        } else if (name == "Toolbar") {
            // Add the new toolbar element
            createToolbar(ce);
        } else {
            qWarning() << mafTr("Unknown DomElement: ") << name << " !!";
        }
    }
}

QDomNode mafGUIManager::parseMenuTree(QDomNode current) {
    // Get the menu items...
    char *name = current.nodeName().toAscii().data();
    QDomNodeList dnl = current.childNodes();
    for (int n=0; n < dnl.count(); ++n) {
        QDomNode node = dnl.item(n);
        parseMenuAttributes(node);
        // Get the inner actions.
        parseMenuTree(node);
    }
    
    return current;
}

void mafGUIManager::createActions() {
    QAction *newAct = new QAction(QIcon(":/images/new.png"), mafTr("&New"), this);
    newAct->setIconText(mafTr("New"));
    newAct->setObjectName("New");
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(mafTr("Create a new file"));
    m_MenuItemList.append(newAct);

    QAction *collaborateAct = new QAction(QIcon(":/images/collaborate.png"), mafTr("Collaborate"), this);
    collaborateAct->setObjectName("Collaborate");
    collaborateAct->setCheckable(true);
    collaborateAct->setChecked(false);
    collaborateAct->setIconText(mafTr("Collaborate"));
    collaborateAct->setStatusTip(mafTr("Collaborate with your conmtacts in gtalk."));
    m_MenuItemList.append(collaborateAct);

    QAction *openAct = new QAction(QIcon(":/images/open.png"), mafTr("&Open..."), this);
    openAct->setObjectName("Open");
    openAct->setIconText(mafTr("Open"));
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(mafTr("Open an existing file"));
    m_MenuItemList.append(openAct);

    QAction *saveAct = new QAction(QIcon(":/images/save.png"), mafTr("&Save"), this);
    saveAct->setObjectName("Save");
    saveAct->setIconText(mafTr("Save"));
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(mafTr("Save the document to disk"));
    m_MenuItemList.append(saveAct);

    QAction *saveAsAct = new QAction(mafTr("Save &As..."), this);
    saveAsAct->setObjectName("SaveAs");
    saveAsAct->setIconText(mafTr("Save As"));
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(mafTr("Save the document under a new name"));
    m_MenuItemList.append(saveAsAct);

    m_RecentFileActs.clear();
    for (int i = 0; i < m_MaxRecentFiles; ++i) {
        QAction *recentFileAction = new QAction(this);
        m_MenuItemList.append(recentFileAction);
        recentFileAction->setVisible(false);
        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_RecentFileActs.append(recentFileAction);
    }

    QAction *exitAct = new QAction(mafTr("E&xit"), this);
    exitAct->setObjectName("Exit");
    exitAct->setIconText(mafTr("Exit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(mafTr("Exit the application"));
    m_MenuItemList.append(exitAct);
    connect(exitAct, SIGNAL(triggered()), m_MainWindow, SLOT(close()));

    QAction *cutAct = new QAction(QIcon(":/images/cut.png"), mafTr("Cu&t"), this);
    cutAct->setObjectName("Cut");
    cutAct->setIconText(mafTr("Cut"));
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(mafTr("Cut the current selection's contents to the "
                            "clipboard"));
    m_MenuItemList.append(cutAct);

    QAction *copyAct = new QAction(QIcon(":/images/copy.png"), mafTr("&Copy"), this);
    copyAct->setObjectName("Copy");
    copyAct->setIconText(mafTr("Copy"));
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(mafTr("Copy the current selection's contents to the "
                             "clipboard"));
    m_MenuItemList.append(copyAct);

    QAction *pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setObjectName("Paste");
    pasteAct->setIconText(mafTr("Paste"));
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    m_MenuItemList.append(pasteAct);

    QAction *aboutAct = new QAction(tr("&About"), this);
    aboutAct->setObjectName("About");
    aboutAct->setIconText(mafTr("About"));
    aboutAct->setStatusTip(tr("Show the application's About box"));
    m_MenuItemList.append(aboutAct);

    QAction *settingsAction = new QAction(mafTr("Settings"), this);
    settingsAction->setObjectName("Settings");
    settingsAction->setIconText(mafTr("Settings"));
    settingsAction->setStatusTip(tr("Show the application's Settings dialog"));
    m_MenuItemList.append(settingsAction);
    connect(settingsAction, SIGNAL(triggered()), SLOT(showSettingsDialog()));

    QAction *sideBarAct = new QAction(tr("Sidebar"), this);
    sideBarAct->setObjectName("SideBar");
    sideBarAct->setCheckable(true);
    sideBarAct->setChecked(true);
    m_MenuItemList.append(sideBarAct);

    QAction *logBarAct = new QAction(tr("LogBar"), this);
    logBarAct->setObjectName("LogBar");
    logBarAct->setCheckable(true);
    logBarAct->setChecked(true);
    m_MenuItemList.append(logBarAct);

    registerDefaultEvents();
}

void mafGUIManager::showSettingsDialog() {
    m_SettingsDialog->show();
}

void mafGUIManager::fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash pluginHash) {
    mafCore::mafObjectBase *sel_vme;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

    if(m_MenuItemList.size() == 0) {
        // Actions has not been created, so neither the menu.
        // Ask to create it which will create also the actions.
        createMenus();
    }
    QString base_class("");
    mafPluggedObjectInformation objInfo;
    mafPluggedObjectsHash::iterator iter = pluginHash.begin();
    while(iter != pluginHash.end()) {
        objInfo = iter.value();
        base_class = iter.key();
        if(base_class == "mafResources::mafOperation" || base_class == "mafResources::mafImporter" || 
           base_class == "mafResources::mafExporter" || base_class == "mafResources::mafView") {
            QAction *action = new QAction(mafTr(objInfo.m_Label.toAscii()), NULL);
            QVariant data_type(objInfo.m_ClassType);
            action->setData(data_type);
            QMenu *menu;
            char *bc = base_class.toAscii().data();
            if(base_class == "mafResources::mafOperation") {
                // Add a new item to the operation's menu.
                menu = (QMenu *)this->menuItemByName("Operations");
                if (menu) {
                    menu->addAction(action);
                    connect(action, SIGNAL(triggered()), this, SLOT(startOperation()));
                }
            } else if(base_class == "mafResources::mafImporter") {
                // Add a new item to the importer's menu.
                menu = (QMenu *)this->menuItemByName("Import");
                if (menu) {
                    menu->addAction(action);
                    connect(action, SIGNAL(triggered()), this, SLOT(startOperation()));
                }
            } else if(base_class == "mafResources::mafExporter") {
                // Add a new item to the exporter's menu.
                menu = (QMenu *)this->menuItemByName("Export");
                if (menu) {
                    menu->addAction(action);
                    connect(action, SIGNAL(triggered()), this, SLOT(startOperation()));
                }
            } else if(base_class == "mafResources::mafView") {
                // Add a new item to the view's menu.
                menu = (QMenu *)this->menuItemByName("Views");
                if (menu) {
                    menu->addAction(action);
                    connect(action, SIGNAL(triggered()), this, SLOT(createView()));
                }
            }
        }
        ++iter;
    }

    updateMenuForSelectedVme(sel_vme);
    updateTreeForSelectedVme(sel_vme);
}

QObject *mafGUIManager::menuItemByName(QString name) {
    for (int i = 0; i < m_MenuItemList.size(); ++i) {
        QObject *action = m_MenuItemList.at(i);
        QString an = action->objectName();
        if (an == name) {
            return action;
        }
    }
    return NULL;
}

void mafGUIManager::updateMenuForSelectedVme(mafCore::mafObjectBase *vme) {
    QStringList accepted_list;
    accepted_list = mafCoreRegistration::acceptObject(vme);

    QMenu *opMenu = (QMenu *)this->menuItemByName("Operations");
    QList<QAction *> opActions = opMenu->actions();
    QString op;
    foreach(QAction *action, opActions) {
        op = action->data().toString();
        action->setEnabled(accepted_list.contains(op));
    }
    
    QMenu *impMenu = (QMenu *)this->menuItemByName("Import");
    QList<QAction *> impActions= impMenu->actions();
    QString imp;
    foreach(QAction *action, impActions) {
        imp = action->data().toString();
        action->setEnabled(accepted_list.contains(imp));
    }
    
    QMenu *expMenu = (QMenu *)this->menuItemByName("Export");
    QList<QAction *> expActions= expMenu->actions();
    QString exp;
    foreach(QAction *action, expActions) {
        exp = action->data().toString();
        action->setEnabled(accepted_list.contains(exp));
    }
}

void mafGUIManager::updateTreeForSelectedVme(mafCore::mafObjectBase *vme) {
    if(m_TreeWidget) {
        QModelIndex index = m_Model->indexFromData(vme);
        
        QItemSelectionModel *sel = m_TreeWidget->selectionModel();
        if(sel) {
            sel->clearSelection();
            sel->setCurrentIndex(index, QItemSelectionModel::Select);
        }
        
        // Show the GUI for the selected VME.
        m_GUILoadedType = mafGUILoadedTypeVme;
        
        QString guiFilename = vme->uiFilename();
        if(guiFilename.isEmpty()) {
            showGui(NULL);
            return;
        }
        
        // Ask the UI Loader to load the operation's GUI.
        m_UILoader->uiLoad(guiFilename);
    }
}

void mafGUIManager::registerDefaultEvents() {
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.gui.action.new");
    provider->createNewId("maf.local.gui.action.open");
    provider->createNewId("maf.local.gui.action.save");
    provider->createNewId("maf.local.gui.action.saveAs");
    provider->createNewId("maf.local.gui.action.cut");
    provider->createNewId("maf.local.gui.action.copy");
    provider->createNewId("maf.local.gui.action.paste");
    provider->createNewId("maf.local.gui.action.about");

    // Register API signals.
    QObject *action;
    action = menuItemByName("New");
    mafRegisterLocalSignal("maf.local.gui.action.new", action, "triggered()");
    action = menuItemByName("Open");
    mafRegisterLocalSignal("maf.local.gui.action.open", action, "triggered()");
    action = menuItemByName("Save");
    mafRegisterLocalSignal("maf.local.gui.action.save", action, "triggered()");
    action = menuItemByName("SaveAs");
    mafRegisterLocalSignal("maf.local.gui.action.saveAs", action, "triggered()");
    action = menuItemByName("Cut");
    mafRegisterLocalSignal("maf.local.gui.action.cut", action, "triggered()");
    action = menuItemByName("Copy");
    mafRegisterLocalSignal("maf.local.gui.action.copy", action, "triggered()");
    action = menuItemByName("Paste");
    mafRegisterLocalSignal("maf.local.gui.action.paste", action, "triggered()");
    action = menuItemByName("About");
    mafRegisterLocalSignal("maf.local.gui.action.about", action, "triggered()");
}

void mafGUIManager::createDefaultMenus() {
    if(m_MenuItemList.size() == 0) {
        createActions();
    }

    QMenuBar *menuBar = m_MainWindow->menuBar();
    
    QMenu *importMenu = new QMenu(tr("Import"));
    importMenu->setObjectName("Import");
    QMenu *exportMenu = new QMenu(tr("Export"));
    exportMenu->setObjectName("Export");
    
    QMenu *fileMenu = menuBar->addMenu(tr("File"));
    fileMenu->setObjectName("File");
    fileMenu->addAction((QAction*)menuItemByName("New"));
    fileMenu->addSeparator();
    fileMenu->addAction((QAction*)menuItemByName("Open"));
    fileMenu->addAction((QAction*)menuItemByName("Collaborate"));
    fileMenu->addSeparator();
    fileMenu->addMenu(importMenu);
    fileMenu->addMenu(exportMenu);
    fileMenu->addSeparator();
    fileMenu->addAction((QAction*)menuItemByName("Save"));
    fileMenu->addAction((QAction*)menuItemByName("SaveAs"));
    
    m_RecentFilesSeparatorAct = fileMenu->addSeparator();
    for (int i = 0; i < m_MaxRecentFiles; ++i) {
        fileMenu->addAction(m_RecentFileActs.at(i));
    }
    
    fileMenu->addSeparator();
    fileMenu->addAction((QAction*)menuItemByName("Exit"));
    
    m_MenuItemList.append(fileMenu);
    
    menuBar->addSeparator();
    
    QMenu *editMenu = menuBar->addMenu(tr("Edit"));
    editMenu->setObjectName("Edit");
    editMenu->addAction((QAction*)menuItemByName("Cut"));
    editMenu->addAction((QAction*)menuItemByName("Copy"));
    editMenu->addAction((QAction*)menuItemByName("Paste"));
    m_MenuItemList.append(editMenu);
    
    menuBar->addSeparator();
    
    QMenu *viewMenu = menuBar->addMenu(tr("Views"));
    viewMenu->setObjectName("Views");
    m_MenuItemList.append(viewMenu);
    
    menuBar->addSeparator();
    
    QMenu *opMenu = menuBar->addMenu(tr("Operations"));
    opMenu->setObjectName("Operations");
    m_MenuItemList.append(opMenu);
    
    menuBar->addSeparator();
    
    QMenu *windowMenu = menuBar->addMenu(tr("Window"));
    windowMenu->setObjectName("Window");
    windowMenu->addAction((QAction*)menuItemByName("SideBar"));
    windowMenu->addAction((QAction*)menuItemByName("LogBar"));
    m_MenuItemList.append(windowMenu);
    
    menuBar->addSeparator();
    
    QMenu *helpMenu = menuBar->addMenu(tr("Help"));
    helpMenu->setObjectName("Help");
    helpMenu->addAction((QAction*)menuItemByName("About"));
    m_MenuItemList.append(helpMenu);
    
    QMenu *optionsMenu = menuBar->addMenu(tr("Options"));
    optionsMenu->setObjectName("Options");
    optionsMenu->addAction((QAction*)menuItemByName("Settings"));
    m_MenuItemList.append(optionsMenu);
    
    createDefaultToolbars();
}

void mafGUIManager::createMenus() {
    int errorLine, errorColumn;
    QString errorMsg;
    QFile modelFile("Menu.mnu");
    if (!modelFile.exists()) {
        qWarning() << "Menu.mnu " << tr("doesn't exists. The default menu will be created");
        createDefaultMenus();
        return;
    }

    QDomDocument document;
    if (!document.setContent(&modelFile, &errorMsg, &errorLine, &errorColumn)) {
        QString error(tr("Syntax error line %1, column %2:\n%3. Default menu will be created."));
        error = error
        .arg(errorLine)
        .arg(errorColumn)
        .arg(errorMsg);
        qCritical() << error;
        createDefaultMenus();
        return;
    }
    
    QDomNode m_CurrentNode = document.firstChild();
    parseMenuTree(m_CurrentNode);

    //Fill recent file menu
    m_RecentFileActs.clear();
    for (int i = 0; i < m_MaxRecentFiles; ++i) {
        QAction *recentFileAction = new QAction(this);
        m_MenuItemList.append(recentFileAction);
        recentFileAction->setVisible(false);
        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_RecentFileActs.append(recentFileAction);
    }
    updateRecentFileActions();
    QMenu *menu = (QMenu *)this->menuItemByName("Open Recent");
    if (menu) {
        for (int i = 0; i < m_RecentFileActs.count(); ++i) {
             menu->addAction(m_RecentFileActs.at(i));
        }
    }
 }

void mafGUIManager::createDefaultToolbars() {
    QToolBar *fileToolBar = m_MainWindow->addToolBar(tr("File"));
    fileToolBar->addAction((QAction*)menuItemByName("New"));
    fileToolBar->addAction((QAction*)menuItemByName("Collaborate"));
    fileToolBar->addAction((QAction*)menuItemByName("Open"));
    fileToolBar->addAction((QAction*)menuItemByName("Save"));
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QToolBar *editToolBar = m_MainWindow->addToolBar(tr("Edit"));
    editToolBar->addAction((QAction*)menuItemByName("Cut"));
    editToolBar->addAction((QAction*)menuItemByName("Copy"));
    editToolBar->addAction((QAction*)menuItemByName("Paste"));
    editToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void mafGUIManager::startOperation() {
    QAction *opAction = (QAction *)QObject::sender();
    m_OperationWidget->setOperationName(opAction->text());

    QString op(opAction->data().toString());
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, op));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.operation.start", mafEventTypeLocal, &argList);
}

void mafGUIManager::operationDidStart(mafCore::mafObjectBase *operation) {
    QMenu *opMenu = (QMenu *)this->menuItemByName("Operations");
    opMenu->setEnabled(false);

    // Get the started operation
    QString guiFilename = operation->uiFilename();
    m_OperationWidget->setOperation(operation);
    operation->setObjectName(m_OperationWidget->operationName());

    m_GUILoadedType = mafGUILoadedTypeOperation;
    
    if(guiFilename.isEmpty()) {
        showGui(NULL);
        return;
    }
    
    // Ask the UI Loader to load the operation's GUI.
    m_UILoader->uiLoad(guiFilename);
}

void mafGUIManager::removeOperationGUI() {
    QMenu *opMenu = (QMenu *)this->menuItemByName("Operations");
    opMenu->setEnabled(true);
    m_GUILoadedType = mafGUILoadedTypeOperation;
    emit guiTypeToRemove(m_GUILoadedType);
}

mafTreeWidget *mafGUIManager::createTreeWidget(mafTreeModel *model, QWidget *parent) {
//    QSettings settings;
    m_Model = model;
    m_TreeWidget = new mafTreeWidget();
    mafTreeItemDelegate *itemDelegate = new mafTreeItemDelegate(this);
    m_TreeWidget->setAnimated(true);
    m_TreeWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_TreeWidget->setMinimumSize(200, 0);
    m_TreeWidget->setMaximumSize(16777215, 16777215);
    connect(m_TreeWidget, SIGNAL(clicked(QModelIndex)), this, SLOT(selectVME(QModelIndex)));
    connect(m_Model, SIGNAL(itemAdded(QModelIndex)), m_TreeWidget, SLOT(expand(QModelIndex)));

    if(parent) {
        if(parent->layout()) {
            parent->layout()->addWidget(m_TreeWidget);
        } else {
            m_TreeWidget->setParent(parent);
        }
    }
    m_TreeWidget->setModel( m_Model );
    m_TreeWidget->setItemDelegate(itemDelegate);
    return m_TreeWidget;
}

mafTextEditWidget *mafGUIManager::createLogWidget(QWidget *parent) {

    mafTextEditWidget *w = m_Logger->textWidgetLog();
    w->setParent(parent);
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    w->enableEditing(false);

    if(parent) {
        if(parent->layout()) {
            parent->layout()->addWidget(w);
        } else {
            w->setParent(parent);
        }
    }

    return w;
}


void mafGUIManager::showGui(mafCore::mafProxyInterface *guiWidget) {
    // Get the widget from the container
    mafProxy<QWidget> *w = mafProxyPointerTypeCast(QWidget, guiWidget);
    QWidget *widget = NULL;
    
    if(w != NULL) {
        widget = *w;
    }

    switch(m_GUILoadedType) {
        case mafGUILoadedTypeOperation:
            m_OperationWidget->setOperationGUI(widget);
            emit guiLoaded(m_GUILoadedType, m_OperationWidget);
        break;
        case mafGUILoadedTypeView:
            {
                if (m_ViewWidget) {
                    m_ViewWidget->close();
                    emit guiTypeToRemove(mafGUILoadedTypeView);
                }
                m_ViewWidget = widget;

                /// get view selected
                mafCore::mafObjectBase *sel_view;
                QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_view);
                mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &ret_val);
                mafView *view = qobject_cast<mafView*>(sel_view);

                mafConnectObjectWithGUI(view->selectedSceneNode()->visualPipe(), m_ViewWidget);
                emit guiLoaded(m_GUILoadedType, m_ViewWidget);
            }
        break;
        case mafGUILoadedTypeVisualPipe:
        break;
        case mafGUILoadedTypeVme:
        {
            if (m_VMEWidget) {
                m_VMEWidget->close();
                emit guiTypeToRemove(mafGUILoadedTypeVme);
            }
            m_VMEWidget = widget;

            mafCore::mafObjectBase *sel_vme;
            QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

            mafConnectObjectWithGUI(sel_vme, m_VMEWidget);
            emit guiLoaded(m_GUILoadedType, m_VMEWidget);
        }
        break;
        default:
            qWarning() << mafTr("type %1 not recognized...").arg(m_GUILoadedType);
            return;
        break;
    }
}

void mafGUIManager::createView() {
    QAction *viewAction = (QAction *)QObject::sender();
    QString view(viewAction->data().toString());
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, view));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList);
}

void mafGUIManager::viewSelected(mafCore::mafObjectBase *view) {
    REQUIRE(view != NULL);
    // Set current hierarchy
    mafHierarchyPointer sceneGraph;
    sceneGraph = view->property("hierarchy").value<mafCore::mafHierarchyPointer>();
    if (m_Model) {
        // Set hierarchy of selected view and set the current index
        //m_Model->clear();
        mafTreeItemSceneNodeDelegate *itemSceneNodeDelegate = new mafTreeItemSceneNodeDelegate(this);//LEAKS!
        m_TreeWidget->setItemDelegate(itemSceneNodeDelegate);
        
        m_Model->setHierarchy(sceneGraph);
        //QModelIndex index = m_Model->index(0, 0);
        // TODO: select previous index
        //m_TreeWidget->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    }
    // Get the selected view's UI file
    QString guiFilename = view->uiFilename();
    if(guiFilename.isEmpty()) {
        showGui(NULL);
        return;
    }
    // Set the current panel to the parent panel of view properties.
    m_GUILoadedType = mafGUILoadedTypeView;
    // Ask the UI Loader to load the view's GUI.
    m_UILoader->uiLoad(guiFilename);
}

void mafGUIManager::viewDestroyed() { //ALL THE VIEWS ARE DESTROYED
    // Get hierarchy from mafVMEManager
    mafCore::mafHierarchyPointer vmeHierarchy;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, vmeHierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
    if (m_Model) {
        // Set VME hierarchy
        //m_Model->clear();
        // TODO: select previous index
        mafTreeItemDelegate *itemDelegate = new mafTreeItemDelegate(this);
        m_TreeWidget->setItemDelegate(itemDelegate);
        
        m_Model->setHierarchy(vmeHierarchy);
    }
}

void mafGUIManager::selectVME(QModelIndex index) {
    QObject *obj = dataObject(index);

    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, qobject_cast<mafCore::mafObjectBase *>(obj)));
    mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventBus::mafEventTypeLocal, &argList);
}

void mafGUIManager::chooseFileDialog(const QString title, const QString start_dir, const QString wildcard) {
    QString fileName = QFileDialog::getOpenFileName(m_MainWindow, title, start_dir, wildcard);
}

void mafGUIManager::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString file_to_open(action->data().toString());
        mafEventArgumentsList argList;
        argList.append(mafEventArgument(QString, file_to_open));
        mafEventBusManager::instance()->notifyEvent("maf.local.logic.openFile", mafEventTypeLocal, &argList);
    }
}

void mafGUIManager::updateRecentFileActions() {
    QSettings settings;
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

    //m_RecentFilesSeparatorAct->setVisible(numRecentFiles > 0);
}

QString mafGUIManager::strippedName(const QString &fullFileName) {
    return QFileInfo(fullFileName).fileName();
}

void mafGUIManager::save() {
    if(m_CompleteFileName.isEmpty()) {
        //open dialog for selecting the name of the session
        QFileDialog::Options options;
        // if (!native->isChecked())
        // options |= QFileDialog::DontUseNativeDialog;
        QString selectedFilter;
        m_CompleteFileName = QFileDialog::getSaveFileName(NULL,
                                                        mafTr("Save Session"),
                                                        mafTr(""),
                                                        mafTr("MAF Storage Format file (*.msf)"),
                                                        /*mafTr("All Files (*);;Text Files (*.xmsf)"),*/
                                                        &selectedFilter,
                                                        options);
        if (m_CompleteFileName.isEmpty()) {
            return;
        }
        int index = m_CompleteFileName.lastIndexOf("/");
        QString fileNameWithExt = m_CompleteFileName.mid(index+1);
        m_LastPath = m_CompleteFileName.left(index);
        m_LastPath.append("/");

        QString fileName = fileNameWithExt.split(".").at(0);
        QString path = m_LastPath.append(fileName);
        QDir saveDir(path);
        saveDir.mkpath(path);

        m_CompleteFileName = saveDir.path();
        m_CompleteFileName.append("/");
        m_CompleteFileName.append(fileNameWithExt);

        //Store memento hierarchy
        m_Logic->storeHierarchy(m_CompleteFileName);
        qDebug() << m_CompleteFileName;
    } else {
        int index = m_CompleteFileName.lastIndexOf("/");
        m_LastPath = m_CompleteFileName.left(index);
        m_Logic->storeHierarchy(m_CompleteFileName);
        qDebug() << m_CompleteFileName;
    }
    emit updateApplicationName();
    QSettings settings;
    QStringList recentFiles = settings.value("recentFileList").toStringList();
    recentFiles.insert(0, m_CompleteFileName);
    settings.setValue("recentFileList", recentFiles);
    updateRecentFileActions();
}

void mafGUIManager::saveAs() {
    m_CompleteFileName = "";
    save();
}

void mafGUIManager::open() {
    //open dialog for selecting the name of the session
    QFileDialog::Options options;
//    if (!native->isChecked())
//        options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(
                                                      NULL, tr("QFileDialog::getOpenFileNames()"),
                                                      m_LastPath,
                                                      mafTr("MAF Storage Format file (*.msf)"),
                                                      /*mafTr("All Files (*);;Text Files (*.xmsf)"),*/
                                                      &selectedFilter,
                                                      options);

    if (files.count() == 0) {
        return;
    }
    qDebug() << files[0];
    
    //Load memento hierarchy
    m_Logic->restoreHierarchy(files[0]);
    m_CompleteFileName = files[0];
    emit updateApplicationName();

    int index = m_CompleteFileName.lastIndexOf("/");
    m_LastPath = m_CompleteFileName.left(index);

    QSettings settings;
    QStringList recentFiles = settings.value("recentFileList").toStringList();
    recentFiles.insert(0, m_CompleteFileName);
    settings.setValue("recentFileList", recentFiles);
    updateRecentFileActions();
}

QObject *mafGUIManager::dataObject(QModelIndex index) {
    QTreeView *tree = (QTreeView *)QObject::sender();
    mafTreeItem *item = (mafTreeItem *)m_Model->itemFromIndex(index);
    QObject *obj = item->data();
    
    QString objName = "mafResources::mafVME";
    if (objName.compare(obj->metaObject()->className())  != 0) {
        QObject * vme;
        QGenericReturnArgument ret_val = mafEventReturnArgument(QObject *, vme);
        QMetaObject::invokeMethod(obj, "dataObject", Qt::AutoConnection, ret_val);
        obj = vme;
    }
    
    return obj;
}
