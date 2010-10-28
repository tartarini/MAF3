#include "mafMainWindow.h"
#include "ui_mafMainWindow.h"

#include "googlechat.h"

#include <mafGUIRegistration.h>

#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSphereSource.h>
#include <vtkSmartPointer.h>

#ifdef WIN32
    #define SETTINGS_EXTENSION ".ini"
#else
    #ifdef __APPLE__
        #define SETTINGS_EXTENSION ".plist"
    #else
        #define SETTINGS_EXTENSION ".ini"
    #endif
#endif

using namespace mafCore;
using namespace mafGUI;

mafMainWindow::mafMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mafMainWindow), m_SettingsFilename("") {
    ui->setupUi(this);

    mafCoreSingletons::mafSingletonsInitialize();

    mafGUIRegistration::registerGUIObjects();
    m_GUIManager = mafNEW(mafGUI::mafGUIManager);

    googleChat = new GoogleChat();

    m_GUIManager->createActions(this);
    m_GUIManager->createMenus(this);
    m_GUIManager->createToolBars(this);

    connectCallbacks();

    ui->statusBar->showMessage(mafTr("Ready!"));
    ui->centralWidget->setLayout(ui->gridLayout);

    connect(ui->mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this, SLOT(viewSelected(QMdiSubWindow*)));

    setUnifiedTitleAndToolBarOnMac(true);
}

void mafMainWindow::connectCallbacks() {
    mafRegisterLocalCallback("maf.local.gui.action.new", this, "createViewWindow()");
    mafRegisterLocalCallback("maf.local.gui.action.collaborate", this, "openGoogleTalk()");
    mafRegisterLocalCallback("maf.local.gui.action.save", this, "save()");
}

mafMainWindow::~mafMainWindow() {
    mafDEL(m_GUIManager);
    delete googleChat;
    delete ui;
}

mafCore::mafMemento *mafMainWindow::createMemento() const {
    return new mafMementoApplication(this, mafCodeLocation);
}

void mafMainWindow::mousePressEvent( QMouseEvent * event ) {
    mafMsgDebug() << "Mouse pressed....";
}

void mafMainWindow::moveEvent ( QMoveEvent * event ) {
    mafMsgDebug() << "Mouse moving....";
}

void mafMainWindow::mouseReleaseEvent ( QMouseEvent * event ) {
    mafMsgDebug() << "Mouse released....";
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
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void mafMainWindow::readSettings() {
    if(m_SettingsFilename.isEmpty()) {
        return;
    }
    mafString settings_fullname = m_SettingsFilename;
    settings_fullname.append(SETTINGS_EXTENSION);
    mafSettings settings(settings_fullname,
                    QSettings::NativeFormat);
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(600, 400)).toSize();
    resize(size);
    move(pos);
}

void mafMainWindow::writeSettings() {
    if(m_SettingsFilename.isEmpty()) {
        return;
    }
    mafString settings_fullname = m_SettingsFilename;
    settings_fullname.append(SETTINGS_EXTENSION);
    mafSettings settings(settings_fullname,
                    QSettings::NativeFormat);
    settings.setValue("pos", pos());
    settings.setValue("size", size());
}

bool mafMainWindow::maybeSave() {
    if (true) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

bool mafMainWindow::save() {
    return true;
}

void mafMainWindow::openGoogleTalk() {
    QMdiSubWindow *sub_win = ui->mdiArea->addSubWindow(googleChat);
    //googleChat->show();
    sub_win->show();
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

    sub_win->show();
}

void mafMainWindow::viewWillBeSelected() {
    mafMsgDebug() << "View will be selected!!";
}

void mafMainWindow::viewSelected(QMdiSubWindow *sub_win) {
    mafMsgDebug() << "View selected!!";
}
