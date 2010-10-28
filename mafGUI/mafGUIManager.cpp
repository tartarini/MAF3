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

using namespace mafCore;
using namespace mafEventBus;
using namespace mafGUI;

mafGUIManager::mafGUIManager(const mafString code_location) : mafObjectBase(code_location) {
}

mafGUIManager::~mafGUIManager() {
}

void mafGUIManager::createActions(QMainWindow *win) {
    m_NewAct = new QAction(QIcon(":/images/new.png"), mafTr("&New"), this);
    m_NewAct->setIconText(mafTr("New"));
    m_NewAct->setShortcuts(QKeySequence::New);
    m_NewAct->setStatusTip(mafTr("Create a new file"));

    m_CollaborateAct = new QAction(QIcon(":/images/collaborate.png"), mafTr("Collaborate"), this);
    m_CollaborateAct->setIconText(mafTr("Collaborate"));
    m_CollaborateAct->setStatusTip(mafTr("Collaborate with your conmtacts in gtalk."));

    m_OpenAct = new QAction(QIcon(":/images/open.png"), mafTr("&Open..."), this);
    m_OpenAct->setIconText(mafTr("Open"));
    m_OpenAct->setShortcuts(QKeySequence::Open);
    m_OpenAct->setStatusTip(mafTr("Open an existing file"));

    m_SaveAct = new QAction(QIcon(":/images/save.png"), mafTr("&Save"), this);
    m_SaveAct->setIconText(mafTr("Save"));
    m_SaveAct->setShortcuts(QKeySequence::Save);
    m_SaveAct->setStatusTip(mafTr("Save the document to disk"));

    m_SaveAsAct = new QAction(mafTr("Save &As..."), this);
    m_SaveAsAct->setIconText(mafTr("Save As"));
    m_SaveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_SaveAsAct->setStatusTip(mafTr("Save the document under a new name"));

    m_ExitAct = new QAction(mafTr("E&xit"), this);
    m_ExitAct->setIconText(mafTr("Exit"));
    m_ExitAct->setShortcuts(QKeySequence::Quit);
    m_ExitAct->setStatusTip(mafTr("Exit the application"));
    connect(m_ExitAct, SIGNAL(triggered()), win, SLOT(close()));

    m_CutAct = new QAction(QIcon(":/images/cut.png"), mafTr("Cu&t"), this);
    m_CutAct->setIconText(mafTr("Cut"));
    m_CutAct->setShortcuts(QKeySequence::Cut);
    m_CutAct->setStatusTip(mafTr("Cut the current selection's contents to the "
                            "clipboard"));

    m_CopyAct = new QAction(QIcon(":/images/copy.png"), mafTr("&Copy"), this);
    m_CopyAct->setIconText(mafTr("Copy"));
    m_CopyAct->setShortcuts(QKeySequence::Copy);
    m_CopyAct->setStatusTip(mafTr("Copy the current selection's contents to the "
                             "clipboard"));

    m_PasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    m_PasteAct->setIconText(mafTr("Paste"));
    m_PasteAct->setShortcuts(QKeySequence::Paste);
    m_PasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));

    m_AboutAct = new QAction(tr("&About"), this);
    m_AboutAct->setIconText(mafTr("About"));
    m_AboutAct->setStatusTip(tr("Show the application's About box"));

    registerSignals();
}

void mafGUIManager::registerSignals() {
    mafIdProvider *provider = mafIdProvider::instance();
    provider->createNewId("maf.local.gui.action.new");
    provider->createNewId("maf.local.gui.action.open");
    provider->createNewId("maf.local.gui.action.save");
    provider->createNewId("maf.local.gui.action.saveas");
    provider->createNewId("maf.local.gui.action.collaborate");
    provider->createNewId("maf.local.gui.action.cut");
    provider->createNewId("maf.local.gui.action.copy");
    provider->createNewId("maf.local.gui.action.paste");
    provider->createNewId("maf.local.gui.action.about");

    // Register API signals.
    mafRegisterLocalSignal("maf.local.gui.action.new", m_NewAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.open", m_OpenAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.save", m_SaveAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.saveas", m_SaveAsAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.collaborate", m_CollaborateAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.cut", m_CutAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.copy", m_CopyAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.paste", m_PasteAct, "triggered()");
    mafRegisterLocalSignal("maf.local.gui.action.about", m_AboutAct, "triggered()");
}

void mafGUIManager::createMenus(QMainWindow *win) {
    QMenuBar *menuBar = win->menuBar();

    m_FileMenu = menuBar->addMenu(tr("&File"));
    m_FileMenu->addAction(m_NewAct);
    m_FileMenu->addAction(m_CollaborateAct);
    m_FileMenu->addAction(m_OpenAct);
    m_FileMenu->addAction(m_SaveAct);
    m_FileMenu->addAction(m_SaveAsAct);
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

    m_HelpMenu = menuBar->addMenu(tr("&Help"));
    m_HelpMenu->addAction(m_AboutAct);
}

void mafGUIManager::createToolBars(QMainWindow *win) {
    m_FileToolBar = win->addToolBar(tr("File"));
    m_FileToolBar->addAction(m_NewAct);
    m_FileToolBar->addAction(m_CollaborateAct);
    m_FileToolBar->addAction(m_OpenAct);
    m_FileToolBar->addAction(m_SaveAct);
    m_FileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    m_EditToolBar = win->addToolBar(tr("Edit"));
    m_EditToolBar->addAction(m_CutAct);
    m_EditToolBar->addAction(m_CopyAct);
    m_EditToolBar->addAction(m_PasteAct);
    m_EditToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}
