/*
 *  mafInterpreterEditor.h
 *  mafGui
 *
 *  Created by Paolo Quadrani - Daniele Giunchi on 10/06/11.
 *  Copyright 2011 B3C. All rights reserved.
 *
 *  See Licence at: http://tiny.cc/QXJ4D
 *
 */

#ifndef MAFINTERPRETEREDITOR_H
#define MAFINTERPRETEREDITOR_H

#include "mafScriptInterpreter_global.h"
#include "mafLog.h"

#include <mafTextEditor.h>

namespace mafScriptInterpreter {

class mafScriptEditor;
class mafInterpreterPreferencesWidget;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafInterpreterEditor : public mafGUI::mafTextEditor
{
    Q_OBJECT

public:
     mafInterpreterEditor(QWidget *parent = 0);
    ~mafInterpreterEditor(void);

    void  readSettings(void);
    void writeSettings(void);

    void registerInterpreter(mafScriptEditor *interpreter);
    void registerAsHandler(mafGUI::mafLog::Handler handler);

    QString filter(QString text);

    mafInterpreterPreferencesWidget *preferencesWidget(QWidget *parent = 0);

    mafScriptEditor *interpreter(void);

public slots:
    void onKeyUpPressed(void);
    void onKeyDownPressed(void);
    void onKeyLeftPressed(void);
    void onKeyRightPressed(void);
    void onKeyEnterPressed(void);
    void onKeyBackspacePressed(void);

    void output(const QString& result,  int *stat);

signals:
    void   input(const QString& command, int *stat);
    void    load(const QString& path);
    void    save(const QString& path);
    void    help(void);
    void stopped(void);

protected:
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    bool eventFilter(QObject *object, QEvent *event);

private:
    mafScriptEditor *m_Interpreter;
    mafInterpreterPreferencesWidget *preferences;
    
    QTextCursor cursor;
    
    QStringList  history;
    unsigned int history_index;
    bool         history_dirty;
    
    bool promptFlag;
};
    
} // end namespace

#endif
