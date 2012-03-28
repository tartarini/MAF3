/*
 *  mafScriptEditor.h
 *  mafScriptInterpreter
 *
 *  Created by Daniele Giunchi - Paolo Quadrani on 08/11.
 *  Copyright 2012 B3C. All rights reserved.
 *
 *  See License at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFSCRIPTEDITOR_H
#define MAFSCRIPTEDITOR_H

#include "mafScriptInterpreter_global.h"
#include <mafObjectBase.h>

namespace mafScriptInterpreter {

class mafScriptEditorPrivate;

/**
Class name: mafScriptEditor
Base class for script editor in a specific language.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditor : public mafCore::mafObjectBase {
    Q_OBJECT
    /// typedef macro.
    mafSuperclassMacro(mafCore::mafObjectBase);

public Q_SLOTS:
    /// Slot for load script.
    bool load(const QString& file);
    /// Slot for save script.
    void save(const QString& file);

    /// Start the console.
    void start(void);

    /// Stop the console.
    void stop(void);

    /// Setup the text highlighter.
    virtual void setupHighliter(QTextDocument *doc) = 0;

    /// Interpret the string after click enter.
    virtual QString interpret(const QString& command, int *stat) = 0;

    /// Interpret the string after click enter using a parameter list.
    virtual QString interpret(const QString& command, const QStringList& args, int *stat) = 0;

    /// Invoke help to display.
    QString help(void) const;

Q_SIGNALS:
    /// Signal launched after stopping the console.
    void stopped(void);
    
public:
    /// Object constructor.
    mafScriptEditor(const QString code_location = "");

    /** Status representation after each interpretation. */
    enum Status {
        Status_Ok,
        Status_Error,
        Status_Return,
        Status_Break,
        Status_Continue
    };

    /// Enable prompt of the console.
    void registerPrompt(char *(*prompt)(void));

    /// Register a boolean type variable with value, name and a description.
    virtual void registerVariable(bool   &var, QString name, QString description = "") = 0;
    /// Register a integer type variable with value, name and a description.
    virtual void registerVariable(int    &var, QString name, QString description = "") = 0;
    /// Register a double type variable with value, name and a description.
    virtual void registerVariable(double &var, QString name, QString description = "") = 0;
    /// Register a char type variable with value, name and a description.
    virtual void registerVariable(char * &var, QString name, QString description = "") = 0;
    /// Register a QObject type variable with value, name and a description.
    virtual void registerVariable(QObject &var, QString name, QString description = "") = 0;

    /// Unregister a variable.
    virtual void unregisterVariable(QString name) = 0;

    /// set verbosity.
    void setVerbose(bool verbose);

    /// Return current prompt.
    QString prompt(void) const;

protected:
    /// Object destructor.
    /*virtual*/ ~mafScriptEditor(void);

    /// Register a variable using a name and a description.
    void registerVariableDescription(QString name, QString description);
    /// Register a function using a name and a description.
    void registerFunctionDescription(QString name, QString description);

    /// Unregister a variable using a name and a description.
    void unregisterVariableDescription(QString name);
    /// Unregister a variable using a name and a description.
    void unregisterFunctionDescription(QString name);

private:
    mafScriptEditorPrivate *m_PrivateClassPointer; /// Pimpl pattern.
};
    
} // end namespace

#endif
