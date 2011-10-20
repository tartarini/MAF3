/*
 *  mafCodecXML.h
 *  mafSerialization
 *
 *  Created by Daniele Giunchi on 08/11.
 *  Copyright 2009 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */
 
#ifndef MAFSCRIPTEDITORCONSOLE_H
#define MAFSCRIPTEDITORCONSOLE_H

#include "mafScriptInterpreter_global.h"

namespace mafScriptInterpreter {

class mafScriptEditorConsolePrivate;

/**
Class name: mafScriptEditorConsole
This class represents the console used by the editor.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafScriptEditorConsole : public QThread {
    Q_OBJECT

public:
    /// Object constructor.
     mafScriptEditorConsole(QObject *parent);
    /// Object Destructor.
    ~mafScriptEditorConsole(void);

    /// set the prompt.
    void registerPrompt(char *(*prompt)(void));
    /// Register styles.
    void registerBindings(QString style = "emacs");
    /// Run the console in loop.
    void run(void);

    /// Return the prompt
    QString prompt(void) const;

public Q_SLOTS:
    /// Start the console.
    void start(Priority priority = InheritPriority);
    /// Stop the console.
    void stop(void);
    /// Return the output in result string.
    void output(const QString& result,  int *stat);

Q_SIGNALS:
    /// signal used when insert an input
    void   input(const QString& command, int *stat);
    /// signal used when load a file
    bool    load(const QString& path);
    /// signal used when save a file
    void    save(const QString& path);
    /// signal used when request help
    void    help(void);
    /// signal used when stop console
    void stopped(void);

private:
    mafScriptEditorConsolePrivate *m_PrivateClassPointer; /// pimpl pattern
};

}  // end namespace

#endif