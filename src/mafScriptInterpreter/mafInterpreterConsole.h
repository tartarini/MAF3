/*
 *  mafInterpreterConsole.h
 *  mafGui
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 10/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERPRETERCONSOLE_H
#define MAFINTERPRETERCONSOLE_H

#include "mafScriptInterpreter_global.h"
#include <mafTextEditor.h>

namespace mafScriptInterpreter {

class mafScriptEditor;

/**
    Class Name: mafInterpreterConsole
    Represents the widget in which will be logged all the output of the script editor.
*/
class MAFSCRIPTINTERPRETERSHARED_EXPORT mafInterpreterConsole : public mafGUI::mafTextEditor {
    Q_OBJECT

public Q_SLOTS:
    /// Key Command Event 
    void onKeyUpPressed(void);
    /// Key Command Event 
    void onKeyDownPressed(void);
    /// Key Command Event 
    void onKeyLeftPressed(void);
    /// Key Command Event 
    void onKeyRightPressed(void);
    /// Key Command Event 
    void onKeyEnterPressed(void);
    /// Key Command Event 
    void onKeyBackspacePressed(void);

    /// Return output
    void output(const QString& result,  int *stat);

Q_SIGNALS:
    /// Signal emitted when input is inserted.
    QString   input(const QString& command, int *stat);
    /// Signal emitted when load an external script.
    void    load(const QString& path);
    /// Signal emitted when save a script.
    void    save(const QString& path);
    /// Signal emitted when request help.
    void    help(void);
    /// Signal emitted when stopped.
    void stopped(void);

    
public:
    /// Object Constructor.
    mafInterpreterConsole(QWidget *parent = 0);
    /// Object Destructor.
    ~mafInterpreterConsole(void);
    
    /// Read settings for the interpreter.
    void  readSettings(void);
    
    /// Initialize signal/slot connections.
    void registerInterpreter(mafScriptEditor *interpreter);
    
    /// Apply a highlight filter to the text.
    QString filter(QString text);
    
    /// Return current script editor.
    mafScriptEditor *interpreter(void);

protected:
    /// Key Command Event 
    void keyPressEvent(QKeyEvent *event);
    /// Mouse Command Event 
    void mousePressEvent(QMouseEvent *event);
    /// Mouse Command Event 
    void mouseReleaseEvent(QMouseEvent *event);

private:
    mafScriptEditor *m_Interpreter; ///< script interpreter
    QTextCursor m_Cursor; ///< mouse cursor.
    
    QStringList  m_History; ///< history list.
    unsigned int m_HistoryIndex; ///< index of history list.
    bool         m_HistoryDirty; ///< flag for invalidating history.
    
};
    
} // namespace mafScriptInterpreter

#endif
