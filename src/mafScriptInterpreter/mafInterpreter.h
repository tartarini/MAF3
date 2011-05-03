/* mafInterpreter.h --- 
 * 
 * Author: Julien Wintz
 * Copyright (C) 2008 - Julien Wintz, Inria.
 * Created: Fri Apr 10 15:29:01 2009 (+0200)
 * Version: $Id$
 * Last-Updated: Fri Sep 11 13:43:41 2009 (+0200)
 *           By: Julien Wintz
 *     Update #: 40
 */

/* Commentary: 
 * 
 */

/* Change log:
 * 
 */

#ifndef MAFINTERPRETER_H
#define MAFINTERPRETER_H

#include "mafScriptInterpreter_global.h"
#include <mafLog.h>

#include <mafTextEditor.h>

namespace mafScriptInterpreter {

class mafScriptEditor;
class mafInterpreterPreferencesWidget;

class MAFSCRIPTINTERPRETERSHARED_EXPORT mafInterpreter : public mafTextEditor
{
    Q_OBJECT

public:
     mafInterpreter(QWidget *parent = 0);
    ~mafInterpreter(void);

    void  readSettings(void);
    void writeSettings(void);

    void registerInterpreter(mafScriptEditor *interpreter);
    void registerAsHandler(mafLog::Handler handler);

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
