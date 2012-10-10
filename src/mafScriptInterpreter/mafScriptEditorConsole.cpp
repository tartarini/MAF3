/*
 *  mafScriptEditorConsole.cpp
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi on 08/11.
 *  Copyright 2009 SCS-B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <iostream>

#ifdef HAVE_EDITLINE
#include <histedit.h>
#endif

#include "mafScriptEditorConsole.h"
#include "mafScriptEditor.h"

using namespace mafScriptInterpreter;

/**
Class Name: mafScriptEditorConsolePrivate
Pimpl Pattern.
*/
class mafScriptInterpreter::mafScriptEditorConsolePrivate
{
public:
    char *(*prompt)(void); ///< pointer function to the prompt

#ifdef HAVE_EDITLINE
    EditLine *m_ElEngine; /// used on unix for emacs style
    History  *m_ElHistory; /// used on unix for emacs style
    HistEvent m_ElEvent; /// used on unix for emacs style
#endif
};

mafScriptEditorConsole::mafScriptEditorConsole(QObject *parent) : QThread(parent), m_PrivateClassPointer(new mafScriptEditorConsolePrivate)
{

}

mafScriptEditorConsole::~mafScriptEditorConsole(void)
{
    delete m_PrivateClassPointer;

    m_PrivateClassPointer = NULL;
}

void mafScriptEditorConsole::registerPrompt(char *(*prompt)(void))
{
    m_PrivateClassPointer->prompt = prompt;
}

void mafScriptEditorConsole::registerBindings(QString style)
{
#ifdef HAVE_EDITLINE
    QByteArray ba = style.toAscii();
    el_set(m_PrivateClassPointer->m_ElEngine, EL_EDITOR, ba.constData());    
#endif
}

QString mafScriptEditorConsole::prompt(void) const
{
    return QString(m_PrivateClassPointer->prompt());
}

void mafScriptEditorConsole::start(Priority priority)
{
#ifdef HAVE_EDITLINE
    m_PrivateClassPointer->m_ElEngine = el_init("mafScriptEditor", stdin, stdout, stderr);
    el_set(m_PrivateClassPointer->m_ElEngine, EL_PROMPT, m_PrivateClassPointer->prompt);
    el_set(m_PrivateClassPointer->m_ElEngine, EL_EDITOR, "emacs");
    
    m_PrivateClassPointer->m_ElHistory = history_init();
    if (!m_PrivateClassPointer->m_ElHistory)
	qFatal("Could not initialize editline history");
    history(m_PrivateClassPointer->m_ElHistory, &(m_PrivateClassPointer->m_ElEvent), H_SETSIZE, 800);
    el_set(m_PrivateClassPointer->m_ElEngine, EL_HIST, history, m_PrivateClassPointer->m_ElHistory);

    QThread::start(priority);
#endif
}

void mafScriptEditorConsole::stop(void)
{
#ifdef HAVE_EDITLINE
    if(this->isRunning()) {
        std::cerr << "bye" << std::endl;
        if (m_PrivateClassPointer->m_ElHistory) {
            history_end(m_PrivateClassPointer->m_ElHistory);
            m_PrivateClassPointer->m_ElHistory = 0l;
        }
        if (m_PrivateClassPointer->m_ElEngine) {
            el_end(m_PrivateClassPointer->m_ElEngine);
            m_PrivateClassPointer->m_ElEngine = 0l;
        }
    }

    quit();
    wait(0);
#endif
}

void mafScriptEditorConsole::run(void)
{
#ifdef HAVE_EDITLINE
    int count;
    int stat;

    forever {

        QString line = QString(el_gets(m_PrivateClassPointer->m_ElEngine, &count)).simplified();
           
        if (line == "bye" || line == "exit" || line == "quit")
            break;
        
        if(line != "") {
            QByteArray ba = line.toAscii();
            history(m_PrivateClassPointer->m_ElHistory, &(m_PrivateClassPointer->m_ElEvent), H_ENTER, ba.constData());
        }

        mafScriptEditorSynchronizer::instance()->lock();

        if (line.startsWith(":load ")) {
            Q_EMIT load(line.remove(":load "));

        } else if (line.startsWith(":save ")) {
            Q_EMIT save(line.remove(":save "));

        } else if (line.startsWith(":emacs")) {
            std::cerr << "switching to emacs bindings" << std::endl;

            registerBindings("emacs");

            Q_EMIT input("", &stat);

        } else if (line.startsWith(":vi")) {
            std::cerr << "switching to vi bindings" << std::endl;

            registerBindings("vi");

            Q_EMIT input("", &stat);

        } else if (line.startsWith(":plugins")) {
            //mafPluginManager::instance()->printPlugins();

            //Q_EMIT input("", &stat);

        } else if (line.startsWith(":help")) {
            std::cerr << "Plugins:" << std::endl;
            std::cerr << " :plugins            prints the status of plugins" << std::endl;
            std::cerr << "" << std::endl;
            std::cerr << "File manipulation:" << std::endl;
            std::cerr << " :load [file]        loads file and interprets its content" << std::endl;
            std::cerr << " :save [file]        saves interpreter history to file" << std::endl;
            std::cerr << "" << std::endl;
            std::cerr << "Keyboard bindings:" << std::endl;
            std::cerr << " :emacs              sets up emacs key bindings" << std::endl;
            std::cerr << " :vi                 sets up vi    key bindings" << std::endl;
            std::cerr << "" << std::endl;
            std::cerr << "Documentation:" << std::endl;
            std::cerr << " :man [page]         displays documentation for page" << std::endl;
            std::cerr << "" << std::endl;

            Q_EMIT input("", &stat);

        } else if (line.startsWith(":man ")) {
            
#if defined(Q_OS_MAC) || defined(Q_OS_UNIX)
            QString program = "man";
        
            QStringList arguments;
            arguments << "-P";
            arguments << "cat";
            arguments << "-M";
            arguments << "../doc/man";
            arguments << line.remove(":man ");

            stat = mafScriptEditor::Status_Ok;
        
            QProcess process;	    
            process.start(program, arguments);
            process.waitForFinished(-1);

            Q_EMIT output(QString(process.readAll()), &stat);
#endif

            Q_EMIT input("", &stat);

        } else if (count > 0) {
            
            Q_EMIT input(line, &stat);

        }

        mafScriptEditorSynchronizer::instance()->wait();
        mafScriptEditorSynchronizer::instance()->unlock();
    }

    if (m_PrivateClassPointer->m_ElHistory) {
        history_end(m_PrivateClassPointer->m_ElHistory);
        m_PrivateClassPointer->m_ElHistory = 0l;
    }
    if (m_PrivateClassPointer->m_ElEngine) {
        el_end(m_PrivateClassPointer->m_ElEngine);
        m_PrivateClassPointer->m_ElEngine = 0l;
    }

    Q_EMIT stopped();
#endif
}

#include <iostream>

void mafScriptEditorConsole::output(const QString& result, int *stat)
{
    if(!result.simplified().isEmpty()) {
        QByteArray ba = result.toAscii();
	    std::cout << (*stat == mafScriptEditor::Status_Ok ? MAF_NO_COLOR : MAF_COLOR_FG_RED) << ba.constData() << MAF_NO_COLOR << std::endl;
    }
}
