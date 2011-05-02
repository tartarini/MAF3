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


#define HAVE_SWIG
#define HAVE_PYTHON

#include <mafInterpreterPreferencesWidget.h>

using namespace mafCore;
using namespace mafGUI;
using namespace mafScriptInterpreter;


mafSplitter::mafSplitter(QWidget * parent) : QSplitter( parent )
{
        setOpaqueResize(true);
}

void mafSplitter::paintEvent (QPaintEvent *)
{
        ((mafMainWindow*)parent()->parent())->synchronizeSplitters(this);
}

void mafSplitter::adjustPosition(int p)
{
        moveSplitter(p,1);
        refresh();
}


mafMainWindow::mafMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mafMainWindow), m_Logic(NULL), m_Interpreter(NULL) {
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
    
    if(m_Interpreter) {
        delete m_Interpreter;
    }
}

void mafMainWindow::initializeMainWindow() {
    ui->setupUi(this);

    mafGUIRegistration::registerGUIObjects();
    m_GUIManager = new mafGUIManager(this, mafCodeLocation);

    m_GUIManager->createMenus();
//    m_GUIManager->createToolBars();

    
    connectCallbacks();

    ui->statusBar->showMessage(mafTr("Ready!"));

    // Connecting layouts (needed because from QtDesign is not managed automatically)
    ui->centralWidget->setLayout(ui->gridLayout);
	     
     QSplitter * mainSplitter = new QSplitter(this);
     mainSplitter->setOrientation(Qt::Vertical);
     mainSplitter->setOpaqueResize(true);

     m_HorizontalSplitterTop = new mafSplitter(mainSplitter);
     //m_LeftTopPanel = new QTextEdit(m_HorizontalSplitterTop);
     //m_LeftTopPanel->setText("here... <b>command console</b> (progress for python console and scripts)");
     /// intepreter creation
     m_Interpreter = new mafInterpreter(m_HorizontalSplitterTop);
    m_PreferencesAction = new QAction("Preferences", this);
    m_PreferencesAction->setShortcut(Qt::ControlModifier + Qt::Key_Comma);
    connect(m_PreferencesAction, SIGNAL(triggered()), this, SLOT(showPreferences()));

    
     m_RightTopPanel = new QTextEdit(m_HorizontalSplitterTop);
     m_RightTopPanel->setText("here... <b>workflow execution</b> (progress on highlighted and autocomplete text editor, or other solution...");

     m_HorizontalSplitterBottom = new mafSplitter(mainSplitter);
     m_LeftBottomPanel = new QTextEdit(m_HorizontalSplitterBottom);
     m_LeftBottomPanel->setText("here... <b>status monitor</b> (which services are available");
     m_RightBottomPanel = new QTextEdit(m_HorizontalSplitterBottom);
     m_RightBottomPanel->setText("here... <b>log monitor</b> (need to create session files or record in db which can be requested");
     setCentralWidget(mainSplitter);

    // **** LogBar ****
    // LogBar Layout
    /*ui->logBarWidgetContents->setLayout(ui->gridLayoutLogBar);

    QObject *logBarAction = m_GUIManager->menuItemByName("LogBar");
    m_LogWidget = m_GUIManager->createLogWidget(ui->logBarWidgetContents);
    connect(ui->dockLogBarWidget, SIGNAL(visibilityChanged(bool)), logBarAction, SLOT(setChecked(bool)));
    connect(logBarAction, SIGNAL(triggered(bool)), ui->dockLogBarWidget, SLOT(setVisible(bool)));*/
    
    ui->dockLogBarWidget->hide();

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
    switch(type) {
    case mafGUILoadedTypeOperation:
        break;
    case mafGUILoadedTypeView:
        break;
    }
    w->show();
}

void mafMainWindow::loadedGUIToRemove(int type) {
}

void mafMainWindow::readSettings() {
    qDebug() << "Reading mafMainWindows settings...";
    QSettings settings;
    
    // reastoring MainWindow
    QPoint pos = settings.value("MainWindow/Position", QPoint(200, 200)).toPoint();
    QSize size = settings.value("MainWindow/Size", QSize(600, 400)).toSize();
    resize(size);
    move(pos);

    // Restoring LogBar
    //int docPos = settings.value("LogBar/DockPosition", Qt::BottomDockWidgetArea).toInt();
    //this->addDockWidget((Qt::DockWidgetArea)docPos, ui->dockLogBarWidget);
    //ui->dockLogBarWidget->setFloating(settings.value("LogBar/isFloating", false).toBool());
    //ui->dockLogBarWidget->setVisible(settings.value("LogBar/isVisible", true).toBool());
//    QRect rectLog = settings.value("LogBar/Geometry", QRect(0, 0, 800, 100)).toRect();
//    ui->dockLogBarWidget->resize(rectLog.size());
}

void mafMainWindow::writeSettings() {
    qDebug() << "Writing mafMainWindows settings...";
    QSettings settings;
    settings.setValue("MainWindow/Position", pos());
    settings.setValue("MainWindow/Size", size());

    // Save the LogBar settings
    /*settings.setValue("LogBar/Geometry", ui->dockLogBarWidget->geometry());
    settings.setValue("LogBar/DockPosition", this->dockWidgetArea(ui->dockLogBarWidget));
    settings.setValue("LogBar/isFloating", ui->dockLogBarWidget->isFloating());
    settings.setValue("LogBar/isVisible", ui->dockLogBarWidget->isVisible());*/

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
    mafContainerInterfacePointer widgetContainer;
    widgetContainer = view->property("renderWidget").value<mafCore::mafContainerInterfacePointer>();

    QWidget *widget = mafContainerPointerTypeCast(QWidget, widgetContainer)->externalData();
    QMdiSubWindow *sub_win = ui->mdiArea->addSubWindow(widget);
    sub_win->setAttribute(Qt::WA_DeleteOnClose);
    sub_win->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(sub_win, SIGNAL(aboutToActivate()), this, SLOT(viewWillBeSelected()));
    connect(sub_win, SIGNAL(destroyed()), view, SLOT(deleteLater()));

    widget->setParent(sub_win);
    sub_win->setMinimumSize(200, 200);
    sub_win->show();
}

void mafMainWindow::viewWillBeSelected() {
    qDebug() << "View will be selected!!";
}

void mafMainWindow::subWindowSelected(QMdiSubWindow *sub_win) {
    qDebug() << "View selected!!";
}


void mafMainWindow::synchronizeSplitters(QObject * sender)
{
        if(m_HorizontalSplitterTop->sizes()==m_HorizontalSplitterBottom->sizes() || !m_HorizontalSplitterTop->isVisible() || !m_HorizontalSplitterBottom->isVisible())
                return;

        mafSplitter* s = (mafSplitter*)sender;
        if (s==m_HorizontalSplitterTop) {
                m_HorizontalSplitterBottom->adjustPosition(m_HorizontalSplitterTop->sizes()[0]);
        }
        else if(s==m_HorizontalSplitterBottom) {
                m_HorizontalSplitterTop->adjustPosition(m_HorizontalSplitterBottom->sizes()[0]);
        };

}

mafInterpreter *mafMainWindow::interpreter(void)
{
    return m_Interpreter;
}

void mafMainWindow::showPreferences(void)
{
    mafInterpreterPreferencesWidget *widget = m_Interpreter->preferencesWidget(this);
    widget->setWindowFlags(Qt::Sheet);
    widget->show();
}

