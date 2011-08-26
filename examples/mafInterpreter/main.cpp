/* main.cpp --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Sat Apr 11 13:49:30 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Thu Oct 14 21:29:07 2010 (+0200)
 *           By: Julien Wintz
 *     Update #: 43
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#include <QtGui/QApplication>

#include "anyoption.h"

#include "tstMainWindow.h"

#include <mafScriptEditor.h>

#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
#include <mafScriptEditorPython.h>
#endif

#include <mafInterpreterConsole.h>

using namespace mafGUI;
using namespace mafScriptInterpreter;

// /////////////////////////////////////////////////////////////////
// log message handler
// /////////////////////////////////////////////////////////////////

QWidget *log_output;

void tstRedirectLogHandler(mafLog::LogType logType, const QString& msg)
{
    QCoreApplication::postEvent(log_output, new mafLogEvent(logType, msg));
}

// /////////////////////////////////////////////////////////////////
// 
// /////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    AnyOption options;
    options.addUsage("Usage: ./mafInterpreterConsole [FLAG] ... [OPTION=VALUE] ...");
    options.addUsage("");
    options.addUsage("Mode:");
    options.addUsage(" -c  --console");
    options.addUsage("");
    options.addUsage("Script:");
    options.addUsage(" -i  --interpreter");
    options.addUsage("     --interpreter=python Run python  gui interpreter");
    options.setFlag("console", 'c');
    options.setOption("interpreter", 'i');
    options.processCommandArgs(argc, argv);

    tstMainWindow window;

    if (options.getValue("interpreter") != NULL || options.getValue('i') != NULL) {

        mafScriptEditor *interpreter = NULL;

#if defined(HAVE_SWIG) && defined(HAVE_PYTHON)
        if(QString(options.getValue('i')) == "python")
            interpreter = new mafScriptEditorPython;
#endif


        if(!interpreter) {
            qDebug() << "No interpreter available. Is BUILD_WRAPPERS enabled ?";
            return 0;
        }

        if(!options.getFlag("console") || !options.getFlag('c')) {

            log_output = window.interpreter();

            window.interpreter()->registerInterpreter(interpreter);
            window.interpreter()->registerAsHandler(tstRedirectLogHandler);
            window.show();
        } else {
            interpreter->start();
        }

        QObject::connect(interpreter, SIGNAL(stopped()), &application, SLOT(quit()));
	QObject::connect(&application, SIGNAL(aboutToQuit()), interpreter, SLOT(stop()));
    } else {
        options.printUsage();
        return 0;
    }

    return application.exec();
}
