/*=============================================================================

  Library: MAF-CTK

  Copyright (c) German Cancer Research Center,
    Division of Medical and Biological Informatics

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

=============================================================================*/

// Qt includes
#include <QtPlugin>
#include <QRect>
#include <QDebug>
#include <QPushButton>
#include <QApplication>
#include <QLabel>
#include <QRect>
#include <QStringList>
#include <QDir>
#include <QTemporaryFile>
#include <QPainter>


// CTK includes
#include "ctkDICOMImage.h"
#include "mafCTKExampleDicomAppLogic_p.h"
#include "mafCTKExampleDicomAppPlugin_p.h"

// DCMTK includes
#include <dcmimage.h>

// MAF includes
#include <mafVME.h>
#include <mafDataSet.h>

using namespace mafEventBus;
using namespace ctkDicomAppHosting;

//----------------------------------------------------------------------------
mafCTKExampleDicomAppLogic::mafCTKExampleDicomAppLogic():
    ctkDicomAbstractApp(mafCTKExampleDicomAppPlugin::getPluginContext()), AppWidget(0), Logic(0)
{
  connect(this, SIGNAL(startProgress()), this, SLOT(onStartProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(resumeProgress()), this, SLOT(onResumeProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(suspendProgress()), this, SLOT(onSuspendProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(cancelProgress()), this, SLOT(onCancelProgress()), Qt::QueuedConnection);
  connect(this, SIGNAL(exitHostedApp()), this, SLOT(onExitHostedApp()), Qt::QueuedConnection);

  //notify Host we are ready.
  try {
    getHostInterface()->notifyStateChanged(ctkDicomAppHosting::IDLE);
  }
  catch(...)
  {
    qDebug() << "ctkDicomAbstractApp: Could not getHostInterface()";
  }
}

//----------------------------------------------------------------------------
mafCTKExampleDicomAppLogic::~mafCTKExampleDicomAppLogic() {
    ctkPluginContext* context = mafCTKExampleDicomAppPlugin::getPluginContext();
    QList <QSharedPointer<ctkPlugin> > plugins = context->getPlugins();
    for (int i = 0; i < plugins.size(); ++i) {
        qDebug() << plugins.at(i)->getSymbolicName ();
    }
}

//----------------------------------------------------------------------------
bool mafCTKExampleDicomAppLogic::bringToFront(const QRect& requestedScreenArea) {
    if(this->AppWidget!=NULL) {
        this->AppWidget->move(requestedScreenArea.topLeft());
        this->AppWidget->resize(requestedScreenArea.size());
        this->AppWidget->activateWindow();
        this->AppWidget->raise();
    }
    return true;
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::do_something() {
    Logic = new mafApplicationLogic::mafLogic();
      // and initialize it. This initialization will load dynamically the mafResources Library.
    Logic->setApplicationName(qApp->applicationName());

      /// push libraries to load during initialization.
    Logic->pushLibraryToLoad("mafResources");
    Logic->pushLibraryToLoad("mafSerialization");

      // and initialize it. This initialization will load dynamically the mafResources Library.
    bool ok = Logic->initialize();
    if(!ok) {
        qDebug() << "Problem on Initializing Logic!";
        exit(1);
    }


    /////// Customize Views and Visual Pipes ////
    Logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "View MIP");
    Logic->customizeVisualization("View MIP", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    Logic->customizeVisualization("View MIP", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKMIPVolume");

    Logic->plugObject("mafResources::mafView", "mafPluginVTK::mafViewVTK", "View Iso");
    Logic->customizeVisualization("View Iso", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");
    Logic->customizeVisualization("View Iso", "vtkStructuredPoints", "mafPluginVTK::mafPipeVisualVTKIsoSurface");
    Logic->customizeVisualization("VTK view", "vtkPolyData", "mafPluginVTK::mafPipeVisualVTKSurface");


    // creation of the maf GUI (with gui manager inside)
    AppWidget = new mafMainWindow(Logic);
    Logic->loadPlugins(); //need to be present the gui manager for connecting register library signals with gui managers slots

    // create a dialog for handle application hosting (can be embedded inside the application)
    this->Dialog = new QDialog(AppWidget);
    ui.setupUi(Dialog);

    connect(ui.LoadDataButton, SIGNAL(clicked()), this, SLOT(onLoadDataClicked()));
    connect(ui.CreateSecondaryCaptureButton, SIGNAL(clicked()), this, SLOT(onCreateSecondaryCapture()));
    try {
        QRect preferred(100,100,100,100);
        qDebug() << "  Asking:getAvailableScreen";
        QRect rect = getHostInterface()->getAvailableScreen(preferred);
        qDebug() << "  got sth:" << rect.top();
        this->Dialog->move(rect.topLeft());
        this->Dialog->resize(rect.size());
    }
    catch (const std::runtime_error& e) {
        qCritical() << e.what();
        return;
    }

    // visualization of the QMainWindow
    AppWidget->setupMainWindow();

    // visualization of the dialog
    //this->Dialog->setModal(true);
    this->Dialog->topLevelWidget();
    this->Dialog->show();

    // resize immediately
    AppWidget->resize(800,600);
    this->Dialog->resize(600,400);
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::onStartProgress() {
    setInternalState(ctkDicomAppHosting::INPROGRESS);

    // we need to create the button before we receive data from
    // the host, which happens immediately after calling
    // getHostInterface()->notifyStateChanged
    do_something();

    getHostInterface()->notifyStateChanged(ctkDicomAppHosting::INPROGRESS);
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::onResumeProgress() {
    //reclame all resources.

    //notify state changed
    setInternalState(ctkDicomAppHosting::INPROGRESS);
    getHostInterface()->notifyStateChanged(ctkDicomAppHosting::INPROGRESS);
    //we're rolling
    //do something else normally, but this is an example
    ui.LoadDataButton->setEnabled(true);
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::onSuspendProgress() {
    //release resources it can reclame later to resume work
    ui.LoadDataButton->setEnabled(false);
    //notify state changed
    setInternalState(ctkDicomAppHosting::SUSPENDED);
    getHostInterface()->notifyStateChanged(ctkDicomAppHosting::SUSPENDED);
    //we're rolling
    //do something else normally, but this is an example
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::onCancelProgress() {
    //release all resources
    onReleaseResources();
    //update state
    setInternalState(ctkDicomAppHosting::IDLE);
    getHostInterface()->notifyStateChanged(ctkDicomAppHosting::IDLE);
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::onExitHostedApp() {
    //useless move, but correct:
    //setInternalState(ctkDicomAppHosting::EXIT);
    //getHostInterface()->notifyStateChanged(ctkDicomAppHosting::EXIT);
    qDebug() << "Exiting";
    //die
    qApp->exit(0);
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::onReleaseResources() {
    this->AppWidget->hide();
    delete (this->AppWidget);
    this->AppWidget = 0 ;
}

//----------------------------------------------------------------------------
bool mafCTKExampleDicomAppLogic::notifyDataAvailable(const ctkDicomAppHosting::AvailableData& data, bool lastData) {
    Q_UNUSED(lastData)
    QString s;
    if(this->AppWidget == 0) {
        qCritical() << "Button is null!";
        return false;
    }
    s = "Received notifyDataAvailable with patients.count()= " + QString().setNum(data.patients.count());
    if(data.patients.count()>0) {
        s=s+" name:"+data.patients.begin()->name+" studies.count(): "+QString().setNum(data.patients.begin()->studies.count());
        if(data.patients.begin()->studies.count()>0) {
            s=s+" series.count():" + QString().setNum(data.patients.begin()->studies.begin()->series.count());
            if(data.patients.begin()->studies.begin()->series.count()>0) {
                s=s+" uid:" + data.patients.begin()->studies.begin()->series.begin()->seriesUID;
      //        QUuid uuid("93097dc1-caf9-43a3-a814-51a57f8d861d");//data.patients.begin()->studies.begin()->series.begin()->seriesUID);
                uuid = data.patients.begin()->studies.begin()->series.begin()->objectDescriptors.begin()->descriptorUUID;
                s=s+" uuid:"+uuid.toString();
            }
        }
    }
    ui.ReceivedDataInformation->setText(s);
    ui.LoadDataButton->setEnabled(true);
    return false;
}

//----------------------------------------------------------------------------
QList<ctkDicomAppHosting::ObjectLocator> mafCTKExampleDicomAppLogic::getData(
  const QList<QUuid>& objectUUIDs,
  const QList<QString>& acceptableTransferSyntaxUIDs,
  bool includeBulkData) {
    Q_UNUSED(objectUUIDs)
    Q_UNUSED(acceptableTransferSyntaxUIDs)
    Q_UNUSED(includeBulkData)
    return QList<ctkDicomAppHosting::ObjectLocator>();
}

//----------------------------------------------------------------------------
void mafCTKExampleDicomAppLogic::releaseData(const QList<QUuid>& objectUUIDs) {
    Q_UNUSED(objectUUIDs)
}

void mafCTKExampleDicomAppLogic::onLoadDataClicked() {
    QList<QUuid> uuidlist;
    uuidlist.append(uuid);
    QString transfersyntax("1.2.840.10008.1.2.1");
    QList<QUuid> transfersyntaxlist;
    transfersyntaxlist.append(transfersyntax);
    QList<ctkDicomAppHosting::ObjectLocator> locators;
    locators = getHostInterface()->getData(uuidlist, transfersyntaxlist, false);
    qDebug() << "got locators! " << QString().setNum(locators.count());

    QString s;
    s=s+" loc.count:"+QString().setNum(locators.count());
    if(locators.count()>0) {
        s=s+" URI: "+locators.begin()->URI +" locatorUUID: "+locators.begin()->locator+" sourceUUID: "+locators.begin()->source;
        qDebug() << "URI: " << locators.begin()->URI;
        QString filename = locators.begin()->URI;
            if(filename.startsWith("file:/",Qt::CaseInsensitive)) {
                filename=filename.remove(0,8);
            }
        qDebug()<<filename;
        if(QFileInfo(filename).exists()) {
            try {
                DicomImage dcmtkImage(filename.toLatin1().data());
                ctkDICOMImage ctkImage(&dcmtkImage);

                QPixmap pixmap = QPixmap::fromImage(ctkImage.frame(0),Qt::AvoidDither);
                if (pixmap.isNull()) {
                    qCritical() << "Failed to convert QImage to QPixmap" ;
                } else {
                    ui.PlaceHolderForImage->setPixmap(pixmap);
                }
            } catch(...) {
                qCritical() << "Caught exception while trying to load file" << filename;
            }
        } else {
            qCritical() << "File does not exist: " << filename;
        }
    }
    ui.ReceivedDataInformation->setText(s);
}

void mafCTKExampleDicomAppLogic::onCreateSecondaryCapture() {
    //delete (this->Dialog);

    mafResources::mafVME *vme = mafNEW(mafResources::mafVME);
    vme->setObjectName("DAH_VME_Test");
    mafResources::mafDataSet *dataset = mafNEW(mafResources::mafDataSet);
    dataset->setBoundaryAlgorithm(NULL);
    vme->dataSetCollection()->insertItem(dataset, 0);

    mafCore::mafObjectBase *toSend = vme;
    mafEventBus::mafEventArgumentsList argList;
    argList.append(mafEventArgument(mafCore::mafObjectBase *, toSend));
    mafEventBusManager::instance()->notifyEvent("maf.local.resources.vme.add", mafEventTypeLocal, &argList);


  /*const QPixmap* pixmap = ui.PlaceHolderForImage->pixmap();
  if(pixmap!=NULL)
  {
    QStringList preferredProtocols;
    preferredProtocols.append("file:");
    QString outputlocation = getHostInterface()->getOutputLocation(preferredProtocols);
    QString templatefilename = QDir(outputlocation).absolutePath();
    if(templatefilename.isEmpty()==false) templatefilename.append('/'); 
    templatefilename.append("ctkdahscXXXXXX.png");
    QTemporaryFile *tempfile = new QTemporaryFile(templatefilename,this->AppWidget);

    if(tempfile->open())
    {
      QString filename = QFileInfo(tempfile->fileName()).absoluteFilePath();
      qDebug() << "Created file: " << filename;
      tempfile->close();
      QPixmap tmppixmap(*pixmap);
      QPainter painter(&tmppixmap);
      painter.setPen(Qt::white);
      painter.setFont(QFont("Arial", 15));
      painter.drawText(tmppixmap.rect(),Qt::AlignBottom|Qt::AlignLeft,"Secondary capture by mafCTKExampleDicomApp");
     //painter.drawText(rect(), Qt::AlignCenter, "Qt");
      tmppixmap.save(tempfile->fileName(), "PNG");
    }
    else
      qDebug() << "Creating temporary file failed.";
  }*/

}
