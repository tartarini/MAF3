#include "mafMainWindow.h"
#include <QtGui>
#include <QVTKWidget.h>
#include "ui_mafMainWindow.h"

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

mafMainWindow::mafMainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::mafMainWindow), m_SettingsFilename("") {
    ui->setupUi(this);

    createActions();
    createMenus();
    createToolBars();

    ui->statusBar->showMessage(mafTr("Ready!"));
    ui->centralWidget->setLayout(ui->gridLayout);

    setUnifiedTitleAndToolBarOnMac(true);
}

mafMainWindow::~mafMainWindow() {
    delete ui;
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

void mafMainWindow::createActions() {
    newAct = new QAction(QIcon(":/images/new.png"), mafTr("&New"), this);
    newAct->setIconText(mafTr("New"));
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(mafTr("Create a new file"));
//    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), mafTr("&Open..."), this);
    openAct->setIconText(mafTr("Open"));
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(mafTr("Open an existing file"));
//    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), mafTr("&Save"), this);
    saveAct->setIconText(mafTr("Save"));
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(mafTr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(mafTr("Save &As..."), this);
    saveAsAct->setIconText(mafTr("Save As"));
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(mafTr("Save the document under a new name"));
//    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(mafTr("E&xit"), this);
    exitAct->setIconText(mafTr("Exit"));
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(mafTr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/cut.png"), mafTr("Cu&t"), this);
    cutAct->setIconText(mafTr("Cut"));
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(mafTr("Cut the current selection's contents to the "
                            "clipboard"));
//    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), mafTr("&Copy"), this);
    copyAct->setIconText(mafTr("Copy"));
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(mafTr("Copy the current selection's contents to the "
                             "clipboard"));
//    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setIconText(mafTr("Paste"));
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
//    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setIconText(mafTr("About"));
    aboutAct->setStatusTip(tr("Show the application's About box"));
//    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}

void mafMainWindow::createMenus() {
    fileMenu = ui->menuBar->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    ui->menuBar->addSeparator();

    editMenu = ui->menuBar->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    ui->menuBar->addSeparator();

    helpMenu = ui->menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);

    //connect(ui->actionAdd_View, SIGNAL(triggered()), this, SLOT(createViewWindow()));
}

void mafMainWindow::createToolBars() {
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
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
    widget->setParent(sub_win);
    widget->setWindowTitle(mafTr("mafView %1").arg(windowCounter++));
    widget->GetRenderWindow()->AddRenderer(renderer);
    sub_win->setMinimumSize(200, 200);

    sub_win->show();
}
