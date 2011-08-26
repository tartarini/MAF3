/*
 *  mafScriptEditor.cpp
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi on 08/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#include <iostream>

#ifdef HAVE_EDITLINE
#include <histedit.h>
#endif

#include "mafScriptEditor.h"
#include "mafScriptEditorConsole.h"

using namespace mafScriptInterpreter;
/**
Class Name: mafScriptEditorPrivate
Pimpl Pattern
*/
class mafScriptInterpreter::mafScriptEditorPrivate
{
public:
    QMap<QString, QString> variableDescription; /// Map for variable description
    QMap<QString, QString> functionDescription; /// Map for function description

#ifndef Q_OS_WIN32
    mafScriptEditorConsole *m_Console; ///< console variable
#endif

    int count; ///< reference count
};

mafScriptEditor::mafScriptEditor(QObject *parent) : QObject(parent), m_PrivateClassPointer(new mafScriptEditorPrivate)
{
    m_PrivateClassPointer->count = 1;

#ifndef Q_OS_WIN32
    m_PrivateClassPointer->m_Console = new mafScriptEditorConsole(this);

    connect(m_PrivateClassPointer->m_Console, SIGNAL(interpreted(const QString&, int *)), m_PrivateClassPointer->m_Console, SLOT(output(const QString&, int *)));
    connect(m_PrivateClassPointer->m_Console, SIGNAL(input(const QString&, int *)), this,    SLOT(interpret(const QString&, int *)));
    connect(m_PrivateClassPointer->m_Console, SIGNAL( load(const QString&)),        this,    SLOT(     load(const QString&)));
    connect(m_PrivateClassPointer->m_Console, SIGNAL( save(const QString&)),        this,    SLOT(     save(const QString&)));
    connect(m_PrivateClassPointer->m_Console, SIGNAL(stopped(void)),                this,  SIGNAL(  stopped(void)));
#endif
}

void mafScriptEditor::retain(void)
{
    m_PrivateClassPointer->count++;
}

void mafScriptEditor::release(void)
{
    if(!(--(m_PrivateClassPointer->count)))
	delete this;
}

QString mafScriptEditor::help(void) const
{
    QString message;

    message += "Functions:\n";
    foreach(QString name, m_PrivateClassPointer->functionDescription.keys())
	message += "\t" + name + ": " + m_PrivateClassPointer->functionDescription.value(name) + "\n";

    message += "Variables:\n";
    foreach(QString name, m_PrivateClassPointer->variableDescription.keys())
	message += "\t" + name + ": " + m_PrivateClassPointer->variableDescription.value(name) + "\n";

    return message;
}

void mafScriptEditor::load(const QString& file)
{
    int stat;
    QFile script(file);
    QString commands;

    script.open(QFile::ReadOnly);
    commands = QLatin1String(script.readAll());
    script.close() ;
    
    interpret(commands, &stat);
}

void mafScriptEditor::save(const QString& file)
{

}

void mafScriptEditor::registerPrompt(char *(*prompt)(void))
{
#ifndef Q_OS_WIN32
    m_PrivateClassPointer->m_Console->registerPrompt(prompt);
#endif
}

void mafScriptEditor::registerVariableDescription(QString name, QString description)
{
    m_PrivateClassPointer->variableDescription.insert(name, description);
}

void mafScriptEditor::registerFunctionDescription(QString name, QString description)
{
    m_PrivateClassPointer->functionDescription.insert(name, description);
}

void mafScriptEditor::unregisterVariableDescription(QString name)
{
    m_PrivateClassPointer->functionDescription.remove(name);
}

void mafScriptEditor::unregisterFunctionDescription(QString name)
{
    m_PrivateClassPointer->functionDescription.remove(name);
}

QString mafScriptEditor::prompt(void) const
{
#ifndef Q_OS_WIN32
    return m_PrivateClassPointer->m_Console->prompt();
#else
	return "interpreter:~$";
#endif
}

void mafScriptEditor::start(void)
{
#ifndef Q_OS_WIN32
	m_PrivateClassPointer->m_Console->start();
#endif
}

void mafScriptEditor::stop(void)
{
#ifndef Q_OS_WIN32
	m_PrivateClassPointer->m_Console->stop();
#endif
}

void mafScriptEditor::setVerbose(bool verbose)
{
#ifndef Q_OS_WIN32
    if(verbose)
	   connect(this, SIGNAL(interpreted(const QString&, int *)), m_PrivateClassPointer->m_Console, SLOT(output(const QString&, int *)));
    else
	disconnect(this, SIGNAL(interpreted(const QString&, int *)), m_PrivateClassPointer->m_Console, SLOT(output(const QString&, int *)));    
#endif
}

mafScriptEditor::~mafScriptEditor(void)
{
#ifndef Q_OS_WIN32
    m_PrivateClassPointer->m_Console->stop();
#endif

    delete m_PrivateClassPointer;
}
