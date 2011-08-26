/* mafScriptEditor.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Wed Nov 26 16:10:31 2008 (+0100)
 * Version: $Id$
 * Last-Updated: Sun Aug  2 11:05:12 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 176
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFSCRIPTINTERPRETER_H
#define MAFSCRIPTINTERPRETER_H

#include "mafScriptInterpreter_global.h"
#include "mafScriptInterpreterDefinitions.h"

namespace mafScriptInterpreter {

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorSynchronizer
{
public:
    static mafScriptEditorSynchronizer *instance(void);

    void lock(void);
    void unlock(void);
    
    void wait(void);
    void wake(void);

protected:
     mafScriptEditorSynchronizer(void);
    ~mafScriptEditorSynchronizer(void);

private:
    static mafScriptEditorSynchronizer *m_instance;

private:
    QMutex         *mutex;
    QWaitCondition *condition;
};

// /////////////////////////////////////////////////////////////////
// mafScriptEditorConsole
// /////////////////////////////////////////////////////////////////

class mafScriptEditorConsolePrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorConsole : public QThread
{
    Q_OBJECT

public:
     mafScriptEditorConsole(QObject *parent);
    ~mafScriptEditorConsole(void);

    void registerPrompt(char *(*prompt)(void));
    void registerBindings(QString style = "emacs");

    void run(void);

    QString prompt(void) const;

public slots:
    void start(Priority priority = InheritPriority);
    void stop(void);

    void output(const QString& result,  int *stat);

signals:
    void   input(const QString& command, int *stat);
    void    load(const QString& path);
    void    save(const QString& path);
    void    help(void);
    void stopped(void);

private:
    mafScriptEditorConsolePrivate *d;
};

// /////////////////////////////////////////////////////////////////
// mafScriptEditor
// /////////////////////////////////////////////////////////////////

class mafAbstractData;
class mafAbstractProcess;
class mafAbstractView;

class mafScriptEditorPrivate;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditor : public QObject
{
    Q_OBJECT

public:
             mafScriptEditor(QObject *parent = 0);
    virtual ~mafScriptEditor(void);

    enum Status {
        Status_Ok,
        Status_Error,
        Status_Return,
        Status_Break,
        Status_Continue
    };

    void retain(void);
    void release(void);

    void registerPrompt(char *(*prompt)(void));

    virtual void registerVariable(bool   &var, QString name, QString description = "") = 0;
    virtual void registerVariable(int    &var, QString name, QString description = "") = 0;
    virtual void registerVariable(double &var, QString name, QString description = "") = 0;
    virtual void registerVariable(char * &var, QString name, QString description = "") = 0;

    virtual void unregisterVariable(QString name) = 0;

    void setVerbose(bool verbose);

    QString prompt(void) const;

public slots:
    void load(const QString& file);
    void save(const QString& file);

    void start(void);
    void stop(void);

    virtual QString interpret(const QString& command, int *stat) = 0;
    virtual QString interpret(const QString& command, const QStringList& args, int *stat) = 0;

    QString help(void) const;

signals:
    void stopped(void);

protected:
    void registerVariableDescription(QString name, QString description);
    void registerFunctionDescription(QString name, QString description);

    void unregisterVariableDescription(QString name);
    void unregisterFunctionDescription(QString name);

private:
    mafScriptEditorPrivate *d;
};
    
} // end namespace

#endif
