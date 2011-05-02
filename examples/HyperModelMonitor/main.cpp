#include <QtGui/QApplication>

#include "mafMainWindow.h"
#include "mafOperationSimpleApp.h"
#include "mafApplicationSettingsPageConfigurations.h"

#include <mafLogic.h>

#define HAVE_SWIG
#define HAVE_PYTHON

#include <mafScriptEditor.h>
#include <mafScriptEditorPython.h>
#include <mafInterpreter.h>
#include <mafInterpreterPreferencesWidget.h>

using namespace mafScriptInterpreter;

// /////////////////////////////////////////////////////////////////
// log message handler
// /////////////////////////////////////////////////////////////////

mafInterpreter *log_output;

void tstRedirectLogHandler(mafLog::Level level, const QString& msg)
{
    char *v = msg.toAscii().data();
    QCoreApplication::postEvent(log_output, new mafLogEvent(level, msg));
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("SCS");
    a.setOrganizationDomain("scsolutions.org");
    a.setApplicationName("HypermodelMonitor");
    a.setApplicationVersion("1.0");

    // Create the application's logic instance
    mafApplicationLogic::mafLogic *logic = new mafApplicationLogic::mafLogic();
    // and initialize it. This initialization will load dinamically the mafResources Library.
    bool ok = logic->initialize();
    if(!ok) {
        exit(1);
    }

    // Plug into the factory the new operation. This operation has also
    // an acceptObject method defined, so call macro below.
    // If the plugged object hasn't the acceptObject defined, the registration
    // to the mafObjectFactory can be done using the following macro:
    // mafRegisterObject(myClassCustom);
    mafRegisterObjectAndAcceptBind(mafOperationSimpleApp);
    // Plug the object's information into the framework

    // Create the instance of the main window and pass to it the application's logic.
    // In this way the mafMainWondow class will also load the plug-ins present
    // in the default 'plugins' directory.
    mafMainWindow w(logic);

    // plug custom application's setting page
    mafApplicationSettingsPageConfigurations *page = new mafApplicationSettingsPageConfigurations();
    w.plugApplicationSettingsPage(page);
    
    // Eventually call the loadPlugins method with custom directory to allow the application
    // load custom plugins located in custom directories.
    //logic->loadPlugins(cusom_plugin_path);
    
    //interpreter
    mafScriptEditor *interpreter = NULL;
    interpreter = new mafScriptEditorPython;
    
    if(!interpreter) {
        qDebug() << "No interpreter available. Is BUILD_WRAPPERS enabled ?";
        return 0;
    }
    
    log_output = w.interpreter();
    w.interpreter()->registerInterpreter(interpreter);
    w.interpreter()->registerAsHandler(tstRedirectLogHandler);
    log_output->readSettings();
        
    QObject::connect(interpreter, SIGNAL(stopped()), &a, SLOT(quit()));
	QObject::connect(&a, SIGNAL(aboutToQuit()), interpreter, SLOT(stop()));
    
    // plug custom application's setting page
    mafInterpreterPreferencesWidget *pageInterpreter = new mafInterpreterPreferencesWidget(log_output);
    w.plugApplicationSettingsPage(pageInterpreter);

    w.setupMainWindow();
    
    int result = a.exec();

    mafDEL(logic);
    
    log_output->writeSettings();
    return result;
}
