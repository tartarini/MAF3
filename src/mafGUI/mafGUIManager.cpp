/*
 *  mafGUIManager.cpp
 *  mafGUI
 *
 *  Created by Paolo Quadrani on 26/10/10.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#include "mafGUIManager.h"
#include "mafUILoaderQt.h"
#include "mafTreeWidget.h"
#include "mafTreeModel.h"
#include "mafLoggerWidget.h"
#include "mafTextEditWidget.h"
#include "mafGUIApplicationSettingsDialog.h"
#include "mafTreeItemSceneNodeDelegate.h"
#include "mafOperationWidget.h"
#include "mafViewCompoundConfigurator.h"

#include <mafObjectBase.h>


#ifdef __APPLE__
#define UI_PATH QString(QCoreApplication::instance()->applicationName()).append("/Contents/MacOS/")
#else
#define UI_PATH ""
#endif

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

mafGUIManager::mafGUIManager(QMainWindow *main_win, const QString code_location) : mafObjectBase(code_location)
    , m_VMEWidget(NULL), m_ViewWidget(NULL), m_VisualPipeWidget(NULL), m_UILoadedFromFile(false), m_CurrentPipeVisual(NULL), m_CurrentView(NULL), m_CurrentVME(NULL), m_MaxRecentFiles(5), m_MainWindow(main_win)
    , m_Model(NULL), m_TreeWidget(NULL), m_Logic(NULL), m_CompleteFileName(), m_LastPath() {

    m_SettingsDialog = new mafGUIApplicationSettingsDialog();
    m_OperationWidget = new mafOperationWidget();
    connect(m_OperationWidget, SIGNAL(operationDismissed()), this, SLOT(removeOperationGUI()));

    m_Logger = new mafLoggerWidget(mafCodeLocation);

    mafCore::mafMessageHandler::instance()->setActiveLogger(m_Logger);

    mafRegisterLocalSignal("maf.local.gui.new", this, "newWorkinSessioneSignal()");
    mafRegisterLocalCallback("maf.local.gui.new", this, "newWorkingSession()");

    mafRegisterLocalCallback("maf.local.resources.plugin.registerLibrary", this, "fillMenuWithPluggedObjects(mafCore::mafPluggedObjectsHash)")

    mafRegisterLocalSignal("maf.local.gui.compoundWidgetConfigure", this, "parseCompoundLayoutFileSignal(QString)");
    mafRegisterLocalCallback("maf.local.gui.compoundWidgetConfigure", this, "parseCompoundLayoutFile(QString)")

    // VME selection callbacks.
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "updateMenuForSelectedVme(mafCore::mafObjectBase *)")
    mafRegisterLocalCallback("maf.local.resources.vme.select", this, "updateTreeForSelectedVme(mafCore::mafObjectBase *)")
    
    // OperationManager's callback
    mafRegisterLocalCallback("maf.local.resources.operation.started", this, "operationDidStart(mafCore::mafObjectBase *)");
    
    // ViewManager's callback.
    mafRegisterLocalCallback("maf.local.resources.view.select", this, "viewSelected(mafCore::mafObjectBase *)");
    mafRegisterLocalCallback("maf.local.resources.view.noneViews", this, "viewDestroyed()");
    mafRegisterLocalCallback("maf.local.resources.view.sceneNodeShow", this, "setVMECheckState(mafCore::mafObjectBase *, bool)");

    m_UILoader = mafNEW(mafGUI::mafUILoaderQt);
    connect(m_UILoader, SIGNAL(uiLoadedSignal(mafCore::mafProxyInterface*, int)), this, SLOT(showGui(mafCore::mafProxyInterface*, int)));
}

mafGUIManager::~mafGUIManager() {
    mafDEL(m_Logger);
    mafDEL(m_UILoader);
}

void mafGUIManager::newWorkingSession() {
    m_Model->setTreeModelStatus(mafTreeModelStatusGenerate);

    // Clear all the previously created hierarchies and create a new one.
    mafHierarchyPointer h = m_Logic->requestNewHierarchy();

    /// Ask the view manager for the selected view
    mafCore::mafObjectBase *sel_view;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_view);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &ret_val);

    if (sel_view) {
        // if any, ask it its SceneGraph
        h = sel_view->property("sceneGraph").value<mafCore::mafHierarchyPointer>();
    }
    
    // Assign the hierarchy to the tree model
    m_Model->setHierarchy(h);
    
    // select the root node.
    QModelIndex index = m_Model->index(0, 0);
    m_TreeWidget->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
    m_CompleteFileName = "";
    Q_EMIT updateApplicationName();
}

void mafGUIManager::quitApplication() {
    // post the closeEvent
    QApplication::postEvent(qApp, new QCloseEvent());
}

void mafGUIManager::createToolbar(QDomElement node) {
    QDomNamedNodeMap attributes = node.attributes();
    QString title = attributes.namedItem("title").nodeValue();
    QString actions = attributes.namedItem("actionList").nodeValue();

    QByteArray ba = title.toAscii();
    QToolBar *toolBar = m_MainWindow->addToolBar(tr(ba.constData()));

    QStringList actionList = actions.split(",");
    Q_FOREACH (QString action, actionList) {
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

    QByteArray ba = title.toAscii();
    QAction *action = new QAction(QIcon(icon), mafTr(ba.constData()), this);
    action->setIconText(mafTr(ba.constData()));
    action->setObjectName(title);
//    action->setShortcuts(QKeySequence::New);
    ba = tip.toAscii();
    action->setStatusTip(mafTr(ba.constData()));
    action->setCheckable(checkable.toInt() != 0);
    action->setChecked(checked.toInt() != 0);
    if (!slot.isEmpty()) {
        slot.prepend(CALLBACK_SIGNATURE);
        ba = slot.toAscii();
        connect(action, SIGNAL(triggered()), this, ba.constData());
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

    QByteArray ba = title.toAscii();
    if (parentName.isEmpty()) {
        // Create a new menu item.
        QMenuBar *menuBar = m_MainWindow->menuBar();
        m_CurrentMenu = menuBar->addMenu(mafTr(ba.constData()));
        m_CurrentMenu->setObjectName(title);
        m_MenuItemList.append(m_CurrentMenu);
    } else {
        // Create the new menu as child of the given parent.
        QMenu *menu = new QMenu(mafTr(ba.constData()));
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
    QByteArray ba = current.nodeName().toAscii();
    char *name = ba.data();
    QDomNodeList dnl = current.childNodes();
    for (int n=0; n < dnl.count(); ++n) {
        QDomNode node = dnl.item(n);
        parseMenuAttributes(node);
        // Get the inner actions.
        parseMenuTree(node);
    }
    
    return current;
}

QObject *mafGUIManager::parseCompoundLayoutFile(QString layoutFile) {
    QString layoutFullPath(QCoreApplication::applicationDirPath());
    layoutFullPath.append("/");
    layoutFullPath.append(layoutFile);
    
    mafViewCompoundConfigurator *configurator = new mafViewCompoundConfigurator();
    configurator->parseConfigurationFile(layoutFullPath);
    QObject *obj = configurator->rootObject();
    delete configurator;
    return obj;
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
    mafCore::mafObjectBase *sel_vme = NULL;
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
            QByteArray ba = base_class.toAscii();
            char *bc = ba.data();
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
        if (an.compare(name) == 0) {
            return action;
        }
    }
    return NULL;
}

void mafGUIManager::updateMenuForSelectedVme(mafCore::mafObjectBase *vme) {
    if (vme && vme->isObjectValid()) {
        m_CurrentVME = vme;
        QStringList accepted_list;
        accepted_list = mafCoreRegistration::acceptObject(vme);

        QMenu *opMenu = (QMenu *)this->menuItemByName("Operations");
        if (opMenu) {
            QList<QAction *> opActions = opMenu->actions();
            QString op;
            Q_FOREACH(QAction *action, opActions) {
                op = action->data().toString();
                action->setEnabled(accepted_list.contains(op));
            }
        }

        QMenu *impMenu = (QMenu *)this->menuItemByName("Import");
        if (impMenu) {
            QList<QAction *> impActions= impMenu->actions();
            QString imp;
            Q_FOREACH(QAction *action, impActions) {
                imp = action->data().toString();
                action->setEnabled(accepted_list.contains(imp));
            }
        }

        QMenu *expMenu = (QMenu *)this->menuItemByName("Export");
        if (expMenu) {
            QList<QAction *> expActions= expMenu->actions();
            QString exp;
            Q_FOREACH(QAction *action, expActions) {
                exp = action->data().toString();
                action->setEnabled(accepted_list.contains(exp));
            }
        }
    }
}

void mafGUIManager::updateTreeForSelectedVme(mafCore::mafObjectBase *vme) {
    if (vme && vme->isObjectValid()) {
        if(m_TreeWidget) {
            QModelIndex index = m_Model->indexFromData(vme);
            if (!index.isValid()) {
                return;
            }

            QItemSelectionModel *sel = m_TreeWidget->selectionModel();
            if(sel) {
                sel->clearSelection();
                sel->setCurrentIndex(index, QItemSelectionModel::Select);
            }

            // Show the GUI for the selected VME.
            QString path(UI_PATH);
            QString guiFilename = path.append(vme->uiFilename());
            m_UILoadedFromFile = !guiFilename.isEmpty();
            if(m_UILoadedFromFile) {
                // Ask the UI Loader to load the operation's GUI.
                m_UILoader->uiLoad(guiFilename, mafGUILoadedTypeVme);
                return;
            }

            showGui(NULL, mafGUILoadedTypeVme);
        }
    }
}

void mafGUIManager::updateGuiForSelectedPipeVisual(mafCore::mafObjectBase *pipeVisual) {
    if (pipeVisual) {
        if (m_CurrentPipeVisual == pipeVisual) {
            return;
        }
        m_CurrentPipeVisual = pipeVisual;
        QString path(UI_PATH);
        QString guiFilename = path.append(pipeVisual->uiFilename());
        m_UILoadedFromFile = !guiFilename.isEmpty();
        if(m_UILoadedFromFile) {
            // Ask the UI Loader to load the view's GUI.
            m_UILoader->uiLoad(guiFilename, mafGUILoadedTypeVisualPipe);
            return;
        }
    } 
    m_CurrentPipeVisual = NULL;
    showGui(NULL, mafGUILoadedTypeVisualPipe);
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
    m_MenuItemList.append(importMenu);
    m_MenuItemList.append(exportMenu);
    
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
    QString path(UI_PATH);
    QString guiFilename = path.append(operation->uiFilename());
    m_OperationWidget->setOperation(operation);
    operation->setObjectName(m_OperationWidget->operationName());

    if(guiFilename.isEmpty()) {
        QObject *customUI = operation->uiRootWidget();
        QWidget *customUIWidget = qobject_cast<QWidget *>(customUI);
        if (customUI == NULL) {
            // No GUI associated with the object...
            showGui(NULL, mafGUILoadedTypeOperation);
            return;
        } else {
            mafProxy<QWidget> gui;
            gui = customUIWidget;
            showGui(&gui, mafGUILoadedTypeOperation);
            return;
        }
    } else {    
        // Ask the UI Loader to load the operation's GUI.
        m_UILoader->uiLoad(guiFilename, mafGUILoadedTypeOperation);
    }
    
    // block the selection if the operation is single thread
    mafTreeItemDelegate *d = (mafTreeItemDelegate *) m_TreeWidget->itemDelegate();
    d->setGlobalLock(true);
    m_TreeWidget->setEnabled(false);
}

void mafGUIManager::removeOperationGUI() {
    QMenu *opMenu = (QMenu *)this->menuItemByName("Operations");
    opMenu->setEnabled(true);
    Q_EMIT guiTypeToRemove(mafGUILoadedTypeOperation);
    
    /// enable the tree
    mafTreeItemDelegate *d = (mafTreeItemDelegate *) m_TreeWidget->itemDelegate();
    d->setGlobalLock(false);
    m_TreeWidget->setEnabled(true);
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
    mafCore::mafLogger *logger = mafCore::mafMessageHandler::instance()->activeLogger();
    mafLoggerWidget *loggerWidget = dynamic_cast<mafLoggerWidget *>(logger);
    if (loggerWidget == NULL) {
        qCritical() << mafTr("Cannot create widget for logger. It should be a subclass of mafLoggerWidget!!");
        return NULL;
    }
    mafTextEditWidget *w = loggerWidget->textWidgetLog();
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

void mafGUIManager::showGui(mafCore::mafProxyInterface *guiWidget, int ui_type) {
    // Get the widget from the container
    mafProxy<QWidget> *w = mafProxyPointerTypeCast(QWidget, guiWidget);
    QWidget *widget = NULL;
    
    if(w != NULL) {
        widget = *w;
    }

    switch(ui_type) {
        case mafGUILoadedTypeOperation:
             m_OperationWidget->setOperationGUI(widget);
             Q_EMIT guiLoaded(ui_type, m_OperationWidget);
        break;
        case mafGUILoadedTypeView:
            {
                if (m_ViewWidget) {
                    m_ViewWidget->close();
                    Q_EMIT guiTypeToRemove(mafGUILoadedTypeView);
                }
                m_ViewWidget = widget;
                if (m_ViewWidget) {
                    if (m_UILoadedFromFile) {
                        mafConnectObjectWithGUI(m_CurrentView, m_ViewWidget);
                    }
                    Q_EMIT guiLoaded(ui_type, m_ViewWidget);
                }
            }
        break;
        case mafGUILoadedTypeVisualPipe:
            {
                if (m_VisualPipeWidget) {
                    m_VisualPipeWidget->close();
                    Q_EMIT guiTypeToRemove(mafGUILoadedTypeVisualPipe);
                }
                m_VisualPipeWidget = widget;
                if (m_CurrentPipeVisual && m_VisualPipeWidget) {
                    if (m_UILoadedFromFile) {
                        mafConnectObjectWithGUI(m_CurrentPipeVisual, m_VisualPipeWidget);
                    }
                    Q_EMIT guiLoaded(ui_type, m_VisualPipeWidget);
                }
            }
        break;
        case mafGUILoadedTypeVme:
        {
            if (m_VMEWidget) {
                m_VMEWidget->close();
                Q_EMIT guiTypeToRemove(mafGUILoadedTypeVme);
            }
            m_VMEWidget = widget;

            mafCore::mafObjectBase *sel_vme = NULL;
            QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_vme);
            mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.selected", mafEventTypeLocal, NULL, &ret_val);

            if (sel_vme) {
                if (m_UILoadedFromFile) {
                    mafConnectObjectWithGUI(sel_vme, m_VMEWidget);
                }
                Q_EMIT guiLoaded(ui_type, m_VMEWidget);
            }
        }
        break;
        default:
            qWarning() << mafTr("type %1 not recognized...").arg(ui_type);
            return;
        break;
    }
}

void mafGUIManager::createView() {
    QAction *viewAction = (QAction *)QObject::sender();
    QString view_type(viewAction->data().toString());
    QString view_name(viewAction->text());
    mafEventArgumentsList argList;
    argList.append(mafEventArgument(QString, view_type));
    argList.append(mafEventArgument(QString, view_name));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.create", mafEventTypeLocal, &argList);

    mafCore::mafObjectBase *sel_view = NULL;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafObjectBase *, sel_view);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.view.selected", mafEventTypeLocal, NULL, &ret_val);
    if (sel_view) {
        connect(sel_view, SIGNAL(pipeVisualSelectedSignal(mafCore::mafObjectBase *)), this, SLOT(updateGuiForSelectedPipeVisual(mafCore::mafObjectBase *)));
    }
}


void mafGUIManager::viewSelected(mafCore::mafObjectBase *view) {
    REQUIRE(view != NULL);
    if (m_CurrentView == view) {
        return;
    }
    m_CurrentView = view;
    
    // Set current hierarchy
    mafHierarchyPointer sceneGraph;
    sceneGraph = view->property("sceneGraph").value<mafCore::mafHierarchyPointer>();
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
    QString path(UI_PATH);
    QString guiFilename = path.append(view->uiFilename());
    m_UILoadedFromFile = !guiFilename.isEmpty();
    if(!m_UILoadedFromFile) {
        // Check if there is a custom widget associated with the class...
        QObject *customUI = view->uiRootWidget();
        QWidget *customUIWidget = qobject_cast<QWidget *>(customUI);
        if (customUI == NULL) {
            // No GUI associated with the object...
            showGui(NULL, mafGUILoadedTypeView);
            return;
        } else {
            mafProxy<QWidget> gui;
            gui = customUIWidget;
            showGui(&gui, mafGUILoadedTypeView);
            return;
        }
    }
    
    // Ask the UI Loader to load the view's GUI.
    m_UILoader->uiLoad(guiFilename, mafGUILoadedTypeView);
}

void mafGUIManager::viewDestroyed() { //ALL THE VIEWS ARE DESTROYED
    // Tis method is called when last view is destroyed, so no other view will be selected.
    // Remove its GUI.
    this->showGui(NULL, mafGUILoadedTypeView);

    // Reset value for current view selected and corresponding visual pipe.
    m_CurrentView = NULL;
    m_CurrentPipeVisual = NULL;

    // Get hierarchy from mafVMEManager
    mafCore::mafHierarchyPointer vmeHierarchy = NULL;
    QGenericReturnArgument ret_val = mafEventReturnArgument(mafCore::mafHierarchyPointer, vmeHierarchy);
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.hierarchy.request", mafEventTypeLocal, NULL, &ret_val);
    if (m_Model && vmeHierarchy) {
        // TODO: select previous index
        mafTreeItemDelegate *itemDelegate = new mafTreeItemDelegate(this);
        m_TreeWidget->setItemDelegate(itemDelegate);
        
        m_Model->setHierarchy(vmeHierarchy);
    }
}

void mafGUIManager::selectVME(const QModelIndex &index) {
    if (!index.isValid()) {
        return;
    }

    QObject *obj = dataObject(index);
    if (obj && obj != m_CurrentVME) {
        mafEventBus::mafEventArgumentsList argList;
        argList.append(mafEventArgument(mafCore::mafObjectBase *, qobject_cast<mafCore::mafObjectBase *>(obj)));
        mafEventBus::mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.select", mafEventBus::mafEventTypeLocal, &argList);
    }
}

void mafGUIManager::setVMECheckState(mafCore::mafObjectBase *vme, bool visible) {
    if(m_TreeWidget) {
        if (vme) {
            QModelIndex index = m_Model->indexFromData(vme);
            mafTreeItem *item = (mafTreeItem *)m_Model->itemFromIndex(index);
            bool checked = item->checkState() == Qt::Checked || item->checkState() == Qt::PartiallyChecked;
            if (item->isCheckable() && checked != visible) {
                item->setStatus(mafItemStatusCheckable, visible);
            }
        }
    }
}

void mafGUIManager::chooseFileDialog(const QString title, const QString start_dir, const QString wildcard) {
    QString fileName = QFileDialog::getOpenFileName(m_MainWindow, title, start_dir, wildcard);
}

void mafGUIManager::openRecentFile() {
    QAction *action = qobject_cast<QAction *>(sender());
    if (action) {
        QString file_to_open(action->data().toString());
        if (!file_to_open.isEmpty() && QFile(file_to_open).exists()){
            newWorkingSession();

            mafEventArgumentsList argList;
            argList.append(mafEventArgument(QString, file_to_open));
            mafEventBusManager::instance()->notifyEvent("maf.local.logic.openFile", mafEventTypeLocal, &argList);

            m_CompleteFileName = file_to_open;
            Q_EMIT updateApplicationName();
            updateRecentFileMenu(m_CompleteFileName);
        } else {
            qCritical() << mafTr("Cannot find file <%1>. File doesn't exist.").arg(file_to_open);
            //remove file from list of recent file.
            QSettings settings;
            QStringList recentFiles = settings.value("recentFileList").toStringList();
            recentFiles.removeOne(file_to_open);
            settings.setValue("recentFileList", recentFiles);
            updateRecentFileActions();
        }
    }
}

void mafGUIManager::updateRecentFileActions() {
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();

    int numRecentFiles = qMin(files.size(), (int)m_MaxRecentFiles);

    for (int i = 0; i < numRecentFiles; ++i) {
        //Remove comment if don't want the full path in open recent menu
        //QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
        m_RecentFileActs.at(i)->setText(files[i]/*text*/);
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
    bool enableSaveAs(false);
    if(m_CompleteFileName.isEmpty()) {
        enableSaveAs = true;
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

    } else {
        int index = m_CompleteFileName.lastIndexOf("/");
        m_LastPath = m_CompleteFileName.left(index);
    }
    
    //Store memento hierarchy
    m_Logic->storeHierarchy(m_CompleteFileName, enableSaveAs);
    qDebug() << m_CompleteFileName;
    Q_EMIT updateApplicationName();
    updateRecentFileMenu(m_CompleteFileName);
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
    
    newWorkingSession();

    //Load memento hierarchy
    m_Logic->restoreHierarchy(files[0]);
    m_CompleteFileName = files[0];
    Q_EMIT updateApplicationName();

    int index = m_CompleteFileName.lastIndexOf("/");
    m_LastPath = m_CompleteFileName.left(index);

    updateRecentFileMenu(m_CompleteFileName);
}

void mafGUIManager::updateRecentFileMenu(QString fileName) {
    QSettings settings;
    QStringList recentFiles = settings.value("recentFileList").toStringList();
    recentFiles.insert(0, m_CompleteFileName);
    recentFiles.removeDuplicates();
    settings.setValue("recentFileList", recentFiles);
    updateRecentFileActions();
}

QObject *mafGUIManager::dataObject(QModelIndex index) {
    QObject *obj(NULL);
    mafTreeItem *item = (mafTreeItem *)m_Model->itemFromIndex(index);
    if (item) {
        obj = item->data();
    }
    
    if (obj) {
        QString objName("mafResources::mafVME");
        if (objName.compare(obj->metaObject()->className())  != 0) {
            QObject * vme(NULL);
            QGenericReturnArgument ret_val = mafEventReturnArgument(QObject *, vme);
            QMetaObject::invokeMethod(obj, "dataObject", Qt::AutoConnection, ret_val);
            obj = vme;
        }
    }
    
    return obj;
}
